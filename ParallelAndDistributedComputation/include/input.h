#pragma once

#include <stdio.h>
#include "HelperStructes.h";





InputData* readInputFile(const char *filename);
void printInputFile(const InputData* data);
void freeInputDate(InputData* inputData);
char* BuildOutput(PictureObjectMatch* matches, int size, char* filePath);
void WriteToFile(char * filePath, char* content);