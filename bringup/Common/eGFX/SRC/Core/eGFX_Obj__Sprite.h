#include "eGFX_DataTypes.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifndef eGFX_OBJ_SPRITE_H
#define eGFX_OBJ_SPRITE_H

	void eGFX_InitObject_Sprite(eGFX_Obj_Sprite * S,
		eGFX_ImagePlane * Sprite,
		eGFX_ImagePlane ** Target
	);

	void    eGFX_DrawObject_Sprite(eGFX_Obj_Sprite *S, eGFX_ObjectDrawCommand Cmd);


#endif

#ifdef __cplusplus
 }
#endif
