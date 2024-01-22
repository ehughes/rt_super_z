#ifdef __cplusplus
extern "C" {
#endif

#ifndef eGFX_OBJ_H
#define eGFX_OBJ_H


	/*
		The object list is just a pointer to the 1st option.  This
		function will just chain objects until the end
	*/

	uint32_t eGFX_AddObject(void * ObjectList, void *ObjectToAdd);

	uint32_t eGFX_GetObjectCount(void * Object, void **LastObject);

	void eGFX_DisableObject(void * Object);

	void eGFX_EnableObject(void * Object);

	uint32_t eGFX_ProcessObjects(void * ObjectList, eGFX_ObjectDrawCommand Cmd);

	uint32_t eGFX_InvalidateObject(void * Object);

	uint32_t eGFX_InvalidateObjectList(void * ObjectList);

#endif

#ifdef __cplusplus
 }
#endif
