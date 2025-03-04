#include "eGFX.h"

#if eGFX_INCLUDE_BITMAP_OUTPUT_SUPPORT == 1 

#ifndef _eGFX_BMP
#define _eGFX_BMP

void ImagePlaneToGrayScaleBMP(char *FileName, eGFX_ImagePlane * IP);

#endif

#endif