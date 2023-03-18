#pragma once

#include <stdio.h>
#include "HelperStructes.h";





InputData* readInputFile(const char *filename);
void printInputFile(const InputData* data);
void freeInputDate(InputData* inputData);
void writeOutputFile(PictureObjectMatch** matches, int size, char* filePath);
