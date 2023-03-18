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
	
	printf("after init\n");

	if (size != 2) {
		printf("Run the example with two processes only\n");
		MPI_Abort(MPI_COMM_WORLD, __LINE__);
	}
	
	MPI_Datatype MPI_DATA;
	MPI_Datatype types[3] = { MPI_CHAR, MPI_INT, MPI_UB };
	int block_lengths[3] = { 10, 1, 1 };

	MPI_Aint displacements[3] = {
		offsetof(Picture, id),
		offsetof(Picture, size),
		sizeof(Picture)
	};

	MPI_Type_create_struct(3, block_lengths, displacements, types, &MPI_DATA);

	MPI_Type_commit(&MPI_DATA);

	if (my_rank == 0) { 

		int dest = 1;

		printf("Hello world from process %d of %d  \n", my_rank, size);
		
		printf("argv[1] = %s  \n", argv[1]);

		InputData* data = readInputFile(argv[1]);

		printf("done to read inupt file", argv[1]);

		int numOfWorkerPictures = data->numOfPictures / 2;

		printf("Number of pictures for worker: %d\n", numOfWorkerPictures);

		MPI_Send(&numOfWorkerPictures, 1, MPI_INT,dest, t_NUM_OF_PICS, MPI_COMM_WORLD);

		for (int i = 0; i < numOfWorkerPictures; i++) {
			printf("Main process sent picture with id: %s \n", data->pictures[i].id);

			MPI_Send(&data->pictures[i], 1, MPI_DATA, dest, t_PIC, MPI_COMM_WORLD);
		}

		printf("Sending number of objects for worker: %d\n", data->numOfObjects);

		MPI_Send(&data->numOfObjects, 1, MPI_INT, dest, t_NUM_OF_OBJS, MPI_COMM_WORLD);

		for (int i = 0; i < data->numOfObjects; i++) {
			printf("Main process sent object with id: %s \n", data->objects[i].id);

			MPI_Send(&data->objects[i], 1, MPI_DATA, dest, t_OBJ, MPI_COMM_WORLD);
		}

		//TODO: change this function to return PictureObjectMatch* instead of PictureObjectMatch**
		PictureObjectMatch** matches = findAllObjectsMatches(data);
	}
	else
	{
		int source = 0;

		int numOfWorkerPictures;

		MPI_Recv(&numOfWorkerPictures, 1, MPI_INT, source, t_NUM_OF_PICS, MPI_COMM_WORLD, &status);
		
		printf("Worker recieved number of pictures : %d\n", numOfWorkerPictures);

		Picture* picutes = (Picture*)malloc(sizeof(Picture)*numOfWorkerPictures);
		
		for (int i = 0; i < numOfWorkerPictures; i++) {

			MPI_Recv(&picutes[i], 1, MPI_DATA, source, t_PIC, MPI_COMM_WORLD, &status);
			
			printf("Worker recieved picture with id: %s \n", picutes[i].id);

		}
		int numberOfObjects;

		MPI_Recv(&numberOfObjects, 1, MPI_INT, source, t_NUM_OF_OBJS, MPI_COMM_WORLD, &status);

		printf("Worker recieved number of pictures : %d\n", numberOfObjects);

		Object* objects = (Object*)malloc(sizeof(Object)*numberOfObjects);

		for (int i = 0; i < numberOfObjects; i++) {

			MPI_Recv(&objects[i], 1, MPI_DATA, source, t_OBJ, MPI_COMM_WORLD, &status);

			printf("Worker recieved picture with id: %s \n", objects[i].id);
		}
	}

	MPI_Finalize();
}




