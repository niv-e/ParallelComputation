#pragma once

typedef struct {
	int x;
	int y;
}Point;

typedef struct {
	char id[10];
	int size;
	int* elements;
} Picture;

typedef struct {
	char id[10];
	int size;
	int* elements;
} Object;

typedef struct {
	double matchingValue;
	Picture* pictures;
	int numOfPictures;
	Object* objects;
	int numOfObjects;
} InputData;

typedef struct {
	Object* object;
	Point* matchPoint;
}ObjectMatch;

typedef struct {
	int numOfMatches;
	Picture* picture;
	ObjectMatch* objectMatchs;
}PictureObjectMatch;