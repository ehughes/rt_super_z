#include "eGFX.h"

#if eGFX_INCLUDE_BITMAP_OUTPUT_SUPPORT == 1 

#ifdef WIN32_PRESENT

#include <stdio.h>

#endif

#ifdef CHAN_FAT_FS_PRESENT

#include "ff.h"

#endif

#pragma pack(1)
typedef struct tagBITMAPFILEHEADER {
  uint16_t  bfType;
  uint32_t bfSize;
  uint16_t  bfReserved1;
  uint16_t  bfReserved2;
  uint32_t bfOffBits;
} BITMAPFILEHEADER;

#pragma pack(1)
typedef struct tagBITMAPINFOHEADER {
  uint32_t biSize;
  int32_t  biWidth;
  int32_t  biHeight;
  uint16_t  biPlanes;
  uint16_t  biBitCount;
  uint32_t biCompression;
  uint32_t biSizeImage;
  int32_t  biXPelsPerMeter;
  int32_t  biYPelsPerMeter;
  uint32_t biClrUsed;
  uint32_t biClrImportant;
} BITMAPINFOHEADER;



void ImagePlaneToGrayScaleBMP(char *FileName,eGFX_ImagePlane * IP)
{
        BITMAPFILEHEADER MyBMP_FileHeader;
        BITMAPINFOHEADER MyBMP_InfoHeader;
        uint32_t i, j, k, PixelOut;

        #ifdef WIN32_PRESENT
        FILE *MyFile;
        #endif
        
        #ifdef CHAN_FAT_FS_PRESENT

        FIL MyFile;
        FRESULT R;

        #endif

        MyBMP_FileHeader.bfType =  0x4D42;
        
        i = sizeof(BITMAPFILEHEADER);
        i = sizeof(BITMAPINFOHEADER);

        MyBMP_FileHeader.bfSize = sizeof(BITMAPFILEHEADER) +
                                                          sizeof(BITMAPINFOHEADER) + 
                                                                (4 * 256) +// This routine uses index color, 8-bit for gray scale.    4 bytes per palette entry
                                                                (IP->SizeX * IP->SizeY)  ;  //Compute the size of the bit map data assuming 1 byte per pixel
                                                                
        MyBMP_FileHeader.bfReserved1 = 0;
        MyBMP_FileHeader.bfReserved2 = 0;
        
        MyBMP_FileHeader.bfOffBits = sizeof(BITMAPFILEHEADER)+
                                                                sizeof(BITMAPINFOHEADER)+
                                                                (4 * 256) ;// This routine uses indexed color, 8-bit for grey scale.    4 bytes per palette entry
        

        MyBMP_InfoHeader.biSize = sizeof(BITMAPINFOHEADER);
        MyBMP_InfoHeader.biWidth = IP->SizeX;
        MyBMP_InfoHeader.biHeight = IP->SizeY;

        MyBMP_InfoHeader.biPlanes = 1;
        MyBMP_InfoHeader.biBitCount = 8;
        MyBMP_InfoHeader.biCompression = 0;
        MyBMP_InfoHeader.biSizeImage = 0;
        MyBMP_InfoHeader.biXPelsPerMeter = 3780;
        MyBMP_InfoHeader.biYPelsPerMeter = 3780;
        MyBMP_InfoHeader.biClrUsed = 256;
        MyBMP_InfoHeader.biClrImportant = 256;

        



        #ifdef WIN32_PRESENT
        MyFile = fopen(FileName, "w");
        if (MyFile == NULL)
                return
        #endif
        #ifdef CHAN_FAT_FS_PRESENT
        R = f_open(&MyFile, FileName, FA_WRITE | FA_CREATE_ALWAYS);

        if(R!=FR_OK)
                return; 
        #endif

        #ifdef WIN32_PRESENT
                fwrite((const void *)&MyBMP_FileHeader, sizeof(uint8_t), 14, MyFile);
                fwrite((const void *)&MyBMP_InfoHeader, sizeof(uint8_t), 40, MyFile);
        #endif
        #ifdef CHAN_FAT_FS_PRESENT
                f_write(&MyFile,(const void *)&MyBMP_FileHeader,  14, &i);
                f_write(&MyFile,(const void *)&MyBMP_InfoHeader, 40, &i);
        #endif

    //Write out the grey scale color data

        for (i = 0; i < 256; i++)
        {
                        #ifdef WIN32_PRESENT
                                        //Write and RGBQUAD
                                        fputc(i, MyFile);
                                        fputc(i, MyFile);
                                        fputc(i, MyFile);
                                        fputc(i, MyFile);
                        #endif

                        #ifdef CHAN_FAT_FS_PRESENT

                                        k =   ((uint32_t)((float)(i) / (255.0) * 245.0))
                                                + (((uint32_t)((float)(i) / (255.0) * 44.0)) <<16)
                                                + (((uint32_t)((float)(i) / (255.0) * 211.0)) <<8);



                                        f_write(&MyFile,&k, 4, &j);
                                        //f_putc(i, &MyFile);
                                        //f_putc(i, &MyFile);
                                        //f_putc(i, &MyFile);
                                        //f_putc(i, &MyFile);

                        #endif
        }

        //Write out the Bitmap Data

        for (i = 0; i < IP->SizeY; i++)
        {
                for (j = 0; j < IP->SizeX; j++)
                {
                        PixelOut = eGFX_GetPixel(IP, j, IP->SizeY - i  - 1);

                        switch (IP->Type)
                        {
                                case eGFX_IMAGE_PLANE_1BPP:

                                        break;

                                case eGFX_IMAGE_PLANE_4BPP:
                                        PixelOut = (PixelOut << 4) & 0xFF;
                                        break;

                                default:
                                case eGFX_IMAGE_PLANE_8BPP:

                                        break;
                        }

                #ifdef WIN32_PRESENT
                        fputc((uint8_t)PixelOut, MyFile);
                #endif
                #ifdef CHAN_FAT_FS_PRESENT
                        f_putc((uint8_t)PixelOut, &MyFile);
                #endif
                }

                //BMPs must be on a 32-bit boundary.  Pad with some zeros.

                if (IP->SizeX & 0x3)
                {
                        for (k = 0; k < 0x4 - (IP->SizeX & 0x3); k++)
                        {
                                #ifdef WIN32
                                        fputc(0, MyFile);
                                #endif
                                #ifdef CHAN_FAT_FS_PRESENT
                                        f_putc(0, &MyFile);
                                #endif
                        }
                }
        }

        #ifdef WIN32_PRESENT
                fclose(MyFile);
        #endif

        #ifdef CHAN_FAT_FS_PRESENT
                f_close(&MyFile);
        #endif

}


#endif