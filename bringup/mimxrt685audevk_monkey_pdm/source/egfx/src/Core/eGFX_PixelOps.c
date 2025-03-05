#include "eGFX_Config.h"
#include "eGFX_DataTypes.h"
#include "eGFX_PixelOps.h"
#include "eGFX_Plane.h"
#include "string.h"
#include "stdint.h"

void eGFX_WEAK eGFX_BitStreamPut(uint8_t *BitStream,
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

    return (BitStream[BaseAddress] & (1<<Offset));
}

eGFX_PutPixel_t * eGFX_GetPutPixelFunc(const eGFX_ImagePlane *Image)
{
        eGFX_PutPixel_t *PP;

        switch(eGFX_GET_BITS_PER_PIXEL_FROM_IMAGE_PLANE(Image))
        {
            case  1:
                    PP = (eGFX_PutPixel_t * )eGFX_PutPixel_1BPP;
                break;

            case  3:
                    PP = (eGFX_PutPixel_t*)eGFX_PutPixel_3BPP;
               break;

            case  4:
                    PP = (eGFX_PutPixel_t*)eGFX_PutPixel_4BPP;
                break;

            case  8:
                    PP = (eGFX_PutPixel_t*)eGFX_PutPixel_8BPP;
                break;

            case  16:
                    PP = (eGFX_PutPixel_t*)eGFX_PutPixel_16BPP;
                break;

            case  24:
                    PP = (eGFX_PutPixel_t*)eGFX_PutPixel_24BPP;
                break;

            case  32:
                   PP = (eGFX_PutPixel_t*)eGFX_PutPixel_32BPP;
               break;
                                
            default:
                   PP = (eGFX_PutPixel_t*)eGFX_PutPixel_Stub;
                break;
        }

        return PP;
}


void eGFX_WEAK eGFX_PutPixel_Stub(const eGFX_ImagePlane *Image,
                                  uint16_t x,
                                  uint16_t y,
                                  eGFX_PixelState PS)
 {

 }        

void eGFX_WEAK eGFX_PutPixel_1BPP(const eGFX_ImagePlane *Image,
                                  uint16_t x,
                                  uint16_t y,
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
 }                                 



void eGFX_WEAK eGFX_PutPixel_3BPP(const eGFX_ImagePlane *Image,
                                  uint16_t x,
                                  uint16_t y,
                                  eGFX_PixelState PS)

 {
    //This image plane type is an odd ball case for the 3-color sharp memory LCD.
    uint32_t BitStreamPosition = x*3 + y*(Image->SizeX*3);
    eGFX_BitStreamPut((uint8_t *)Image->Data, BitStreamPosition++, PS&0x01);
    eGFX_BitStreamPut((uint8_t *)Image->Data, BitStreamPosition++, PS&0x02);
    eGFX_BitStreamPut((uint8_t *)Image->Data, BitStreamPosition,   PS&0x04);
                                
 }                                


 void eGFX_WEAK eGFX_PutPixel_4BPP(const eGFX_ImagePlane *Image,
                                  uint16_t x,
                                  uint16_t y,
                                  eGFX_PixelState PS)
 {
      
    uint32_t Offset;
   
    uint32_t MemWidthInBytes;

    #if eGFX_PUT_PIXEL_FLIP_X == 1
        x = Image->SizeX - 1 - x;
    #endif
    
    #if eGFX_PUT_PIXEL_FLIP_Y == 1
        y = Image->SizeY - y;
    #endif
    
    //Properly Implement Rounding  for odd bit plane sizes
    MemWidthInBytes = eGFX_CALCULATE_4BPP_BUFFER_ROW_BYTE_SIZE(Image->SizeX);

    Offset = (y * (MemWidthInBytes)) + (x>>1);

    if(x&0x01)
    {
        ((uint8_t *)Image->Data)[Offset] &= ~0x0F;
        ((uint8_t *)Image->Data)[Offset] |= ((uint8_t)(PS&0xF));
    }
    else
    {
        ((uint8_t *)Image->Data)[Offset] &= ~0xF0;
        ((uint8_t *)Image->Data)[Offset] |= ((uint8_t)(PS&0xF)<<4);
    }
 }                                 



void eGFX_WEAK eGFX_PutPixel_8BPP(const eGFX_ImagePlane *Image,
                                  uint16_t x,
                                  uint16_t y,
                                  eGFX_PixelState PS)
 {
    uint32_t Offset;
    
    uint32_t MemWidthInBytes;

    #if eGFX_PUT_PIXEL_FLIP_X == 1
        x = Image->SizeX - 1 - x;
    #endif
    
    #if eGFX_PUT_PIXEL_FLIP_Y == 1
        y = Image->SizeY - y;
    #endif

    MemWidthInBytes  = Image->SizeX;
    Offset = (y * MemWidthInBytes) + x;
    ((uint8_t *)Image->Data)[Offset] = ((uint8_t)PS);
 }                                 



void eGFX_WEAK eGFX_PutPixel_16BPP(const eGFX_ImagePlane *Image,
                                  uint16_t x,
                                  uint16_t y,
                                  eGFX_PixelState PS)
 {
        uint32_t Offset;
        
        uint32_t MemWidthInBytes;
        
        #if eGFX_PUT_PIXEL_FLIP_X == 1
            x = Image->SizeX - 1 - x;
        #endif
        
        #if eGFX_PUT_PIXEL_FLIP_Y == 1
            y = Image->SizeY - y;
        #endif
        

        MemWidthInBytes  = Image->SizeX <<1;
        Offset = (y * MemWidthInBytes) + (x << 1);
        *(uint16_t *)(Image->Data + Offset) = (uint16_t)(PS);
 }                                 



void eGFX_WEAK eGFX_PutPixel_24BPP(const eGFX_ImagePlane *Image,
                                  uint16_t x,
                                  uint16_t y,
                                  eGFX_PixelState PS)
 {
    uint32_t Offset;
 
    uint32_t MemWidthInBytes;

    #if eGFX_PUT_PIXEL_FLIP_X == 1
        x = Image->SizeX - 1 - x;
    #endif
    
    #if eGFX_PUT_PIXEL_FLIP_Y == 1
        y = Image->SizeY - y;
    #endif

    MemWidthInBytes  = Image->SizeX * 3;
    Offset = (y * MemWidthInBytes) + (x*3);
    memcpy(&Image->Data[Offset],&PS,3);
 }                                 

 

void eGFX_WEAK eGFX_PutPixel_32BPP(const eGFX_ImagePlane *Image,
                                  uint16_t x,
                                  uint16_t y,
                                  eGFX_PixelState PS)
 {
    #if eGFX_PUT_PIXEL_FLIP_X == 1
        x = Image->SizeX - 1 - x;
    #endif
    
    #if eGFX_PUT_PIXEL_FLIP_Y == 1
        y = Image->SizeY - y;
    #endif

    ((uint32_t *)(Image->Data))[y * Image->SizeX + x] = (uint32_t)(PS);
 }                                 

                              
void eGFX_WEAK eGFX_PutPixel(const eGFX_ImagePlane *Image,
                             uint16_t x,
                             uint16_t y,
                             eGFX_PixelState PS)
{
    if((x<Image->SizeX) && (y<Image->SizeY) && (x>=0) && (y>=0))
    {
        eGFX_PutPixel_t *PP = eGFX_GetPutPixelFunc(Image);
        PP(Image,x,y,PS);        
    }
}


eGFX_PixelState eGFX_WEAK eGFX_GetPixel(const eGFX_ImagePlane *Image,
                                        uint16_t x,
                                        uint16_t y)
{
    eGFX_PixelState PS=0;

    if((x<Image->SizeX) && (y<Image->SizeY) && (x>=0) && (y>=0))
    {
        eGFX_GetPixel_t *GP = eGFX_GetGetPixelFunc(Image);
        PS = GP(Image,x,y); 
    }

    return PS;
}


eGFX_GetPixel_t *eGFX_GetGetPixelFunc(const eGFX_ImagePlane *Image)
{
        eGFX_GetPixel_t *GP;

        switch(eGFX_GET_BITS_PER_PIXEL_FROM_IMAGE_PLANE(Image))
        {
            case  1:
                    GP = (eGFX_GetPixel_t * )eGFX_GetPixel_1BPP;
                break;

            case  3:
                    GP = (eGFX_GetPixel_t * )eGFX_GetPixel_3BPP;
               break;

            case  4:
                    GP = (eGFX_GetPixel_t * )eGFX_GetPixel_4BPP;
                break;

            case  8:
                    GP = (eGFX_GetPixel_t * )eGFX_GetPixel_8BPP;
                break;

            case  16:
                    GP = (eGFX_GetPixel_t * )eGFX_GetPixel_16BPP;
                break;

            case  24:
                    GP = (eGFX_GetPixel_t * )eGFX_GetPixel_24BPP;
                break;

            case  32:
                   GP = (eGFX_GetPixel_t * )eGFX_GetPixel_32BPP;
               break;
                                
            default:
                   GP = (eGFX_GetPixel_t * )eGFX_GetPixel_Stub;
                break;
        }

        return GP;
}
        

eGFX_PixelState eGFX_WEAK eGFX_GetPixel_Stub(const eGFX_ImagePlane* Image,
    uint16_t x,
    uint16_t y
)
{
    return 0;
}

eGFX_PixelState eGFX_WEAK eGFX_GetPixel_1BPP(const eGFX_ImagePlane *Image,
                                  uint16_t x,
                                  uint16_t y
                                  )
 {
    uint32_t Offset = 0;
    uint8_t Mask;
    uint32_t MemWidthInBytes;
    eGFX_PixelState PS;

    MemWidthInBytes = eGFX_CALCULATE_1BPP_BUFFER_ROW_BYTE_SIZE(Image->SizeX);

    Offset = (y * (MemWidthInBytes)) + (x>>3);
    Mask = 0x01 << (x & 0x07);

    if(Image->Data[Offset] & Mask)
        PS = eGFX_PIXEL_ON;
    else
        PS = eGFX_PIXEL_OFF;

    return PS;
 }


eGFX_PixelState eGFX_WEAK eGFX_GetPixel_3BPP(const eGFX_ImagePlane *Image,
                                  uint16_t x,
                                  uint16_t y
)
{
    eGFX_PixelState PS = 0;
    
    uint32_t BitStreamPosition = x*3 + y*Image->SizeX*3;

    if(eGFX_BitStreamGet((uint8_t *)Image->Data, BitStreamPosition++))
        PS |= 0x01;
    if(eGFX_BitStreamGet((uint8_t *)Image->Data, BitStreamPosition++))
        PS |= 0x02;
    if(eGFX_BitStreamGet((uint8_t *)Image->Data, BitStreamPosition++))
        PS |= 0x04;

    return PS;            
}                                 



eGFX_PixelState eGFX_WEAK eGFX_GetPixel_4BPP(const eGFX_ImagePlane *Image,
                                  uint16_t x,
                                  uint16_t y
                                  )
 {
    uint32_t Offset = 0;
    
    uint32_t MemWidthInBytes;
    eGFX_PixelState PS;

    MemWidthInBytes = eGFX_CALCULATE_4BPP_BUFFER_ROW_BYTE_SIZE(Image->SizeX);

    Offset = (y * (MemWidthInBytes)) + (x>>1);

    if ((x & 0x01))
        PS =  Image->Data[Offset] & 0x0F;
    else
        PS = ((Image->Data[Offset])>>4) & 0x0F;

    return PS;
 }                                 



eGFX_PixelState eGFX_WEAK eGFX_GetPixel_8BPP(const eGFX_ImagePlane *Image,
    uint16_t x,
                                  uint16_t y
                                  )
 {
    uint32_t Offset = 0;
  
    uint32_t MemWidthInBytes;
    eGFX_PixelState PS;

    MemWidthInBytes  = Image->SizeX;
    Offset = (y * MemWidthInBytes) + x;
    PS = Image->Data[Offset];

    return PS;
 }                                 



eGFX_PixelState eGFX_WEAK eGFX_GetPixel_16BPP(const eGFX_ImagePlane *Image,
                                  uint16_t x,
                                  uint16_t y
                                  )
 {
    uint32_t Offset = 0;
    uint32_t MemWidthInBytes;
    eGFX_PixelState PS = 0;

    MemWidthInBytes  = Image->SizeX << 1;
    Offset = (y * MemWidthInBytes) + (x << 1);
    PS = *(uint16_t *)(Image->Data + Offset);

    return PS;
 }                                 



eGFX_PixelState eGFX_WEAK eGFX_GetPixel_24BPP(const eGFX_ImagePlane *Image,
                                  uint16_t x,
                                  uint16_t y
                                  )
{
    uint32_t Offset = 0;
    uint32_t MemWidthInBytes;
    eGFX_PixelState PS;

    MemWidthInBytes  = Image->SizeX * 3;
    Offset = (y * MemWidthInBytes) + (x*3);
    PS  = ((int32_t)Image->Data[Offset]);
    PS |= ((int32_t)Image->Data[Offset+1])<<8;
    PS |= ((int32_t)Image->Data[Offset+2])<<16;

    return PS;

}                                 

eGFX_PixelState eGFX_WEAK eGFX_GetPixel_32BPP(const eGFX_ImagePlane *Image,
                                  uint16_t x,
                                  uint16_t y
                                  )
{
    uint32_t Offset = 0;
    uint32_t MemWidthInBytes;
    eGFX_PixelState PS;

    MemWidthInBytes  = Image->SizeX << 2;
    Offset = (y * MemWidthInBytes) + (x<<2);
    PS = *(uint32_t *)(Image->Data + Offset);

    return PS;
}

