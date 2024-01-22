#include "eGFX_Config.h"
#include "eGFX_DataTypes.h"
#include "eGFX_Animator.h"
#include "eGFX_Obj.h"
#include "math.h"


uint32_t eGFX_AnimatorsAreComplete(eGFX_AnimatorHeader * AnimatorList)
{
        uint32_t RetVal = eGFX_TRUE;
                
		while (AnimatorList != NULL)
		{
			if (eGFX_AnimatorIsComplete(AnimatorList) == eGFX_FALSE)
			{
				RetVal = eGFX_FALSE;
				break;
			}

			AnimatorList = ((eGFX_AnimatorHeader *)(AnimatorList))->Next;
		}
        
        return RetVal;
}


uint32_t eGFX_AnimatorIsComplete(eGFX_AnimatorHeader * A)
{
        uint32_t RetVal = eGFX_FALSE;

        if(((eGFX_AnimatorHeader *)(A))->State == eGFX_ANIMATOR_STATE_COMPLETE)
        {
                RetVal = eGFX_TRUE;
        }

        return RetVal;

}


void eGFX_PauseAnimator(eGFX_AnimatorHeader * A)
{
        ((eGFX_AnimatorHeader *)(A))->State = eGFX_OBJECT_STATE_INACTIVE;
}

void eGFX_StartAnimator(eGFX_AnimatorHeader * A)
{
        (*((eGFX_AnimatorHeader *)(A))).State = eGFX_OBJECT_STATE_ACTIVE;
}


void eGFX_PauseAnimators(eGFX_AnimatorHeader * AnimatorList)
{
	while (AnimatorList != NULL)
	{
		eGFX_PauseAnimator(AnimatorList);
		AnimatorList = ((eGFX_AnimatorHeader *)(AnimatorList))->Next;
	}
}

void eGFX_StartAnimators(eGFX_AnimatorHeader * AnimatorList)
{
		while (AnimatorList != NULL)
		{
			eGFX_StartAnimator(AnimatorList);
			AnimatorList = ((eGFX_AnimatorHeader *)(AnimatorList))->Next;
		}
}

void eGFX_InvalidateAnimatorObject(eGFX_AnimatorHeader * A)
{
	if (A != NULL)
	{
		eGFX_InvalidateObject(((eGFX_AnimatorHeader *)(A))->ObjectToInvalidate);
	}
}

void eGFX_InvalidateAllAnimatorObjects(eGFX_AnimatorHeader * AnimatorList)
{
	while (AnimatorList != NULL)
	{
		eGFX_InvalidateAnimatorObject(AnimatorList);
		AnimatorList = ((eGFX_AnimatorHeader *)(AnimatorList))->Next;
	}
}

uint32_t eGFX_AddAnimator(eGFX_AnimatorHeader * AnimatorList, eGFX_AnimatorHeader *AnimatorToAdd)
{
	eGFX_AnimatorHeader * LastObject = NULL;
	uint32_t NumObjects = 0;

	if (AnimatorList != NULL)
	{
		NumObjects = eGFX_GetAnimatorCount(AnimatorList, &LastObject);

		if ((LastObject != NULL) && (AnimatorToAdd != NULL))
		{
			((eGFX_AnimatorHeader *)LastObject)->Next = AnimatorToAdd;

			((eGFX_AnimatorHeader *)AnimatorToAdd)->Next = NULL;
		}
	}

	return NumObjects;
}

uint32_t eGFX_GetAnimatorCount(eGFX_AnimatorHeader * Animator, eGFX_AnimatorHeader **LastAnimator)
{
	uint32_t Count = 0;

	if (Animator != NULL)
	{
		Count = 1;
		while (
			(((eGFX_ObjectHeader *)Animator)->Next != NULL)
			)
		{
			Animator = ((eGFX_ObjectHeader *)Animator)->Next;
			Count++;
		}

		*LastAnimator = Animator;
	}

	return Count;
}

uint32_t eGFX_ProcessAnimators(eGFX_AnimatorHeader * AnimatorList)
{
	uint32_t ProcessedAnimators = 0;
	eGFX_AnimatorHeader * AnimatorStart = AnimatorList;

	//Process the list
	while (AnimatorList != NULL)
	{
		if ((eGFX_ObjectState)((eGFX_AnimatorHeader *)(AnimatorList))->State == (eGFX_ObjectState)eGFX_OBJECT_STATE_ACTIVE)
		{

			switch (((eGFX_AnimatorHeader *)(AnimatorList))->Type)
			{

				case eGFX_ANIMATOR_POINT:
					eGFX_Process_PointAnimator((eGFX_Point_Animator *)AnimatorList);
					break;

				case eGFX_ANIMATOR_SCALAR:
					eGFX_Process_ScalarAnimator((eGFX_Scalar_Animator *)AnimatorList);
					break;

				default:

					break;
			}
		}



		AnimatorList = ((eGFX_ObjectHeader *)(AnimatorList))->Next;
		ProcessedAnimators++;
	}

	//We need to keep invalidating  all animator objects until they are all done to avoid visual artifacts
	AnimatorList = AnimatorStart;
	if (eGFX_AnimatorsAreComplete(AnimatorList) == eGFX_FALSE)
	{
		eGFX_InvalidateAllAnimatorObjects(AnimatorList);
	}

	return ProcessedAnimators;
}


//Initializes the data struct
void eGFX_Init_PointAnimator(eGFX_Point_Animator *A,
							eGFX_Point Start,
							eGFX_Point End,
							eGFX_Point *Current, //Point this to the thing you want to animate
							uint32_t LastFrame,      //Maximum allowed frames before forced convergence
							eGFX_PointF	 FractionToMove, //The amount of percentage of the distance the point should move each step. Values <=0 or >= 1 are set to 0.5
							eGFX_ObjectHeader * ObjectToInValidate, // Object to Mark when animation is stepped;
							eGFX_AnimatorMode Mode
						)
{
	if (A != NULL)
	{
		A->Header.Type = eGFX_ANIMATOR_POINT;
		A->Header.State = eGFX_ANIMATOR_STATE_INACTIVE;
		A->Start = Start;
		A->End = End;

		A->__Frac.X = (float)A->Start.X;
		A->__Frac.Y = (float)A->Start.Y;
		A->Current = Current;

		if (Current != NULL)
		{
			*(A->Current) = A->Start;
		}

		A->LastFrame = LastFrame;

		A->Header.ObjectToInvalidate = ObjectToInValidate;

		A->Mode = Mode;

		A->CurrentFrame = 0;

		if (A->Mode == eGFX_ANIMATOR_MODE__LINEAR)
		{
			A->FractionToMove.X = ((float)A->End.X - (float)A->Start.X) * FractionToMove.X;
			A->FractionToMove.Y = ((float)A->End.Y - (float)A->Start.Y) * FractionToMove.Y;
		}
		else
		{
			A->FractionToMove = FractionToMove;
		}

		eGFX_InvalidateAnimatorObject((eGFX_AnimatorHeader *)A);
	}
}

void eGFX_Init_ScalarAnimator(eGFX_Scalar_Animator *A,
	int32_t Start,
	int32_t End,
	int32_t *Current, //Point this to the thing you want to animate
	uint32_t LastFrame,      //Maximum allowed frames before forced convergence
	float	 FractionToMove, //The amount of percentage of the distance the point should move each step. Values <=0 or >= 1 are set to 0.5
	eGFX_ObjectHeader * ObjectToInValidate, // Object to Mark when animation is stepped;
	eGFX_AnimatorMode Mode
)
{
	if (A != NULL)
	{
		A->Header.Type = eGFX_ANIMATOR_SCALAR;
		A->Header.State = eGFX_ANIMATOR_STATE_INACTIVE;
		A->Start = Start;
		A->End = End;

		A->__Frac = (float)A->Start;
	
		A->Current = Current;

		if (Current != NULL)
		{
			*(A->Current) = A->Start;
		}

		A->LastFrame = LastFrame;

		A->Header.ObjectToInvalidate = ObjectToInValidate;

		A->Mode = Mode;

		A->CurrentFrame = 0;

		if (A->Mode == eGFX_ANIMATOR_MODE__LINEAR)
		{
			A->FractionToMove = ((float)A->End - (float)A->Start) * FractionToMove;
		}
		else
		{
			A->FractionToMove = FractionToMove;
		}

		eGFX_InvalidateAnimatorObject((eGFX_AnimatorHeader*)A);
	}
}


void eGFX_Process_PointAnimator(eGFX_Point_Animator *A)
{

        if (A == NULL)
                return;

        if (A->Header.State == eGFX_ANIMATOR_STATE_ACTIVE)
        {
                //Move a fraction of a distance towards the target
				
			if (A->Mode == eGFX_ANIMATOR_MODE__FRACTIONAL_BISECT)
			{
				A->__Frac.X += ((float)A->End.X - A->__Frac.X) * A->FractionToMove.X;
				A->__Frac.Y += ((float)A->End.Y - A->__Frac.Y) * A->FractionToMove.Y;

				//See if we have converged or beyond the max number of steps
				if ((A->CurrentFrame >= A->LastFrame) ||
					((fabs((float)(A->End.Y) - (float)(A->__Frac.Y)) <= 0.5)
						&& (fabs((float)(A->End.X) - (float)(A->__Frac.X)) <= 0.5)))
				{
					*(A->Current) = A->End;
					A->Header.State = eGFX_ANIMATOR_STATE_COMPLETE;
				}
				else
				{
					(*(A->Current)).X = (int32_t)A->__Frac.X;
					(*(A->Current)).Y = (int32_t)A->__Frac.Y;
				}

			}
			else
			{

				//See if we have converged or beyond the max number of steps
				if ((A->CurrentFrame >= A->LastFrame))
				{
					*(A->Current) = A->End;
					A->Header.State = eGFX_ANIMATOR_STATE_COMPLETE;
				}
				else
				{
					uint32_t X_Done = eGFX_FALSE;
					uint32_t Y_Done = eGFX_FALSE;

					if ((fabs((float)(A->End.X) - (float)(A->__Frac.X)) >= fabs(A->FractionToMove.X)))
					{
						A->__Frac.X += A->FractionToMove.X;
		
						X_Done = eGFX_FALSE;
					}
					else
					{
						X_Done = eGFX_TRUE;
					}

					if ((fabs((float)(A->End.Y) - (float)(A->__Frac.Y)) >= fabs(A->FractionToMove.Y)))
					{
						A->__Frac.Y += A->FractionToMove.Y;
						Y_Done = eGFX_FALSE;
					}
					else
					{
						Y_Done = eGFX_TRUE;
					}

					if ((X_Done == eGFX_TRUE) && (Y_Done == eGFX_TRUE))
					{
						*(A->Current) = A->End;
						A->Header.State = eGFX_ANIMATOR_STATE_COMPLETE;
					}
					else
					{
						(*(A->Current)).X = (int32_t)A->__Frac.X;
						(*(A->Current)).Y = (int32_t)A->__Frac.Y;
					}

				}

			}


            A->CurrentFrame++;
			
    		//the linked object will get invalidated in the process routine
        }
}


void eGFX_Process_ScalarAnimator(eGFX_Scalar_Animator *A)
{

	if (A == NULL)
		return;

	if (A->Header.State == eGFX_ANIMATOR_STATE_ACTIVE)
	{
		//Move a fraction of a distance towards the target

		if (A->Mode == eGFX_ANIMATOR_MODE__FRACTIONAL_BISECT)
		{
			A->__Frac += ((float)A->End - A->__Frac) * A->FractionToMove;

			//See if we have converged or beyond the max number of steps
			if ((A->CurrentFrame >= A->LastFrame) ||
				((fabs((float)(A->End) - (float)(A->__Frac)) <= 0.5)))
			{
				*(A->Current) = A->End;
				A->Header.State = eGFX_ANIMATOR_STATE_COMPLETE;
			}
			else
			{
				*(A->Current) = (int32_t)A->__Frac;
			}

		}
		else
		{
			//See if we have converged or beyond the max number of steps
			if ((A->CurrentFrame >= A->LastFrame))
			{
				*(A->Current) = A->End;
				A->Header.State = eGFX_ANIMATOR_STATE_COMPLETE;
			}
			else
			{
				uint32_t Done = eGFX_FALSE;

				if ((fabs((float)(A->End) - (float)(A->__Frac)) >= fabs(A->FractionToMove)))
				{
					A->__Frac += A->FractionToMove;
					Done = eGFX_FALSE;
				}
				else
				{
					Done = eGFX_TRUE;
				}

				if (Done == eGFX_TRUE)
				{
					*(A->Current) = A->End;
					A->Header.State = eGFX_ANIMATOR_STATE_COMPLETE;
				}
				else
				{
					*(A->Current) = (int32_t)A->__Frac;
				}
			}
		}

		A->CurrentFrame++;

		//the linked object will get invalidated in the process routine
	}
}
