#include "../include/input.h"
#include "../include/scanImages.h"
#include <stdio.h>
#include <string.h>


int main(int argc, char *argv[]) {

	if (argc != 2) {
		printf("Usage: %s <input_file>\n", argv[0]);
		return 1;
	}

	InputData* data = readInputFile(argv[1]);

	printInputFile(data);
	
	PictureObjectMatch** matches = findAllObjectsMatches(data);

	writeOutputFile(matches, data->numOfPictures, "output.txt");

	freePictureObjectMatches(matches);

	freeInputDate(data);

	return 0;
}




