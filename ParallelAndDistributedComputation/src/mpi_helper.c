#include <mpi.h>
#include "..\include\mpi_helper.h"
#include <stddef.h>

void CreateMPIDataStruct(int numOfPic, int numOfObj) {
	MPI_Datatype picture_datatype, object_datatype;

	MPI_Type_contiguous(sizeof(Picture), MPI_BYTE, &picture_datatype);
	MPI_Type_commit(&picture_datatype);

	MPI_Type_contiguous(sizeof(Object), MPI_BYTE, &object_datatype);
	MPI_Type_commit(&object_datatype);

	// create a new MPI datatype for MyStruct
	MPI_Datatype MPI_DATA_TYPE;
	int blocklengths[4] = { 1, numOfPic, 1, numOfObj };
	MPI_Aint displacements[4];
	MPI_Datatype types[4] = { MPI_INT, picture_datatype, MPI_INT, object_datatype };

	displacements[0] = offsetof(MPI_DATA_STRUCT, numOfPictures);
	displacements[1] = offsetof(MPI_DATA_STRUCT, pictures);
	displacements[2] = offsetof(MPI_DATA_STRUCT, numOfObjects);
	displacements[3] = offsetof(MPI_DATA_STRUCT, objects);

	MPI_Type_create_struct(4, blocklengths, displacements, types, &MPI_DATA_TYPE);
	MPI_Type_commit(&MPI_DATA_TYPE);


}