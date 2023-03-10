#pragma once

#include <stdio.h>
#include "HelperStructes.h";



typedef struct {
  double matchingValue;
  Picture* pictures;
  int numOfPictures;
  Object* objects;
  int numOfObjects;
} InputData;

InputData* readInputFile(const char *filename);
void printInputFile(const InputData* data);
void freeInputDate(InputData* inputData);
void writeOutputFile(PictureObjectMatch** matches, int size, char* filePath);
