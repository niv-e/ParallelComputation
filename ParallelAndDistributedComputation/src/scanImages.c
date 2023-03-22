#include <math.h>
#include "../include/scanImages.h"
#include "stdlib.h"
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

static Point* IsObjectInPicture(Picture * picture, Object * object, float matchingValue);

//should convert to CUDA kernal function
double calculateDiff(int p, int o) {
	return fabs((p - o) / p);
}

double matching(Picture * picture, Object * object, Point* matchingPoint) {

	int diff = 0;
	int pictureX = 0, pictureY = 0;
	int size = object->size;
	int i, j = 0;

	#pragma omp for
	for (i = 0; i < size; i++) {
		j++;
		pictureX = (matchingPoint->x) + i;
		
		for (j = 0; j < object->size; j++) {
			pictureY = (matchingPoint->y) + j;
			diff += calculateDiff(*(picture->elements + (pictureX * picture->size) + pictureY), *(object->elements + (i * object->size) + j));
		}
	}
 	return (double)diff / (double)(object->size * object->size);
}

static ObjectMatch* createPictureObjectMatch(ObjectMatch* ptr, Object * object, Point* matchingPoint);

ObjectMatch* createPictureObjectMatch(ObjectMatch* ptr, Object * object, Point* matchingPoint) {
	//ObjectMatch currentMatch;
	ptr->matchPoint = matchingPoint;
	ptr->object = object;

	return ptr;
}

Point* IsObjectInPicture(Picture * picture, Object * object, double matchingValue)
{

	Point* matchingPoint = (Point*)malloc(sizeof(Point));

	for (int x = 0; x <= picture->size - object->size; x++) {
		for (int y = 0; y <= picture->size - object->size; y++) {
			matchingPoint->x = x;
			matchingPoint->y = y;

			double matchingResult = matching(picture, object, matchingPoint);

			if (matchingResult < matchingValue)
			{
				Point* foundedMatchingPoint = (Point*)malloc(sizeof(Point));
				foundedMatchingPoint->x = x;
				foundedMatchingPoint->y = y;
				return foundedMatchingPoint;
			}
		}
	}

	free(matchingPoint);

	return NULL;
}

//Should Parallel using MPI
PictureObjectMatch* findAllObjectsMatches(InputData * data)
{
	PictureObjectMatch* pictureObjectMatches = (PictureObjectMatch*)malloc(sizeof(PictureObjectMatch) * data->numOfPictures);
	
	for (int picture = 0; picture < data->numOfPictures; picture++) {

		PictureObjectMatch pictureObjectMatch;

 		pictureObjectMatch.picture = &data->pictures[picture];

		pictureObjectMatch.objectMatchs = NULL;

		pictureObjectMatch.numOfMatches = 0;

		pictureObjectMatches[picture] = pictureObjectMatch;

		for (int object = 0; object < data->numOfObjects; object++) {
			Point* matchingPoint = IsObjectInPicture(&data->pictures[picture], &data->objects[object], data->matchingValue);
		
			if (!matchingPoint == NULL) {
				if (pictureObjectMatches[picture].numOfMatches == 0) {
					
					pictureObjectMatches[picture].numOfMatches++;

					pictureObjectMatches[picture].objectMatchs = (ObjectMatch*)malloc(sizeof(ObjectMatch));

					ObjectMatch* currentMatch = createPictureObjectMatch(pictureObjectMatches[picture].objectMatchs ,&(data->objects[object]), matchingPoint);

 					continue;
				}

				pictureObjectMatches[picture].numOfMatches++;
				
				pictureObjectMatches[picture].objectMatchs =
					(ObjectMatch*)
					realloc(pictureObjectMatches[picture].objectMatchs, sizeof(ObjectMatch) * pictureObjectMatches[picture].numOfMatches);

					createPictureObjectMatch(
					(pictureObjectMatches[picture].objectMatchs + pictureObjectMatches[picture].numOfMatches -1 ),
					&(data->objects[object]),
					matchingPoint);
	
				if (pictureObjectMatches[picture].numOfMatches == 3) {
					break;
				}
			}
		}

	}
	return pictureObjectMatches;

	return NULL;
}

void freePictureObjectMatches(PictureObjectMatch ** pictureObjectMatches)
{
	int size = sizeof(pictureObjectMatches) / sizeof(PictureObjectMatch*);

	for(int i = 0; i< size; i++){
		//for (int j = 0; j < pictureObjectMatches[i]->numOfMatches; j++) {
		//	free(pictureObjectMatches[i].objectMatchs[j]->matchPoint);
		//	free(pictureObjectMatches[i].objectMatchs[j]);
		//}
		free(pictureObjectMatches[i]);
	}

}
