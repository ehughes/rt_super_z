#ifndef _EGFX_CONFIG_H_
#define _EGFX_CONFIG_H_



/***
 *       ____               
 *      / ___|___  _ __ ___ 
 *     | |   / _ \| '__/ _ \
 *     | |__| (_) | | |  __/
 *      \____\___/|_|  \___|
 *                          
 */
 

#ifdef _MSC_VER 
	#define eGFX_WEAK                                                
#else
	#define eGFX_WEAK                                          __attribute__((weak))
#endif

#define eGFX_PUT_PIXEL_FLIP_X                                     (0)                                     
#define eGFX_PUT_PIXEL_FLIP_Y                                     (0)
 
/*
        0 to allocate as static 
        1 to allocate on stack  
*/

#define eGFX_STRING_BUF_ALLOC                                     (0)
#define eGFX_MAX_PRINTF_BUF_LENGTH                                (64)
#define eGFX_SPACES_PER_TAB                                       (4)
#define eGFX_MAX_STRING_LEN                                       (64)

/***
 *         _        _   _       _ _         
 *        / \   ___| |_(_)_   _(_) |_ _   _ 
 *       / _ \ / __| __| \ \ / / | __| | | |
 *      / ___ \ (__| |_| |\ V /| | |_| |_| |
 *     /_/   \_\___|\__|_| \_/ |_|\__|\__, |
 *                                    |___/ 
 */


#define eGFX_ACTIVITY_STACK_DEPTH                                                                 16
#define eGFX_MAX_SYSTEM_ACTIVITIES                                                                16


/***
 *      ____  __  __ ____     ___        _               _   
 *     | __ )|  \/  |  _ \   / _ \ _   _| |_ _ __  _   _| |_ 
 *     |  _ \| |\/| | |_) | | | | | | | | __| '_ \| | | | __|
 *     | |_) | |  | |  __/  | |_| | |_| | |_| |_) | |_| | |_ 
 *     |____/|_|  |_|_|      \___/ \__,_|\__| .__/ \__,_|\__|
 *                                          |_|              
 */

#define eGFX_INCLUDE_BITMAP_OUTPUT_SUPPORT  (0)


//Select only one of these!!

//#define WIN32_PRESENT
//#define CHAN_FAT_FS_PRESENT


#endif
