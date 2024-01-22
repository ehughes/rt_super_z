#include "eGFX_Config.h"
#include "eGFX_DataTypes.h"
#include "eGFX_PixelOps.h"
#include "eGFX_Plane.h"
#include "string.h"

/***
 *      _____ _          _    ____                       _   _
 *     |  __ (_)        | |  / __ \                     | | (_)
 *     | |__) |__  _____| | | |  | |_ __   ___ _ __ __ _| |_ _  ___  _ __  ___
 *     |  ___/ \ \/ / _ \ | | |  | | '_ \ / _ \ '__/ _` | __| |/ _ \| '_ \/ __|
 *     | |   | |>  <  __/ | | |__| | |_) |  __/ | | (_| | |_| | (_) | | | \__ \
 *     |_|   |_/_/\_\___|_|  \____/| .__/ \___|_|  \__,_|\__|_|\___/|_| |_|___/
 *                                 | |
 *                                 |_|
 */


void eGFX_WEAK eGFX_BitStreamPut(uint8_t * BitStream,
                                 uint32_t Position,
                                 uint32_t PixelState)
{
        uint32_t BaseAddress = Position >> 3;
        uint32_t Offset = Position & 0x7;

        if(PixelState)
        {
                BitStream[BaseAddress] |= (1<<Offset);
        }
        else
        {
                BitStream[BaseAddress] &= ~(1<<Offset);
        }
}

uint32_t eGFX_WEAK eGFX_BitStreamGet(uint8_t * BitStream, uint32_t Position)
{

        uint32_t BaseAddress = Position >> 3;
        uint32_t Offset = Position & 0x7;

        return  (BitStream[BaseAddress] & (1<<Offset));

}


void eGFX_WEAK eGFX_PutPixel(const eGFX_ImagePlane *Image,
                             int32_t x,
                             int32_t y,
                             eGFX_PixelState PS)
{
    uint32_t Offset;
    uint8_t Mask;
    uint32_t MemWidthInBytes;

    #if eGFX_PUT_PIXEL_FLIP_X == 1
        x = Image->SizeX - 1 - x;
    #endif
    
    #if eGFX_PUT_PIXEL_FLIP_Y == 1
        y = Image->SizeY - y;
    #endif
    
    if((x<Image->SizeX) && (y<Image->SizeY) && (x>=0) && (y>=0))
    {
        switch(Image->Type)
        {
            case  eGFX_IMAGE_PLANE_1BPP:
                //Properly Implement Rounding  for odd bit plane sizes
                                MemWidthInBytes = eGFX_CALCULATE_1BPP_BUFFER_ROW_BYTE_SIZE(Image->SizeX);

                Offset = (y * (MemWidthInBytes)) + (x>>3);
                Mask = 0x01 << (x & 0x07);

                if(PS)
                {
                    Image->Data[Offset] |= Mask;
                }
                else
                {
                    Image->Data[Offset] &= ~Mask;
                }

                break;

            case  eGFX_IMAGE_PLANE_3BPP_BGR:

                                {
                                        //This image plane type is an odd ball case for the 3-color sharp memory LCD.
                                    uint32_t BitStreamPosition = x*3 + y*(Image->SizeX*3);
                                        eGFX_BitStreamPut((uint8_t *)Image->Data, BitStreamPosition++, PS&0x01);
                                        eGFX_BitStreamPut((uint8_t *)Image->Data, BitStreamPosition++, PS&0x02);
                                    eGFX_BitStreamPut((uint8_t *)Image->Data, BitStreamPosition,   PS&0x04);
                                }
               break;


            case  eGFX_IMAGE_PLANE_4BPP_XBGR:

                //Properly Implement Rounding  for odd bit plane sizes
                MemWidthInBytes = eGFX_CALCULATE_4BPP_BUFFER_ROW_BYTE_SIZE(Image->SizeX);

                Offset = (y * (MemWidthInBytes)) + (x>>1);

                if( (x&0x01) == 0)
                {
                    Image->Data[Offset] &= ~0x0F;
                    Image->Data[Offset] |= ((uint8_t)(PS&0xF));
                }
                else
                {
                    Image->Data[Offset] &= ~0xF0;
                    Image->Data[Offset] |= ((uint8_t)(PS&0xF)<<4);
                }

            	break;

            case  eGFX_IMAGE_PLANE_4BPP:
                //Properly Implement Rounding  for odd bit plane sizes
                MemWidthInBytes = eGFX_CALCULATE_4BPP_BUFFER_ROW_BYTE_SIZE(Image->SizeX);

                Offset = (y * (MemWidthInBytes)) + (x>>1);

                if(x&0x01)
                {
                    Image->Data[Offset] &= ~0x0F;
                    Image->Data[Offset] |= ((uint8_t)(PS&0xF));
                }
                else
                {
                    Image->Data[Offset] &= ~0xF0;
                    Image->Data[Offset] |= ((uint8_t)(PS&0xF)<<4);
                }

                break;

            case  eGFX_IMAGE_PLANE_8BPP:
                MemWidthInBytes  = Image->SizeX;
                Offset = (y * MemWidthInBytes) + x;
                Image->Data[Offset] = ((uint8_t)PS);
                break;

            case  eGFX_IMAGE_PLANE_16BPP_RGB565:
			case  eGFX_IMAGE_PLANE_16BPP_BGR565:
            case  eGFX_IMAGE_PLANE_16BPP_GBRG3553:
            case  eGFX_IMAGE_PLANE_16BPP_GRBG3553:

                MemWidthInBytes  = Image->SizeX <<1;
                Offset = (y * MemWidthInBytes) + (x << 1);
                *(uint16_t *)(Image->Data + Offset) = (uint16_t)(PS);
                break;

            case  eGFX_IMAGE_PLANE_24BPP:
    
                MemWidthInBytes  = Image->SizeX * 3;
                Offset = (y * MemWidthInBytes) + (x*3);
                memcpy(&Image->Data[Offset],&PS,3);
    
            case  eGFX_IMAGE_PLANE_32BPP:
                MemWidthInBytes  = Image->SizeX << 2;
                Offset = (y * MemWidthInBytes) + (x<<2);
                *(uint32_t *)(Image->Data + Offset) = (uint32_t)(PS);
               break;
                                
            default:
                break;
        }
    }
}


eGFX_PixelState eGFX_WEAK eGFX_GetPixel(const eGFX_ImagePlane *Image,
                                        int32_t x,
                                        int32_t y)
{
    uint32_t Offset = 0;
    uint8_t Mask;
    uint32_t MemWidthInBytes;
    eGFX_PixelState PS = 0;

    if((x<Image->SizeX) && (y<Image->SizeY) && (x>=0) && (y>=0))
    {
        switch(Image->Type)
        {
            case  eGFX_IMAGE_PLANE_1BPP:
                //Properly Implement Rounding  for odd bit plane sizes
                                MemWidthInBytes = eGFX_CALCULATE_1BPP_BUFFER_ROW_BYTE_SIZE(Image->SizeX);

                Offset = (y * (MemWidthInBytes)) + (x>>3);
                Mask = 0x01 << (x & 0x07);

                if(Image->Data[Offset] & Mask)
                    PS = eGFX_PIXEL_ON;
                else
                    PS = eGFX_PIXEL_OFF;

                break;

            case  eGFX_IMAGE_PLANE_3BPP_BGR:
                        {
                        //This image plane type is an odd ball case for the 3-color sharp memory LCD.
                        uint32_t BitStreamPosition = x*3 + y*Image->SizeX*3;
                        if(eGFX_BitStreamGet((uint8_t *)Image->Data, BitStreamPosition++))
                                        PS |= 0x01;
                        if(eGFX_BitStreamGet((uint8_t *)Image->Data, BitStreamPosition++))
                                        PS |= 0x02;
                        if(eGFX_BitStreamGet((uint8_t *)Image->Data, BitStreamPosition++))
                                        PS |= 0x04;
                        }

                    break;

            case  eGFX_IMAGE_PLANE_4BPP_XBGR:

                //Properly Implement Rounding  for odd bit plane sizes
                 MemWidthInBytes = eGFX_CALCULATE_4BPP_BUFFER_ROW_BYTE_SIZE(Image->SizeX);

                 Offset = (y * (MemWidthInBytes)) + (x>>1);

                if( (x&0x01) == 0)
                    PS = Image->Data[Offset] & 0x0F;
                else
                    PS = ((Image->Data[Offset])>>4) & 0x0F;

            	break;

            case  eGFX_IMAGE_PLANE_4BPP:
                //Properly Implement Rounding  for odd bit plane sizes
                 MemWidthInBytes = eGFX_CALCULATE_4BPP_BUFFER_ROW_BYTE_SIZE(Image->SizeX);

                 Offset = (y * (MemWidthInBytes)) + (x>>1);

                if(x&0x01)
                    PS = Image->Data[Offset] & 0x0F;
                else
                    PS = ((Image->Data[Offset])>>4) & 0x0F;

                break;

            case  eGFX_IMAGE_PLANE_8BPP:
                MemWidthInBytes  = Image->SizeX;
                Offset = (y * MemWidthInBytes) + x;
                PS = Image->Data[Offset];
                break;

            case  eGFX_IMAGE_PLANE_16BPP_RGB565:
			case  eGFX_IMAGE_PLANE_16BPP_BGR565:
            case  eGFX_IMAGE_PLANE_16BPP_GBRG3553:
            case  eGFX_IMAGE_PLANE_16BPP_GRBG3553:

                MemWidthInBytes  = Image->SizeX << 1;
                Offset = (y * MemWidthInBytes) + (x << 1);
                PS = *(uint16_t *)(Image->Data + Offset);
                break;
            

            case  eGFX_IMAGE_PLANE_24BPP:
                MemWidthInBytes  = Image->SizeX * 3;
                Offset = (y * MemWidthInBytes) + (x*3);
                PS  = ((int32_t)Image->Data[Offset]);
                PS |= ((int32_t)Image->Data[Offset+1])<<8;
                PS |= ((int32_t)Image->Data[Offset+2])<<16;
                break;

            case  eGFX_IMAGE_PLANE_32BPP:
                MemWidthInBytes  = Image->SizeX << 2;
                Offset = (y * MemWidthInBytes) + (x<<2);
                PS = *(uint32_t *)(Image->Data + Offset);
                break;
                                

            default:
                break;
        }
    }

    return PS;
}
