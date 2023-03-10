#pragma once

typedef struct {
	int x;
	int y;
}Point;

typedef struct {
	char id[10];
	int size;
	int** elements;
} Picture;

typedef struct {
	char id[10];
	int size;
	int** elements;
} Object;

typedef struct {
	Object* object;
	Point* matchPoint;
}ObjectMatch;

typedef struct {
	Picture* picture;
	ObjectMatch** objectMatchs;
	int numOfMatches;
}PictureObjectMatch;