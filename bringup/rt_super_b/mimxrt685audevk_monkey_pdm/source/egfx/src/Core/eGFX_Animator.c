
#include "eGFX_Animator.h"
#include "math.h"

bool eGFX_AnimatorsAreComplete(eGFX_AnimatorHeader * AnimatorList)
{
	bool RetVal = true;
                
		while (AnimatorList != NULL)
		{
			if (eGFX_AnimatorIsComplete(AnimatorList) == false)
			{
				RetVal = false;
				break;
			}

			AnimatorList = ((eGFX_AnimatorHeader *)(AnimatorList))->Next;
		}
        
        return RetVal;
}


bool eGFX_AnimatorIsComplete(eGFX_AnimatorHeader * A)
{
	bool RetVal = false;

        if(((eGFX_AnimatorHeader *)(A))->State == eGFX_ANIMATOR_STATE_COMPLETE)
        {
                RetVal = true;
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


static void eGFX_AnimatorMarkComplete(eGFX_AnimatorHeader* A)
{

	A->State = eGFX_ANIMATOR_STATE_COMPLETE;

	if (A->Complete != NULL)
	{
		A->Complete(A->ObjectToNotify);
	}
}


static void eGFX_InvalidateAnimatorObject(eGFX_AnimatorHeader * A)
{
	if (A != NULL)
	{
		if (A->Invalidator != NULL)
		{
			A->Invalidator(A->ObjectToInvalidate);
		}
	}
}

static void eGFX_InvalidateAllAnimatorObjects(eGFX_AnimatorHeader * AnimatorList)
{
	while (AnimatorList != NULL)
	{
		eGFX_InvalidateAnimatorObject(AnimatorList);
		AnimatorList = ((eGFX_AnimatorHeader *)(AnimatorList))->Next;
	}
}

uint32_t eGFX_Animator_AddToList(eGFX_AnimatorHeader * AnimatorList, eGFX_AnimatorHeader *AnimatorToAdd)
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


bool eGFX_Animator_Process(eGFX_AnimatorHeader* A)
{
	bool Processed = true;

	if ((eGFX_ObjectState)((eGFX_AnimatorHeader*)(A))->State == (eGFX_ObjectState)eGFX_OBJECT_STATE_ACTIVE)
	{
		switch (((eGFX_AnimatorHeader*)(A))->Type)
		{

		case eGFX_ANIMATOR_POINT:
			eGFX_Process_PointAnimator((eGFX_Point_Animator*)A);
			break;

		case eGFX_ANIMATOR_SCALAR:
			eGFX_Process_ScalarAnimator((eGFX_Scalar_Animator*)A);
			break;

		default:
			Processed = false;
			break;
		}
	}
	else
	{ 
		Processed = false;
	}

	return Processed;
}

uint32_t eGFX_Animator_ProcessList(eGFX_AnimatorHeader * AnimatorList)
{
	uint32_t ProcessedAnimators = 0;
	eGFX_AnimatorHeader * AnimatorStart = AnimatorList;

	//Process the list
	while (AnimatorList != NULL)
	{
		if (eGFX_Animator_Process(AnimatorList))
		{
			ProcessedAnimators++;
		}
		AnimatorList = ((eGFX_ObjectHeader *)(AnimatorList))->Next;
	}

	//We need to keep invalidating all animator objects until they are all done to avoid visual artifacts
	AnimatorList = AnimatorStart;
	if (eGFX_AnimatorsAreComplete(AnimatorList) == false)
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

				float dist_y = fabs((float)(A->End.Y) - (float)(A->__Frac.Y));
				float dist_x = fabs((float)(A->End.X) - (float)(A->__Frac.X));

				//See if we have converged or beyond the max number of steps
				if (
						(A->CurrentFrame >= A->LastFrame) ||
						(
							(dist_y < 0.5) &&
							(dist_x < 0.5)
						)
				  )
					
				{
					*(A->Current) = A->End;
					
					eGFX_AnimatorMarkComplete(&(A->Header));
				}
				else
				{
					(*(A->Current)).X = (int32_t)A->__Frac.X;
					(*(A->Current)).Y = (int32_t)A->__Frac.Y;
				}

			}
			else
			{

				bool X_Done = false;
				bool Y_Done = false;

				//See if we have converged or beyond the max number of steps
				if ((A->CurrentFrame >= A->LastFrame))
				{
					*(A->Current) = A->End;

					X_Done = true;
					Y_Done = true;
				
				}
				else
				{

					if ((fabs((float)(A->End.X) - (float)(A->__Frac.X)) >= fabs(A->FractionToMove.X)))
					{
						A->__Frac.X += A->FractionToMove.X;

						X_Done = false;
					}
					else
					{
						X_Done = true;
					}

					if ((fabs((float)(A->End.Y) - (float)(A->__Frac.Y)) >= fabs(A->FractionToMove.Y)))
					{
						A->__Frac.Y += A->FractionToMove.Y;
						Y_Done = false;
					}
					else
					{
						Y_Done = true;
					}
				}
				
				if ((X_Done == true) && (Y_Done == true))
				{
					*(A->Current) = A->End;
					eGFX_AnimatorMarkComplete(&(A->Header));
				}
				else
				{
					(*(A->Current)).X = (int32_t)A->__Frac.X;
					(*(A->Current)).Y = (int32_t)A->__Frac.Y;
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
				eGFX_AnimatorMarkComplete(&(A->Header));
			}
			else
			{
				*(A->Current) = (int32_t)A->__Frac;
			}

		}
		else
		{
			bool Done = false;

			//See if we have converged or beyond the max number of steps
			if ((A->CurrentFrame >= A->LastFrame))
			{
				*(A->Current) = A->End;
				Done = true;
			}
			else
			{
				//Check for convergence
				if ((fabs((float)(A->End) - (float)(A->__Frac)) >= fabs(A->FractionToMove)))
				{
					A->__Frac += A->FractionToMove;
					Done = false;
				}
				else
				{
					Done = true;
				}
			}

			if (Done == true)
			{
				*(A->Current) = A->End;
				eGFX_AnimatorMarkComplete(&(A->Header));
			}
			else
			{
				*(A->Current) = (int32_t)A->__Frac;
			}
		}

		A->CurrentFrame++;

		//the linked object will get invalidated in the process routine
	}
}
