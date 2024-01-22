#include "eGFX_Config.h"
#include "eGFX_DataTypes.h"
#include "eGFX_PixelOps.h"
#include "eGFX_PrimitiveOps.h"
#include "eGFX_StringOps.h"
#include "eGFX_Blit.h"
#include "eGFX_Plane.h"
#include "eGFX_Obj.h"
#include "eGFX_Obj__Sprite.h"
#include "eGFX_Obj__Circle.h"
/*
	The object list is just a pointer to the 1st option.  This
	function will just chain objects until the end
*/

uint32_t eGFX_AddObject(void * ObjectList,void *ObjectToAdd)
{
	void * LastObject = NULL;
	uint32_t NumObjects = 0;

	if (ObjectList != NULL)
	{
		NumObjects = eGFX_GetObjectCount(ObjectList, &LastObject);

		if ((LastObject != NULL) && (ObjectToAdd != NULL))
		{
			((eGFX_ObjectHeader *)LastObject)->Next = ObjectToAdd;

			((eGFX_ObjectHeader *)ObjectToAdd)->Next = NULL;
		}
	}
	
	return NumObjects;
}

uint32_t eGFX_GetObjectCount(void * Object, void **LastObject)
{
	uint32_t Count = 0;
	
	if (Object != NULL)
	{
		Count = 1;
		while (
				(((eGFX_ObjectHeader *)Object)->Next != NULL)
		      )
		{
			Object = ((eGFX_ObjectHeader *)Object)->Next;
			Count++;
		}

		*LastObject = Object;
	}
	
	return Count;
}

void eGFX_DisableObject(void * Object)
{
        if(Object!=NULL)
        {
            ((eGFX_ObjectHeader *)Object)->State = eGFX_OBJECT_STATE_INACTIVE;
        }
}

void eGFX_EnableObject(void * Object)
{
    if(Object!=NULL)
    {
        ((eGFX_ObjectHeader *)Object)->State = eGFX_OBJECT_STATE_ACTIVE;
    }
}

uint32_t eGFX_ProcessObjects(void * ObjectList, eGFX_ObjectDrawCommand Cmd)
{
	uint32_t ProcessedObjects = 0;
	void * ObjectStart = ObjectList;

	//We need 2  steps.  1st loop through and restore the dirty rectangles following by a paint

		ObjectList = ObjectStart;
		while (ObjectList != NULL)
		{
			if (((eGFX_ObjectHeader *)(ObjectList))->State != eGFX_OBJECT_STATE_INACTIVE)
			{
				//object handlers here!
				switch (((eGFX_ObjectHeader *)(ObjectList))->Type)
				{

				case eGFX_OBJECT_SPRITE:
					eGFX_DrawObject_Sprite((eGFX_Obj_Sprite *)ObjectList, Cmd);
					break;

				case eGFX_OBJECT_CIRCLE:
					eGFX_DrawObject_Circle((eGFX_Obj_Circle *)ObjectList, Cmd);
					break;

				default:

					break;
				}
			}

			ObjectList = ((eGFX_ObjectHeader *)(ObjectList))->Next;
			ProcessedObjects++;
		}


	return ProcessedObjects;

}


uint32_t eGFX_InvalidateObject(void * Object)
{
	((eGFX_ObjectHeader *)(Object))->Invalidated = true;

	return 0;
}

uint32_t eGFX_InvalidateObjectList(void * ObjectList)
{
	uint32_t ProcessedObjects = 0;

	while (ObjectList != NULL)
	{
		if (((eGFX_ObjectHeader *)(ObjectList))->State != eGFX_OBJECT_STATE_INACTIVE)
		{
			eGFX_InvalidateObject(ObjectList);
		}

		ObjectList = ((eGFX_ObjectHeader *)(ObjectList))->Next;
		ProcessedObjects++;
	}

	return ProcessedObjects;

}
