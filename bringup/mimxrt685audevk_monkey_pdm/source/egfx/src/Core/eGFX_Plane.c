#include "eGFX_Config.h"
#include "eGFX_DataTypes.h"
#include "eGFX_PixelOps.h"
#include "eGFX_Blit.h"
#include "eGFX_Plane.h"


uint8_t eGFX_WEAK eGFX_GetBitFromArray(uint8_t *DataArray,uint16_t Bit)
{
    uint8_t Mask;
    Mask = 1 << (7-(Bit&0x7));

    if(DataArray[Bit>>3] & Mask)
        return 1;
    else
        return 0;
}

//This function grabs an 8-bit chunk from each row of an image plane.   Assumes we are byte aligned and is really only used for 1BPP and 4BPP
//Image types to assist a driver
//X is the starting pixel.   For 1BPP It will be byte aligned. I.e.  requesting X = 12 will get you the byte for X starting at 8.
//for 4BPP, requesting X = 13 will get you the byte for X starting for 12

uint8_t eGFX_WEAK eGFX_ImagePlane_GetByte(eGFX_ImagePlane   *Image,
                                          uint16_t X,
                                          uint16_t Y)
{
    uint32_t Offset;
    uint32_t MemWidthInBytes;
    uint8_t RetVal = 0;

    switch(Image->Type)
    {
        case eGFX_IMAGE_PLANE_4BPP_XBGR:
        case eGFX_IMAGE_PLANE_4BPP:

                  if(X > Image->SizeX)   //Make sure we aren't asking for a chunk greater than the width of one row
                    {
                                X = (Image->SizeX - 1);
                    }
                  if(Y > Image->SizeY)
                    {
                                  X = (Image->SizeY- - 1);
                    }

                    X &= ~((uint16_t)0x1); // Make sure we are on a byte boundary

                    //Properly Implement Rounding for odd bit plane sizes
                      MemWidthInBytes  = (Image->SizeX)>>1;

                      if(Image->SizeX & 0x1)
                                MemWidthInBytes++;

                      Offset = (Y * MemWidthInBytes) + (X>>1);
                      RetVal = Image->Data[Offset];

                break;

        case eGFX_IMAGE_PLANE_1BPP:

                  if(X > Image->SizeX)   //Make sure we aren't asking for a chunk greater than the width of one row
                    {
                        X = (Image->SizeX - 1);
                    }

                  if(Y > Image->SizeY)
                  {
                          X = (Image->SizeY- - 1);
                  }

                    X &= ~((uint16_t)0x7); // Make sure we are on a byte boundary
                    //Properly Implement Rounding for odd bit plane sizes
                    MemWidthInBytes  = (Image->SizeX)>>3;

                    if(Image->SizeX & 0x7)
                        MemWidthInBytes++;

                    Offset = (Y * MemWidthInBytes) + (X>>3);
                    RetVal = Image->Data[Offset];

                    break;

        default:

                // Other image planes not supported
                

                break;
    }
                
                return RetVal;
}

void eGFX_WEAK eGFX_ImagePlaneInit(eGFX_ImagePlane   *Image,
                                   uint8_t * Store,
                                   int16_t SizeX,
                                   int16_t SizeY,
                                   uint32_t Type)
{
    Image->Data = Store;
    Image->SizeX = SizeX;
    Image->SizeY = SizeY;
    Image->Type = Type;
}

void eGFX_WEAK eGFX_ImagePlane_Clear(eGFX_ImagePlane   *Image)
{
    uint32_t PlaneSpaceSize = 0;
    uint32_t i = 0;

    switch(Image->Type)
    {
        case  eGFX_IMAGE_PLANE_1BPP:
            PlaneSpaceSize = eGFX_CALCULATE_1BPP_IMAGE_STORAGE_SPACE_SIZE(Image->SizeX,Image->SizeY);

            for(i=0; i<PlaneSpaceSize; i++)
            {
                Image->Data[i] = 0;
            }

            break;

        case  eGFX_IMAGE_PLANE_4BPP_XBGR:
        case  eGFX_IMAGE_PLANE_4BPP:
            PlaneSpaceSize = eGFX_CALCULATE_4BPP_IMAGE_STORAGE_SPACE_SIZE(Image->SizeX,Image->SizeY);

            for(i=0; i<PlaneSpaceSize; i++)
            {
                Image->Data[i] = 0;
            }

            break;

        case  eGFX_IMAGE_PLANE_3BPP_BGR:
            PlaneSpaceSize = eGFX_CALCULATE_3BPP_IMAGE_STORAGE_SPACE_SIZE(Image->SizeX,Image->SizeY);

            for(i=0; i<PlaneSpaceSize>>2; i++)
            {
              ((uint32_t *)(Image->Data))[i] = 0;
            }

            break;

        case  eGFX_IMAGE_PLANE_8BPP:
            PlaneSpaceSize = eGFX_CALCULATE_8BPP_IMAGE_STORAGE_SPACE_SIZE(Image->SizeX,Image->SizeY);

            for(i=0; i<PlaneSpaceSize; i++)
            {
                Image->Data[i] = 0;
            }

            break;

        case  eGFX_IMAGE_PLANE_16BPP_RGB565:
		case  eGFX_IMAGE_PLANE_16BPP_BGR565:

        case  eGFX_IMAGE_PLANE_16BPP_GBRG3553:
        case  eGFX_IMAGE_PLANE_16BPP_GRBG3553:

            PlaneSpaceSize = eGFX_CALCULATE_16BPP_IMAGE_STORAGE_SPACE_SIZE(Image->SizeX,Image->SizeY);

            for(i=0; i<PlaneSpaceSize>>2; i++)
            {
                 ((uint32_t *)(Image->Data))[i] = 0;
            }

            break;
 
        case  eGFX_IMAGE_PLANE_24BPP:
            PlaneSpaceSize = eGFX_CALCULATE_24BPP_IMAGE_STORAGE_SPACE_SIZE(Image->SizeX,Image->SizeY);
                            
            for(i=0; i<PlaneSpaceSize; i++)
            {
                Image->Data[i] = 0;
            }
        
            break;
        
        case  eGFX_IMAGE_PLANE_32BPP:
        case  eGFX_IMAGE_PLANE_32BPP_XRGB888:

            PlaneSpaceSize = eGFX_CALCULATE_32BPP_IMAGE_STORAGE_SPACE_SIZE(Image->SizeX,Image->SizeY);

            for(i=0; i<PlaneSpaceSize; i++)
            {
                Image->Data[i] = 0;
            }

            break;
                        
        default:
            break;
    }
}
