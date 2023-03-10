#include "../include/input.h"
#include "stdlib.h"

//Declare the functions privately
static Picture readPicture(FILE *file);
static Object readObject(FILE *file);

Picture readPicture(FILE *file) {
	  Picture picture;
	  fscanf(file, "%s", &picture.id);
	  fscanf(file, "%d", & picture.size);
	  picture.elements = (int**)malloc(sizeof(int*) * picture.size);

	  if (picture.elements == NULL) {
		  fprintf(stderr, "Error: Memory allocation failed\n");
		  exit(1);
	  }

	  for (int i = 0; i < picture.size; i++) {

		picture.elements[i] = (int*)malloc(sizeof(int) * picture.size);

		if (picture.elements[i] == NULL) {
			fprintf(stderr, "Error: Memory allocation failed\n");
			exit(1);
		}

		for (int j = 0; j < picture.size; j++) {
		  fscanf(file, "%d", & picture.elements[i][j]);
		}
	  }

	  return picture;
}

Object readObject(FILE *file) {
	  Object object;
	  fscanf(file, "%s", &object.id);
	  fscanf(file, "%d", &object.size);

	  object.elements = (int**)malloc(sizeof(int*) * object.size);

	  if (object.elements == NULL) {
		  fprintf(stderr, "Error: Memory allocation failed\n");
		  exit(1);
	  }

	  for (int i = 0; i < object.size; i++) {

		  object.elements[i] = (int*)malloc(sizeof(int) * object.size);

		  if (object.elements[i] == NULL) {
			  fprintf(stderr, "Error: Memory allocation failed\n");
			  exit(1);
		  }


		for (int j = 0; j < object.size; j++) {
		  fscanf(file, "%d", &object.elements[i][j]);
		}
	  }


	  return object;
}

InputData* readInputFile(const char *filename) {
	InputData* data = (InputData*)malloc(sizeof(InputData));

	if (data == NULL) {
		goto Error;
	}
	
	FILE *file = fopen(filename, "r");

	fscanf(file, "%lf", &data->matchingValue);

	fscanf(file, "%d", &data->numOfPictures);
	data->pictures = malloc(data->numOfPictures * sizeof(Object));
	
	for (int i = 0; i < data->numOfPictures; i++) {
		data->pictures[i] = readPicture(file);
	}

	fscanf(file, "%d", &data->numOfObjects);
	data->objects = malloc(data->numOfObjects * sizeof(Object));
	for (int i = 0; i < data->numOfObjects; i++) {
		data->objects[i] = readObject(file);
	}

	fclose(file);
	return data;

	Error:
	  fprintf(stderr, "Error: Memory allocation failed\n");
	  return NULL;
}


void printInputFile(const InputData * data)
{
	printf("Matching value: %f\n", data->matchingValue);

	printf("Number of Pictures: %d\n", data->numOfPictures);
	for (int i = 0; i < data->numOfPictures; i++) {
		Picture picture = data->pictures[i];
		printf("PictureId: %s\n", picture.id);
		printf("Size: %d\n", picture.size);
		printf("Elements:\n");
		for (int j = 0; j < picture.size; j++) {
			for (int k = 0; k < picture.size; k++) {
				printf("%d ", picture.elements[j][k]);
			}
			printf("\n");
		}
	}

	printf("Number of Objects: %d\n", data->numOfObjects);
	for (int i = 0; i < data->numOfObjects; i++) {
		Object object = data->objects[i];
		printf("ObjectId: %s\n", object.id);
		printf("Size: %d\n", object.size);
		printf("Elements:\n");
		for (int j = 0; j < object.size; j++) {
			for (int k = 0; k < object.size; k++) {
				printf("%d ", object.elements[j][k]);
			}
			printf("\n");
		}
	}
}



void freeInputDate(InputData* inputData){

	for (int i = 0; i < inputData->pictures->size; i++) {
		free(inputData->pictures->elements[i]);
	}
	free(inputData->pictures);


	for (int i = 0; i < inputData->objects->size; i++) {
		free(inputData->objects->elements[i]);
	}
	free(inputData->objects);

	free(inputData);
}

void writeOutputFile(PictureObjectMatch** matches,int size , char * filePath)
{		
	FILE *fp = fopen(filePath, "w");
	
	if (fp == NULL) {
		printf("Error opening file!\n");
		return 1;
	}

	char buffer[1024] = "";


	for (int i = 0; i < size; i++) {
		if (matches[i]->numOfMatches < 3) {
			fprintf(fp, "%s: No three different Objects were found\n", matches[i]->picture->id);
		}
		else{
			fprintf(fp, "%s: found Objects :",matches[i]->picture->id);

			for (int j = 0; j < matches[i]->numOfMatches; j++) {
				fprintf(fp, " %s Position(%d,%d) ;",
					matches[i]->objectMatchs[j]->object->id,
					matches[i]->objectMatchs[j]->matchPoint->x,
					matches[i]->objectMatchs[j]->matchPoint->y);
			}

			fprintf(fp, "\n");

		}
	}

	fclose(fp);
}

	
