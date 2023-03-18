#pragma once

#include "..\include\HelperStructes.h"

static const t_NUM_OF_PICS = 10;
static const t_PIC = 11;
static const t_NUM_OF_OBJS = 12;
static const t_OBJ = 11;

static const t_TERMINATE = 99;


typedef struct {
	int numOfPictures;
	Picture* pictures;
	int numOfObjects;
	Object* objects;
} MPI_DATA_STRUCT;

void CreateMPIDataStruct(int numOfPic, int numOfObj);