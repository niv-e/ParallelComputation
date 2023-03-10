#pragma once

#include "input.h";
#include "HelperStructes.h";


PictureObjectMatch** findAllObjectsMatches(InputData* data);

void freePictureObjectMatches(PictureObjectMatch** pictureObjectMatches);

