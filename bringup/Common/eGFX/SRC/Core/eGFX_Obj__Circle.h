#include "eGFX_DataTypes.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifndef eGFX_OBJ_CIRCLE_H
#define eGFX_OBJ_CIRCLE_H
#endif

void eGFX_InitObject_Circle(eGFX_Obj_Circle * S,
						   uint32_t Radius,
						   eGFX_ImagePlane ** Target
);

void    eGFX_DrawObject_Circle(eGFX_Obj_Circle *S,  eGFX_ObjectDrawCommand Cmd);

#ifdef __cplusplus
 }
#endif
