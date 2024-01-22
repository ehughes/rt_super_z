#ifdef __cplusplus
extern "C" {
#endif

#ifndef eGFX_STRING_OPS_H
#define eGFX_STRING_OPS_H

int32_t eGFX_WEAK eGFX_GetStringWidth(char *String,
                                      const eGFX_Font *MyFont);

int32_t eGFX_WEAK eGFX_DrawCharacter(eGFX_ImagePlane *Image,
                                     int32_t StartX,
                                     int32_t StartY,
                                     uint8_t Character,
                                     const eGFX_Font *MyFont);

int32_t eGFX_WEAK eGFX_ScaledDrawCharacter(eGFX_ImagePlane *Image,
                                           int32_t StartX,
                                           int32_t StartY,
                                           uint8_t Character,
                                           const eGFX_Font *MyFont,
                                           float ScaleFactor);                                     

int32_t eGFX_WEAK eGFX_DrawCharacterFaded(eGFX_ImagePlane *Image,
                                          int32_t StartX,
                                          int32_t StartY,
                                          uint8_t Character,
                                          const eGFX_Font *MyFont,
                                          eGFX_PixelState PS);                                           

int32_t eGFX_WEAK eGFX_DrawCharacterColored(eGFX_ImagePlane *Image,
                                            int32_t StartX,
                                            int32_t StartY,
                                            uint8_t Character,
                                            const eGFX_Font *MyFont,
                                            eGFX_PixelState Color);

int32_t eGFX_WEAK eGFX_DrawCharacterShaded(eGFX_ImagePlane *Image,
                                           int32_t StartX,
                                           int32_t StartY,
                                           uint8_t Character,
                                           const eGFX_Font *MyFont,
                                           float Shading);

int32_t eGFX_WEAK eGFX_GetStringWidth_CustomSpacing(char *String,
                                                    const eGFX_Font *MyFont,
                                                    uint32_t Spacing);

void eGFX_WEAK eGFX_DrawHorizontalCenteredString(eGFX_ImagePlane *Image,
                                                 int32_t StartY,
                                                 char *String,
                                                 const eGFX_Font *MyFont);                                                    

int32_t eGFX_WEAK eGFX_DrawRightAlignedCenteredString_CustomSpacing(eGFX_ImagePlane *Image,
                                                                    int32_t StartY,
                                                                    char *String,
                                                                    const eGFX_Font *MyFont,
                                                                    uint32_t CustomCharacterSpacing,
                                                                    int32_t RightSideBuffer);   

void eGFX_WEAK eGFX_DrawHorizontalCenteredString_CustomSpacing(eGFX_ImagePlane *Image,
                                                     int32_t StartY,
                                                     char *String,
                                                     const eGFX_Font *MyFont,
                                                     uint32_t Spacing);

void eGFX_WEAK eGFX_ScaledDrawString(eGFX_ImagePlane *Image,
                                     int16_t StartX,
                                     int16_t StartY,
                                     char *String,
                                     const eGFX_Font *MyFont,
                                     float ScaleFactor);

void eGFX_WEAK eGFX_ScaledDrawString_CustomSpacing(eGFX_ImagePlane *Image,
                                                   int32_t StartX,
                                                   int32_t StartY,
                                                   char *String,
                                                   const eGFX_Font *MyFont,
                                                   float ScaleFactor,
                                                   uint32_t Spacing);                                     

int32_t eGFX_WEAK eGFX_DrawString(eGFX_ImagePlane *Image,
                                  char *String,
                                  int32_t StartX,
                                  int32_t StartY,
                                  const eGFX_Font *MyFont);
                                  
int32_t eGFX_WEAK eGFX_DrawStringFaded(eGFX_ImagePlane *Image,
                              char *String,
                              int32_t StartX,
                              int32_t StartY,
                              const eGFX_Font *MyFont, 
                              eGFX_PixelState PS); 

int32_t eGFX_WEAK eGFX_DrawStringShaded(eGFX_ImagePlane *Image,
                                        char *String,
                                        int32_t StartX,
                                        int32_t StartY,
                                        const eGFX_Font *MyFont,
                                        float Shading); 

int32_t eGFX_WEAK eGFX_DrawStringColored(eGFX_ImagePlane *Image,
                                         char *String,
                                         int32_t StartX,
                                         int32_t StartY,
                                         const eGFX_Font *MyFont,
                                         uint32_t Color);  

int32_t eGFX_WEAK eGFX_printf(eGFX_ImagePlane *Image,
                              int32_t StartX,
                              int32_t StartY,
                              const eGFX_Font *MyFont,
                              char *FormatString,...);        
                              
int32_t eGFX_WEAK eGFX_printf_Colored(eGFX_ImagePlane *Image,
                                      int16_t StartX,
                                      int16_t StartY,
                                      const eGFX_Font *MyFont,
                                      uint32_t Color,
                                      char *FormatString,...);

int32_t eGFX_WEAK eGFX_printf_HorizontalCentered_Colored(eGFX_ImagePlane *Image,
                                                         int32_t StartY,
                                                         const eGFX_Font *MyFont,
                                                         uint32_t Color,
                                                         char *FormatString,...);    

int32_t eGFX_WEAK eGFX_DrawString_CustomSpacing(eGFX_ImagePlane *Image,
                                                int32_t StartX,
                                                int32_t StartY,
                                                char *String,
                                                const eGFX_Font *MyFont,
                                                uint16_t Spacing);

int32_t eGFX_WEAK eGFX_vsnprintf(char * buffer,uint32_t Len, char * FormatString,...);

#endif

#ifdef __cplusplus
}
#endif
