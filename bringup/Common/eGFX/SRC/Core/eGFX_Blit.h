#ifdef __cplusplus
extern "C" {
#endif

#ifndef eGFX_BLIT_H
#define eGFX_BLIT_H

void eGFX_WEAK eGFX_Blit(eGFX_ImagePlane *Destination,
                         int32_t x,
                         int32_t y,
                         const eGFX_ImagePlane *Sprite);

void eGFX_WEAK eGFX_BlitRestore(eGFX_ImagePlane *Destination,
				eGFX_Rect *Region,
				eGFX_ImagePlane *Source);

void eGFX_WEAK eGFX_BlitTransparentPixel(eGFX_ImagePlane *Destination,
	int32_t x,
	int32_t y,
	const eGFX_ImagePlane *Sprite,
	eGFX_PixelState TransparentPixel);


void eGFX_WEAK eGFX_BlitCentered(eGFX_ImagePlane *Destination,
                                 int32_t x,
                                 int32_t y,
                                 const eGFX_ImagePlane *Sprite);

void eGFX_WEAK eGFX_BlitFaded(eGFX_ImagePlane *Destination,
                              int32_t x,
                              int32_t y,
                              const eGFX_ImagePlane *Sprite,
                              eGFX_PixelState PS);
                            
void eGFX_WEAK eGFX_BlitShaded(eGFX_ImagePlane *Destination,
                               int32_t x,
                               int32_t y,
                               const eGFX_ImagePlane *Sprite,
                               float Shading);                            

void eGFX_WEAK eGFX_BlitColored(eGFX_ImagePlane *Destination,
                                int16_t x,
                                int16_t y,
                                const eGFX_ImagePlane *Sprite,
                                uint32_t Color);

void eGFX_WEAK eGFX_ScaledBlit(eGFX_ImagePlane *Destination,
                               int32_t x,
                               int32_t y,
                               const eGFX_ImagePlane *Sprite,
                               float ScaleFactor);
                               
#endif

#ifdef __cplusplus
 }
#endif
