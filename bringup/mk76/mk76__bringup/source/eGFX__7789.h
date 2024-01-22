#include "eGFX_DataTypes.h"

#ifndef eGFX_DRIVER_7789
#define eGFX_DRIVER_7789


/*
     Required symbols for any driver
*/

#define eGFX_DISPLAY_DRIVER_PRESENT

#define eGFX_DISPLAY_DRIVER_IMAGE_PLANE_TYPE   eGFX_IMAGE_PLANE_16BPP_RGB565

#define eGFX_PHYSICAL_SCREEN_SIZE_X     240
#define eGFX_PHYSICAL_SCREEN_SIZE_Y     320

  /*
      eGFX_InitDriver() should do any hardware related init for the display and
      setup eGFX_BackBuffer
  */

extern void eGFX_InitDriver(eGFX_VSyncCallback_t VS);

  /*
     eGFX_DeInitDriver() is provided in case you want some way of shutting down the graphics

  */

extern void eGFX_DeInitDriver();

/*
      eGFX_Dump() should dump an ImagePlane to the physical screen.
*/

extern void eGFX_Dump(eGFX_ImagePlane *Image);

  /*
      A driver should expose at least one back buffer that is the physical screen size and have
      a matching color space.
  */

 #define eGFX_NUM_BACKBUFFERS 1

extern eGFX_ImagePlane eGFX_BackBuffer[eGFX_NUM_BACKBUFFERS];

extern uint32_t eGFX_GetInactiveBackBuffer();

/*
*  Integer value 0 (off) to 100 (full on)
*
*/

extern void eGFX_SetBacklight(uint8_t BacklightValue);

#endif

