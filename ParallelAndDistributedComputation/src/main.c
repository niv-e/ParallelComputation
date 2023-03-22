#include "../include/input.h"
#include "../include/scanImages.h"
#include <stdio.h>
#include <string.h>
#include <Windows.h>
#include <mpi.h>
#include "../include/mpi_helper.h"

/* Parallelism strategy:
	main process read the file
	using MPI the main process will sent for each process
	the a relative number of images the all the objects

	using openMp each process will execute the GPU compution
	and 

*/
int main(int argc, char *argv[]) {
	printf("first line in main\n");

	int my_rank, size;
	MPI_Status status;   /* return status for receive */
	// Initialize the MPI environment
	MPI_Init(&argc, &argv);
	// Get the rank of the process
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
	// Get the process id 
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	
	if (size != 2) {
		printf("Run the example with two processes only\n");
		MPI_Abort(MPI_COMM_WORLD, __LINE__);
	}
	
	if (my_rank == 0) { 

		int dest = 1;

		InputData* data = readInputFile(argv[1]);

		MPI_Send(&data->matchingValue, 1, MPI_DOUBLE, dest, t_NUM_OF_PICS, MPI_COMM_WORLD);

		int numOfWorkerPictures = data->numOfPictures / 2;

		MPI_Send(&numOfWorkerPictures, 1, MPI_INT,dest, t_NUM_OF_PICS, MPI_COMM_WORLD);

		for (int i = 0; i < numOfWorkerPictures; i++) {

			MPI_Send(&data->pictures[i].id, 10, MPI_CHAR, dest, t_PIC, MPI_COMM_WORLD);

			MPI_Send(&data->pictures[i].size, 1, MPI_INT, dest, t_PIC, MPI_COMM_WORLD);

			MPI_Send(data->pictures[i].elements, data->pictures[i].size * data->pictures[i].size, MPI_INT, dest, t_PIC, MPI_COMM_WORLD);
		}

		data->pictures = data->pictures += numOfWorkerPictures;

		data->numOfPictures = data->numOfPictures -= numOfWorkerPictures;

		printf("Sending number of objects for worker: %d\n", data->numOfObjects);

		MPI_Send(&data->numOfObjects, 1, MPI_INT, dest, t_NUM_OF_OBJS, MPI_COMM_WORLD);

		for (int i = 0; i < data->numOfObjects; i++) {
			printf("Main process sent object with id: %s \n", data->objects[i].id);

			MPI_Send(&data->objects[i].id, 10, MPI_CHAR, dest, t_PIC, MPI_COMM_WORLD);

			MPI_Send(&data->objects[i].size, 1, MPI_INT, dest, t_PIC, MPI_COMM_WORLD);

			MPI_Send(data->objects[i].elements, data->objects[i].size * data->objects[i].size, MPI_INT, dest, t_PIC, MPI_COMM_WORLD);

		}
		
		PictureObjectMatch* matches = findAllObjectsMatches(data);

		char* output = BuildOutput(matches, data->numOfPictures, "");

		char* message = NULL;

		int message_size = 0;

		MPI_Probe(dest, t_TERMINATE, MPI_COMM_WORLD, &status);

		MPI_Get_count(&status, MPI_CHAR, &message_size);

		message = (char*)malloc(message_size * sizeof(char));
		
		MPI_Recv(message, message_size, MPI_CHAR, dest, t_TERMINATE, MPI_COMM_WORLD, &status);

		WriteToFile("output.txt", output);
		WriteToFile("output.txt", message);
		
		free(message);

	}
	else
	{
		int source = 0;

		int numOfWorkerPictures;
		
		double matchingVal;

		MPI_Recv(&matchingVal, 1, MPI_DOUBLE, source, t_NUM_OF_PICS, MPI_COMM_WORLD, &status);

		MPI_Recv(&numOfWorkerPictures, 1, MPI_INT, source, t_NUM_OF_PICS, MPI_COMM_WORLD, &status);

		Picture* pictures = (Picture*)malloc(sizeof(Picture)*numOfWorkerPictures);

		for (int i = 0; i < numOfWorkerPictures; i++) {
			
			MPI_Recv(&pictures[i].id, 10, MPI_CHAR, source, t_PIC, MPI_COMM_WORLD, &status);

			MPI_Recv(&pictures[i].size, 1, MPI_INT, source, t_PIC, MPI_COMM_WORLD, &status);

			pictures[i].elements = (int*)malloc(sizeof(int) * pictures[i].size * pictures[i].size);

			MPI_Recv(pictures[i].elements, pictures[i].size * pictures[i].size, MPI_INT, source, t_PIC, MPI_COMM_WORLD, &status);

		}
		int numberOfObjects;

		MPI_Recv(&numberOfObjects, 1, MPI_INT, source, t_NUM_OF_OBJS, MPI_COMM_WORLD, &status);

		Object* objects = (Object*)malloc(sizeof(Object)*numberOfObjects);

		for (int i = 0; i < numberOfObjects; i++) {

			MPI_Recv(&objects[i].id, 10, MPI_CHAR, source, t_PIC, MPI_COMM_WORLD, &status);

			MPI_Recv(&objects[i].size, 1, MPI_INT, source, t_PIC, MPI_COMM_WORLD, &status);

			objects[i].elements = (int*)malloc(sizeof(int) * objects[i].size * objects[i].size);

			MPI_Recv(objects[i].elements, objects[i].size * objects[i].size, MPI_INT, source, t_PIC, MPI_COMM_WORLD, &status);
		}

		InputData* data = (InputData*)malloc(sizeof(InputData));

		data->matchingValue = matchingVal;
		data->numOfPictures = numOfWorkerPictures;
		data->pictures = pictures;
		data->numOfObjects = numberOfObjects;
		data->objects = objects;

		PictureObjectMatch* matches = findAllObjectsMatches(data);

		char* output = BuildOutput(matches, data->numOfPictures, "");

		MPI_Send(output, strlen(output) + 1, MPI_CHAR, source, t_TERMINATE, MPI_COMM_WORLD);
	}
	MPI_Finalize();
}




