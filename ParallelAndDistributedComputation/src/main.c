#include "../include/input.h"
#include "../include/scanImages.h"
#include <stdio.h>
#include <string.h>
#include <Windows.h>


int main(int argc, char *argv[]) {

	if (argc != 2) {
		printf("Usage: %s <input_file>\n", argv[0]);
		return 1;
	}

	InputData* data = readInputFile(argv[1]);

	printInputFile(data);
	
	LARGE_INTEGER start, end, frequency;
	QueryPerformanceFrequency(&frequency);
	QueryPerformanceCounter(&start);

	PictureObjectMatch** matches = findAllObjectsMatches(data);

	QueryPerformanceCounter(&end);

	double duration = (double)(end.QuadPart - start.QuadPart) / frequency.QuadPart;
	printf("Execution time: %f seconds\n", duration);

	writeOutputFile(matches, data->numOfPictures, "output.txt");

	freePictureObjectMatches(matches);

	freeInputDate(data);

	return 0;
}




