#include "eGFX_Config.h"
#include "eGFX_DataTypes.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifndef eGFX_PRIMITIVE_OPS_H
#define eGFX_PRIMITIVE_OPS_H

void  eGFX_DrawH_Line(eGFX_ImagePlane *Image,
                               int32_t X_Start,
                               int32_t X_Stop,
                               int32_t Y,
                               eGFX_PixelState PS);

void eGFX_WEAK eGFX_DrawV_Line(eGFX_ImagePlane *Image,
                              int32_t Y_Start,
                              int32_t YStop,
                              int32_t X,
                              eGFX_PixelState PS);    

void eGFX_WEAK eGFX_DrawSolidRectangle(eGFX_ImagePlane *Image,
							eGFX_Rect * R,
							eGFX_PixelState PS);


void eGFX_WEAK eGFX_DrawFilledBox(eGFX_ImagePlane *Image,
                                  eGFX_Box *Box,
                                  eGFX_PixelState PS);                              
                              
void eGFX_WEAK eGFX_DrawBox(eGFX_ImagePlane *Image,
                            eGFX_Box *Box,
                            eGFX_PixelState PS);

void eGFX_WEAK eGFX_DrawLine(eGFX_ImagePlane *Image,
                             int32_t X1,
                             int32_t Y1,
                             int32_t X2,
                             int32_t Y2,
                             eGFX_PixelState PS);      

void eGFX_WEAK eGFX_DrawCircle(eGFX_ImagePlane *Image,
                               int32_t x0,
                               int32_t y0,
                               int32_t radius,
                               eGFX_PixelState PS);      

void eGFX_WEAK eGFX_DrawCircleFromBackground(eGFX_ImagePlane *Image,
	eGFX_ImagePlane *Background,
	int32_t x0,
	int32_t y0,
	int32_t radius);

void eGFX_WEAK eGFX_DrawFilledCircle(eGFX_ImagePlane *Image,
                                    int16_t x0,
                                    int16_t y0,
                                    int16_t radius,
                                    eGFX_PixelState PS);                               
         
#endif

#ifdef __cplusplus
 }
#endif
