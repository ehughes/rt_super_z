#include "stdint.h"
#include "stdbool.h"

#ifndef eGFX_DATA_TYPES_H
#define eGFX_DATA_TYPES_H

#ifndef eGFX_TRUE
      #define eGFX_TRUE 1
#endif

#ifndef eGFX_FALSE
 	#define eGFX_FALSE 0
#endif

#ifndef  NULL
  #define NULL 0
 #endif


#ifndef M_PI
#define M_PI 3.14159265358979323846264338327950288
#endif


#define 	eGFX_IMAGE_PLANE_1BPP	   0x0100
#define		eGFX_IMAGE_PLANE_3BPP_BGR  0x0300
#define		eGFX_IMAGE_PLANE_4BPP      0x0400
#define		eGFX_IMAGE_PLANE_4BPP_XBGR 0x0401
#define		eGFX_IMAGE_PLANE_8BPP      0x0800
#define		eGFX_IMAGE_PLANE_16BPP_RGB565  0x1000
#define		eGFX_IMAGE_PLANE_16BPP_BGR565  0x1001
#define		eGFX_IMAGE_PLANE_16BPP_GBRG3553  0x1002
#define		eGFX_IMAGE_PLANE_16BPP_GRBG3553  0x1003
#define		eGFX_IMAGE_PLANE_24BPP  0x1800
#define		eGFX_IMAGE_PLANE_32BPP  0x2000



typedef struct
{
	uint32_t Type;
    uint8_t *Data;
    uint16_t SizeX;
    uint16_t SizeY;

} eGFX_ImagePlane;

typedef struct
{
    int32_t X;
    int32_t Y;
} eGFX_Point;

typedef struct
{
	int32_t X;
	int32_t Y;
} eGFX_Vec2;

typedef struct
{
    float X;    
    float Y;  
} eGFX_PointF;

typedef struct
{
    eGFX_Point P1;
    eGFX_Point P2;
} eGFX_Box;


typedef struct
{
	eGFX_Point P1;
	eGFX_Point P2;
} eGFX_Rect;


/***
 *            ____ _______  __   ____      _
 *       ___ / ___|  ___\ \/ /  / ___|___ | | ___  _ __ ___
 *      / _ \ |  _| |_   \  /  | |   / _ \| |/ _ \| '__/ __|
 *     |  __/ |_| |  _|  /  \  | |__| (_) | | (_) | |  \__ \
 *      \___|\____|_|   /_/\_\  \____\___/|_|\___/|_|  |___/
 *
 */



typedef uint32_t eGFX_PixelState;


#define eGFX_RGB888_TO_RGB565(R,G,B)     ( ((R>>3)<<11) | ((G>>2)<<5) | (B>>3))

#define eGFX_PIXEL_ON  				     0xFFFFFFFF
#define eGFX_PIXEL_OFF    				0

#define eGFX_3BPP_RGB__BLACK			0
#define eGFX_3BPP_RGB__RED				1
#define eGFX_3BPP_RGB__GREEN			2
#define eGFX_3BPP_RGB__BLUE				4
#define eGFX_3BPP_RGB__YELLOW			3
#define eGFX_3BPP_RGB__CYAN				6
#define eGFX_3BPP_RGB__MAGENTA			5
#define eGFX_3BPP_RGB__WHITE			7

#define eGFX_4BPP_XBGR__BLACK			0
#define eGFX_4BPP_XBGR__RED				1
#define eGFX_4BPP_XBGR__GREEN			2
#define eGFX_4BPP_XBGR__BLUE			4
#define eGFX_4BPP_XBGR__YELLOW			3
#define eGFX_4BPP_XBGR__CYAN			6
#define eGFX_4BPP_XBGR__MAGENTA			5
#define eGFX_4BPP_XBGR__WHITE			7

#define ROUND_TO_INT16_T(x)     ((int16_t)(x + 0.5f))


typedef struct
{
    const eGFX_ImagePlane *CharacterSprites[0x80-0x20];
  
        //Some TT Metrics
        int32_t Ascent; //Ascent scaled up by 65536 to store fractional value           (Q15.16)
        int32_t Descent; //Descent scaled up by 65536 to store fractional value         (Q15.16)
        int32_t LineSpacing; //Linespacing scaled up by 65536 to store fractional value  (Q15.16)
        
        uint8_t  SpacesPerTab;
        uint8_t  InterCharacterSpacing;
   
        uint8_t  Padding[2];

} eGFX_Font;



/***
*       ____  _     _           _
*      / __ \| |   (_)         | |
*     | |  | | |__  _  ___  ___| |_ ___
*     | |  | | '_ \| |/ _ \/ __| __/ __|
*     | |__| | |_) | |  __/ (__| |_\__ \
*      \____/|_.__/| |\___|\___|\__|___/
*                 _/ |
*                |__/
*/

typedef enum
{
	eGFX_OBJECT_SPRITE         =   0x0000,
	eGFX_OBJECT_CIRCLE		   =   0x0001,
	eGFX_OBJECT_TEXT		   = 0x0002 
}eGFX_ObjectType;

typedef enum
{
   	 eGFX_OBJECT_STATE_INACTIVE          = 0,
	 eGFX_OBJECT_STATE_ACTIVE            = 1,

}eGFX_ObjectState;


typedef enum
{
	eGFX_OBJECT_DRAW_CMD__RESTORE = 0,
	eGFX_OBJECT_DRAW_CMD__PAINT = 1,
	eGFX_NUM_DRAW_CMDS
}eGFX_ObjectDrawCommand;

typedef enum
{
	eGFX_DIRTY_RECTANGLE_NO_RESTORE = 0x0,
	eGFX_DIRTY_RECTANGLE_RESTORE_BACKGROUND_COLOR = 0x01,
	eGFX_DIRTY_RECTANGLE_RESTORE_BACKGROUND_IMAGE = 0x02

}eGFX_ObjectDirtyRectangleOption;

typedef struct 
{
		eGFX_ObjectType	 Type;
        eGFX_ObjectState State;
		void *Next; //Singley Linked list
		bool Invalidated;
		eGFX_ObjectDirtyRectangleOption DirtyRectangeRestore;
		eGFX_PixelState *BackgroundColor; //Background Color to use for dirty rectangles.
										  //We use indirection so it is easier to update large groups of objects
		eGFX_ImagePlane **BackgroundImage;//Background image to use for dirty rectangles
										  //We use indirection so it is easier to update large groups of objects

		eGFX_ImagePlane **TargetImage;
		
}eGFX_ObjectHeader;




typedef enum
{
	eGFX_RENDER_OPTION_BLIT_TRANSPARENT  = 0x0,
	eGFX_RENDER_OPTION_BLIT_SOLID = 0x1
	
}eGFX_ObjectSpriteRenderOption;





/***
*       _____                 _     _         ____  _     _           _
*      / ____|               | |   (_)       / __ \| |   (_)         | |
*     | |  __ _ __ __ _ _ __ | |__  _  ___  | |  | | |__  _  ___  ___| |_ ___
*     | | |_ | '__/ _` | '_ \| '_ \| |/ __| | |  | | '_ \| |/ _ \/ __| __/ __|
*     | |__| | | | (_| | |_) | | | | | (__  | |__| | |_) | |  __/ (__| |_\__ \
*      \_____|_|  \__,_| .__/|_| |_|_|\___|  \____/|_.__/| |\___|\___|\__|___/
*                      | |                              _/ |
*                      |_|                             |__/
*/


typedef struct
{
	eGFX_ObjectHeader Header;

	eGFX_ImagePlane * Sprite;
	uint32_t InvalidIdx;

	eGFX_Rect		InvalidatedArea[2];
	eGFX_Point		Position;
	eGFX_PixelState TransparentColor; //Color to detect for transparent blits
	eGFX_ObjectSpriteRenderOption RenderOption;
	bool			CenterOnPosition;
	bool DoubleBuffered;

}eGFX_Obj_Sprite;



typedef struct
{
	eGFX_ObjectHeader Header;
	
	int32_t Radius;
	eGFX_Point		Position;
	eGFX_PixelState Color;
	bool Filled;
	bool DoubleBuffered;
	uint32_t InvalidIdx;
	int32_t InvalidatedRadius[2];
	eGFX_Point	InvalidatedPosition[2];

}eGFX_Obj_Circle;


typedef struct
{
	eGFX_ObjectHeader Header;

	eGFX_ImagePlane * Sprite;
	eGFX_Rect		InvalidatedArea;
	eGFX_Point		Position;
	eGFX_PixelState TransparentColor; //Color to detect for transparent blits
	eGFX_ObjectSpriteRenderOption RenderOption;
	bool			CenterOnPosition;

}eGFX_Obj_Text;


/***
*                     _                 _                ____  _     _           _
*         /\         (_)               | |              / __ \| |   (_)         | |
*        /  \   _ __  _ _ __ ___   __ _| |_ ___  _ __  | |  | | |__  _  ___  ___| |_ ___
*       / /\ \ | '_ \| | '_ ` _ \ / _` | __/ _ \| '__| | |  | | '_ \| |/ _ \/ __| __/ __|
*      / ____ \| | | | | | | | | | (_| | || (_) | |    | |__| | |_) | |  __/ (__| |_\__ \
*     /_/    \_\_| |_|_|_| |_| |_|\__,_|\__\___/|_|     \____/|_.__/| |\___|\___|\__|___/
*                                                                  _/ |
*                                                                 |__/
*/

typedef enum
{
	eGFX_ANIMATOR_POINT = 0x8000,
	eGFX_ANIMATOR_SCALAR = 0x8001,
}eGFX_AnimatorType;

typedef enum
{
	eGFX_ANIMATOR_STATE_INACTIVE = 0,
	eGFX_ANIMATOR_STATE_ACTIVE = 1,
	eGFX_ANIMATOR_STATE_COMPLETE = 2,
}eGFX_AnimatorState;


typedef struct
{
	eGFX_AnimatorType	 Type;
	eGFX_AnimatorState State;
	void *Next; //Singley Linked list
	eGFX_ObjectHeader *ObjectToInvalidate;

}eGFX_AnimatorHeader;




typedef enum 
{
	eGFX_ANIMATOR_MODE__FRACTIONAL_BISECT = 0,
	eGFX_ANIMATOR_MODE__LINEAR = 1

}eGFX_AnimatorMode;


typedef struct
{
	eGFX_AnimatorHeader Header;

	eGFX_AnimatorMode Mode;
	uint32_t CurrentFrame;
	uint32_t LastFrame;

	eGFX_Point End;
	eGFX_Point Start;
	eGFX_Point *Current;

	eGFX_PointF FractionToMove;

	eGFX_PointF __Frac;

	


} eGFX_Point_Animator;


typedef struct
{
	eGFX_AnimatorHeader Header;

	eGFX_AnimatorMode Mode;
	uint32_t CurrentFrame;
	uint32_t LastFrame;

	int32_t End;
	int32_t Start;
	int32_t *Current;

	float FractionToMove;
	float __Frac;

} eGFX_Scalar_Animator;

#endif
