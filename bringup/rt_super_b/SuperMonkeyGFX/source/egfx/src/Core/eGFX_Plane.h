#ifdef __cplusplus
extern "C" {
#endif

#ifndef eGFX_PLANE_H
#define eGFX_PLANE_H
     
     

     
     
uint8_t eGFX_WEAK eGFX_GetBitFromArray(uint8_t *DataArray,
                                       uint16_t Bit);

uint8_t eGFX_WEAK eGFX_ImagePlane_GetByte(eGFX_ImagePlane   *Image,
                                          uint16_t X,
                                          uint16_t Y);

void eGFX_WEAK eGFX_ImagePlaneInit(eGFX_ImagePlane* Image,
                                    uint8_t* Store,
                                    int16_t SizeX,
                                    int16_t SizeY,
                                    uint32_t Type);


void eGFX_WEAK eGFX_ImagePlane_Clear(eGFX_ImagePlane   *Image);

                                   
#endif

#ifdef __cplusplus
 }
#endif
