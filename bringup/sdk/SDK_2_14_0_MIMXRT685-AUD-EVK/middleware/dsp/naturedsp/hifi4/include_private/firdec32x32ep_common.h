/* ------------------------------------------------------------------------ */
/*  IntegrIT, Ltd.   www.integrIT.com, info@integrIT.com                    */
/*                                                                          */
/*  NatureDSP Signal Library                                                */
/*                                                                          */
/* This library contains copyrighted materials, trade secrets and other     */
/* proprietary information of IntegrIT, Ltd. This software is licensed for  */
/* use with Tensilica HiFi4 core only and must not be used for any other    */
/* processors and platforms. The license to use these sources was given to  */
/* Cadence, Inc. under Terms and Condition of a Software License Agreement  */
/* between Cadence, Inc. and IntegrIT, Ltd. Any third party must not use    */
/* this code in any form unless such rights are given by Tensilica, Inc.    */
/* Compiling, linking, executing or using this library in any form you      */
/* unconditionally accept these terms.                                      */
/* ------------------------------------------------------------------------ */
/*          Copyright (C) 2014 IntegrIT, Limited.                           */
/*                          All Rights Reserved.                            */
/* ------------------------------------------------------------------------ */  

#ifndef _FIRDEC32X32EP_COMMON_H_
#define _FIRDEC32X32EP_COMMON_H_

/* Portable data types. */
#include "NatureDSP_types.h"
/* Signal Processing Library API. */
#include "NatureDSP_Signal_fir.h"
/* Common utility and macros declarations. */
#include "common.h"

/* Data processing function of a particular decimating filter. Stores a
 * block of input samples to the circular delay line buffer and computes
 * decimating FIR filter's response.
 * Input:
 *   delayLine - circular delay line buffer start address
 *   delayLen  - Delay line buffer length
 *   wrIx    - next position in the buffer to be filled with an input sample
 *   x[N*D]  - input samples
 *   h[]     - decimating FIR filter coefficients, array layout varies
 * Output:
 *   y[N]    - output samples
 *   retval  - updated index of the oldest sample
 * Notes and restrictions:
 *   1. Most of data processing functions feature a single, hard-coded 
 *      decimation factor, so they expect a determined value for parameter D.
 *   2. All pointers with the exception of y[N] must be aligned on an 8-bytes
 *      boundary.
 *   3. N - must be a multiple of 4.
 *   4. M - must be a multiple of 4. */

typedef int (proc_fxn_t)( int32_t * restrict y,
                                  int32_t * delayLine,
                                  int32_t delayLen,
                          const int32_t * restrict x,
                          const int32_t * restrict h,
                          int wrIx, int D, int N, int M );


proc_fxn_t fir32x32ep_D2_proc;
proc_fxn_t fir32x32ep_D3_proc;
proc_fxn_t fir32x32ep_D4_proc;
proc_fxn_t fir32x32ep_DX_proc;


#endif
