#include "eGFX.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifndef eGFX_PIXEL_OPS_H
#define eGFX_PIXEL_OPS_H

void eGFX_WEAK eGFX_BitStreamPut(uint8_t * BitStream, uint32_t Position, uint32_t PixelState);

uint32_t eGFX_WEAK eGFX_BitStreamGet(uint8_t * BitStream, uint32_t Position);

eGFX_PutPixel_t* eGFX_GetPutPixelFunc(const eGFX_ImagePlane* Image);

void eGFX_WEAK eGFX_PutPixel(const eGFX_ImagePlane *Image,
                             uint16_t x,
                             uint16_t y,
                             eGFX_PixelState PS);

void eGFX_WEAK eGFX_PutPixel_Stub(const eGFX_ImagePlane *Image,
                                  uint16_t x,
                                  uint16_t y,
                                  eGFX_PixelState PS);

void eGFX_WEAK eGFX_PutPixel_1BPP(const eGFX_ImagePlane *Image,
                                  uint16_t x,
                                  uint16_t y,
                                  eGFX_PixelState PS);

void eGFX_WEAK eGFX_PutPixel_3BPP(const eGFX_ImagePlane *Image,
                                  uint16_t x,
                                  uint16_t y,
                                  eGFX_PixelState PS);

 void eGFX_WEAK eGFX_PutPixel_4BPP(const eGFX_ImagePlane *Image,
                                  uint16_t x,
                                  uint16_t y,
                                  eGFX_PixelState PS);

void eGFX_WEAK eGFX_PutPixel_8BPP(const eGFX_ImagePlane *Image,
                                  uint16_t x,
                                  uint16_t y,
                                  eGFX_PixelState PS);

void eGFX_WEAK eGFX_PutPixel_16BPP(const eGFX_ImagePlane *Image,
                                  uint16_t x,
                                  uint16_t y,
                                  eGFX_PixelState PS);

void eGFX_WEAK eGFX_PutPixel_24BPP(const eGFX_ImagePlane *Image,
                                  uint16_t x,
                                  uint16_t y,
                                  eGFX_PixelState PS);   

void eGFX_WEAK eGFX_PutPixel_32BPP(const eGFX_ImagePlane *Image,
                                  uint16_t x,
                                  uint16_t y,
                                  eGFX_PixelState PS);                                  

eGFX_GetPixel_t *eGFX_GetGetPixelFunc(const eGFX_ImagePlane *IP);

eGFX_PixelState eGFX_WEAK eGFX_GetPixel(const eGFX_ImagePlane *Image,
                                        uint16_t x,
                                        uint16_t y
);          

eGFX_PixelState eGFX_WEAK eGFX_GetPixel_Stub(const eGFX_ImagePlane *Image,
                                  uint16_t x,
                                  uint16_t y
                                  );

eGFX_PixelState eGFX_WEAK eGFX_GetPixel_1BPP(const eGFX_ImagePlane *Image,
                                  uint16_t x,
                                  uint16_t y
                                  );

eGFX_PixelState eGFX_WEAK eGFX_GetPixel_3BPP(const eGFX_ImagePlane *Image,
                                  uint16_t x,
                                  uint16_t y
                                  );

eGFX_PixelState eGFX_WEAK eGFX_GetPixel_4BPP(const eGFX_ImagePlane *Image,
                                  uint16_t x,
                                  uint16_t y
                                  );

eGFX_PixelState eGFX_WEAK eGFX_GetPixel_8BPP(const eGFX_ImagePlane *Image,
                                  uint16_t x,
                                  uint16_t y
                                  );

eGFX_PixelState eGFX_WEAK eGFX_GetPixel_16BPP(const eGFX_ImagePlane *Image,
                                  uint16_t x,
                                  uint16_t y
                                  );

eGFX_PixelState eGFX_WEAK eGFX_GetPixel_24BPP(const eGFX_ImagePlane *Image,
                                  uint16_t x,
                                  uint16_t y
                                  );   

eGFX_PixelState eGFX_WEAK eGFX_GetPixel_32BPP(const eGFX_ImagePlane *Image,
                                  uint16_t x,
                                  uint16_t y
                                  );                                         


#endif

#ifdef __cplusplus
 }
#endif
