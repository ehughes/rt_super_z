#ifdef __cplusplus
extern "C" {
#endif

#ifndef eGFX_PIXEL_OPS_H
#define eGFX_PIXEL_OPS_H


void eGFX_WEAK eGFX_BitStreamPut(uint8_t * BitStream, uint32_t Position, uint32_t PixelState);

uint32_t eGFX_WEAK eGFX_BitStreamGet(uint8_t * BitStream, uint32_t Position);

void eGFX_WEAK eGFX_PutPixel(const eGFX_ImagePlane *Image,
                             int32_t x,
                             int32_t y,
                             eGFX_PixelState PS);

eGFX_PixelState eGFX_WEAK eGFX_GetPixel(const eGFX_ImagePlane *Image,
                                        int32_t x,
                                        int32_t y);                   

#endif

#ifdef __cplusplus
 }
#endif
