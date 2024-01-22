#include "eGFX_Config.h"
#include "eGFX_DataTypes.h"
#include "eGFX_PixelOps.h"
#include "eGFX_PrimitiveOps.h"
#include "eGFX_StringOps.h"
#include "eGFX_Blit.h"
#include "eGFX_Plane.h"
#include "eGFX_Obj.h"


/***
*      ___          _ _     
*     / __|_ __ _ _(_) |_ ___
*     \__ \ '_ \ '_| |  _/ -_) 
*     |___/ .__/_| |_|\__\___|
*         |_|
*/

void eGFX_InitObject_Sprite(eGFX_Obj_Sprite * S,
	eGFX_ImagePlane * Sprite,
	eGFX_ImagePlane ** Target
)
{
	if (S != NULL)
	{
		eGFX_EnableObject(S);

		S->Header.Type = eGFX_OBJECT_SPRITE;
		S->Header.TargetImage = Target;
		S->Sprite = Sprite;
		S->RenderOption = eGFX_RENDER_OPTION_BLIT_SOLID;
		S->TransparentColor = 0;
		S->Position.X = 0;
		S->Position.Y = 0;
		S->Header.Invalidated = true;
		S->InvalidatedArea[0].P1.X = 0;
		S->InvalidatedArea[0].P2.X = 0;
		S->InvalidatedArea[0].P1.Y = 0;
		S->InvalidatedArea[0].P2.Y = 0;
		S->InvalidatedArea[1].P1.X = 0;
		S->InvalidatedArea[1].P2.X = 0;
		S->InvalidatedArea[1].P1.Y = 0;
		S->InvalidatedArea[1].P2.Y = 0;
	}

}

void    eGFX_DrawObject_Sprite(eGFX_Obj_Sprite *S, eGFX_ObjectDrawCommand Cmd)
{
	int32_t StartX = 0;
	int32_t StartY = 0;
	if (S != NULL)
	{
		if (S->Sprite == NULL)
			return;
		if (S->Header.TargetImage == NULL)
			return;
		
		//1st restore the previous invalidated area
		
			if (Cmd == eGFX_OBJECT_DRAW_CMD__RESTORE)
			{
				switch (S->Header.DirtyRectangeRestore)
				{
				default:
				case eGFX_DIRTY_RECTANGLE_NO_RESTORE:
					break;

				case eGFX_DIRTY_RECTANGLE_RESTORE_BACKGROUND_COLOR:

					eGFX_DrawSolidRectangle(*(S->Header.TargetImage),
						&S->InvalidatedArea[S->InvalidIdx & 0x01],
						*(S->Header.BackgroundColor));
					break;

				case eGFX_DIRTY_RECTANGLE_RESTORE_BACKGROUND_IMAGE:

					eGFX_BlitRestore(*(S->Header.TargetImage),
						&S->InvalidatedArea[S->InvalidIdx & 0x01],
						*(S->Header.BackgroundImage));

				break;
				}

			}
			else if (Cmd == eGFX_OBJECT_DRAW_CMD__PAINT)
			{
				if (S->Header.Invalidated)
				{

					//Draw the Sprite

					if (S->CenterOnPosition == true)
					{
						StartX = S->Position.X - ((S->Sprite->SizeX + 1) >> 1);
						StartY = S->Position.Y - ((S->Sprite->SizeY + 1) >> 1);
					}
					else
					{
						StartX = S->Position.X;
						StartY = S->Position.Y;
					}

					switch (S->RenderOption)
					{
					default:
						break;

					case eGFX_RENDER_OPTION_BLIT_SOLID:
						eGFX_Blit(*(S->Header.TargetImage),
							StartX,
							StartY,
							S->Sprite);
						break;

					case eGFX_RENDER_OPTION_BLIT_TRANSPARENT:
						eGFX_BlitTransparentPixel(*(S->Header.TargetImage),
							StartX,
							StartY,
							S->Sprite,
							S->TransparentColor);
						break;
					}

					S->InvalidatedArea[S->InvalidIdx].P1.X = StartX;
					S->InvalidatedArea[S->InvalidIdx].P1.Y = StartY;
					S->InvalidatedArea[S->InvalidIdx].P2.X = StartX + S->Sprite->SizeX;
					S->InvalidatedArea[S->InvalidIdx].P2.Y = StartY + S->Sprite->SizeY;
					if (S->DoubleBuffered == true)
					{
						S->InvalidIdx++;
						S->InvalidIdx &= 0x01;
					}
					S->Header.Invalidated = false;
				}
			}
		}
	}


