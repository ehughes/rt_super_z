#ifdef __cplusplus
extern "C" {
#endif

#ifndef eGFX_PLANE_H
#define eGFX_PLANE_H
     
     
#define eGFX_CALCULATE_1BPP_BUFFER_ROW_BYTE_SIZE(x)		     ((x+7)>>3) //We add 8 to round up to the next even byte boundary
#define eGFX_CALCULATE_4BPP_BUFFER_ROW_BYTE_SIZE(x)		     ((x+1)>>1) //We add 1 to round up to the next even byte boundary
#define eGFX_CALCULATE_8BPP_BUFFER_ROW_BYTE_SIZE(x)		     (x)
#define eGFX_CALCULATE_16BPP_BUFFER_ROW_BYTE_SIZE(x)		 (x*2)
#define eGFX_CALCULATE_24BPP_BUFFER_ROW_BYTE_SIZE(x)	 	 (x*3)
#define	eGFX_CALCULATE_32BPP_BUFFER_ROW_BYTE_SIZE(x)	 	 (x*4)

#define eGFX_CALCULATE_1BPP_IMAGE_STORAGE_SPACE_SIZE(x,y)    (eGFX_CALCULATE_1BPP_BUFFER_ROW_BYTE_SIZE(x)	* y)
#define eGFX_CALCULATE_3BPP_IMAGE_STORAGE_SPACE_SIZE(x,y)    (((x * y * 3))/8) //This is a special type  for sharp 8 color memory LCDs.   Packing is done in the data buffer so the dump to the screen is simple
#define eGFX_CALCULATE_4BPP_IMAGE_STORAGE_SPACE_SIZE(x,y)    (eGFX_CALCULATE_4BPP_BUFFER_ROW_BYTE_SIZE(x)	* y)
#define eGFX_CALCULATE_8BPP_IMAGE_STORAGE_SPACE_SIZE(x,y)    (eGFX_CALCULATE_8BPP_BUFFER_ROW_BYTE_SIZE(x)	* y)
#define eGFX_CALCULATE_16BPP_IMAGE_STORAGE_SPACE_SIZE(x,y)   (eGFX_CALCULATE_16BPP_BUFFER_ROW_BYTE_SIZE(x)	* y)
#define eGFX_CALCULATE_24BPP_IMAGE_STORAGE_SPACE_SIZE(x,y)   (eGFX_CALCULATE_24BPP_BUFFER_ROW_BYTE_SIZE(x)	* y)
#define eGFX_CALCULATE_32BPP_IMAGE_STORAGE_SPACE_SIZE(x,y)   (eGFX_CALCULATE_32BPP_BUFFER_ROW_BYTE_SIZE(x)	* y)

     
     
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
