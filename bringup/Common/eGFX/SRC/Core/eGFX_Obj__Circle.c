#include "eGFX_Config.h"
#include "eGFX_DataTypes.h"
#include "eGFX_PixelOps.h"
#include "eGFX_PrimitiveOps.h"
#include "eGFX_StringOps.h"
#include "eGFX_Blit.h"
#include "eGFX_Plane.h"
#include "eGFX_Obj.h"
#include "eGFX_Obj__Circle.h"


void eGFX_InitObject_Circle(eGFX_Obj_Circle * S,
						   uint32_t Radius,
						   eGFX_ImagePlane ** Target
)
{
	if (S != NULL)
	{
		eGFX_EnableObject(S);

		S->Header.Type = eGFX_OBJECT_CIRCLE;
		S->Header.State = eGFX_OBJECT_STATE_ACTIVE;
		S->Header.Invalidated = true;
		S->Header.TargetImage = Target;
		S->Position.X = 0;
		S->Position.Y = 0;
		S->Radius = Radius;
		S->InvalidatedRadius[0] = Radius;
		S->InvalidatedRadius[1] = Radius;
		S->InvalidIdx = 0;

	}

}

void    eGFX_DrawObject_Circle(eGFX_Obj_Circle *S, eGFX_ObjectDrawCommand Cmd)
{

	if (S != NULL)
	{

		if (S->Header.TargetImage == NULL)
			return;
		
		if (*(S->Header.TargetImage) == NULL)
			return;
		//1st restore the previous invalidated area

		if (S->Header.Invalidated)
							{
			if (Cmd == eGFX_OBJECT_DRAW_CMD__RESTORE)
			{
				switch (S->Header.DirtyRectangeRestore)
				{
				default:
				case eGFX_DIRTY_RECTANGLE_NO_RESTORE:
					break;

				case eGFX_DIRTY_RECTANGLE_RESTORE_BACKGROUND_COLOR:

					if (S->Filled == false)
					{
						// Just need to restore the circle.  Faster than doing entire region.
						eGFX_DrawCircle(*(S->Header.TargetImage),
							S->InvalidatedPosition[S->InvalidIdx&0x01].X,
							S->InvalidatedPosition[S->InvalidIdx&0x01].Y,
							S->InvalidatedRadius[S->InvalidIdx & 0x01],
							*(S->Header.BackgroundColor));
					}
					else
					{
						//In this case will we just blit a whole rectangle.  Probably faster than computing the circle

						eGFX_Rect R;
						R.P1.X = S->InvalidatedPosition[S->InvalidIdx&0x01].X - S->InvalidatedRadius[S->InvalidIdx&0x01] - 1;
						R.P1.Y = S->InvalidatedPosition[S->InvalidIdx&0x01].Y - S->InvalidatedRadius[S->InvalidIdx&0x01] - 1;
						R.P2.X = S->InvalidatedPosition[S->InvalidIdx&0x01].X + S->InvalidatedRadius[S->InvalidIdx&0x01] + 1;
						R.P2.Y = S->InvalidatedPosition[S->InvalidIdx&0x01].Y+ S->InvalidatedRadius[S->InvalidIdx&0x01] + 1;

						eGFX_DrawSolidRectangle(*(S->Header.TargetImage),
							&R,
							*(S->Header.BackgroundColor));
					}

					break;

				case eGFX_DIRTY_RECTANGLE_RESTORE_BACKGROUND_IMAGE:

					if (S->Filled == false)
					{
						// Just need to restore the circle.  Faster than doing entire region.
						eGFX_DrawCircleFromBackground(*(S->Header.TargetImage),
							*(S->Header.BackgroundImage),
							S->InvalidatedPosition[S->InvalidIdx&0x01].X,
							S->InvalidatedPosition[S->InvalidIdx&0x01].Y,
							S->InvalidatedRadius[S->InvalidIdx & 0x01]);
					}
					else
					{
						//In this case will we just blit a whole rectangle.  Probably faster than computing the circle
						eGFX_Rect R;

						R.P1.X = S->InvalidatedPosition[S->InvalidIdx&0x01].X - S->InvalidatedRadius[S->InvalidIdx&0x01] - 1;
						R.P1.Y = S->InvalidatedPosition[S->InvalidIdx&0x01].Y- S->InvalidatedRadius[S->InvalidIdx&0x01] - 1;
						R.P2.X = S->InvalidatedPosition[S->InvalidIdx&0x01].X + S->InvalidatedRadius[S->InvalidIdx&0x01] + 1;
						R.P2.Y = S->InvalidatedPosition[S->InvalidIdx&0x01].Y + S->InvalidatedRadius [S->InvalidIdx&0x01]+ 1;

						eGFX_BlitRestore(*(S->Header.TargetImage),
							&R,
							*(S->Header.BackgroundImage));
					}

					break;

				}
			}
			//Draw the Sprite
			else if (Cmd == eGFX_OBJECT_DRAW_CMD__PAINT)
			{

				if (S->Filled == false)
				{
					eGFX_DrawCircle(*(S->Header.TargetImage),
						S->Position.X,
						S->Position.Y,
						S->Radius,
						S->Color);

				}
				else
				{
					eGFX_DrawFilledCircle(*(S->Header.TargetImage),
						S->Position.X,
						S->Position.Y,
						S->Radius,
						S->Color);
				}


				S->InvalidatedRadius[S->InvalidIdx] = S->Radius;
				S->InvalidatedPosition[S->InvalidIdx]  = S->Position;
				S->Header.Invalidated  = false;
				if(S->DoubleBuffered == true)
				{
					S->InvalidIdx++;
					S->InvalidIdx&=0x01;
				}

			}
			}

	}
}


