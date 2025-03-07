/* ------------------------------------------------------------------------ */
/* Copyright (c) 2018 by Cadence Design Systems, Inc. ALL RIGHTS RESERVED.  */
/* These coded instructions, statements, and computer programs ("Cadence    */
/* Libraries") are the copyrighted works of Cadence Design Systems Inc.	    */
/* Cadence IP is licensed for use with Cadence processor cores only and     */
/* must not be used for any other processors and platforms. Your use of the */
/* Cadence Libraries is subject to the terms of the license agreement you   */
/* have entered into with Cadence Design Systems, or a sublicense granted   */
/* to you by a direct Cadence licensee.                                     */
/* ------------------------------------------------------------------------ */
/*  IntegrIT, Ltd.   www.integrIT.com, info@integrIT.com                    */
/*                                                                          */
/* DSP Library                                                              */
/*                                                                          */
/* This library contains copyrighted materials, trade secrets and other     */
/* proprietary information of IntegrIT, Ltd. This software is licensed for  */
/* use with Cadence processor cores only and must not be used for any other */
/* processors and platforms. The license to use these sources was given to  */
/* Cadence, Inc. under Terms and Condition of a Software License Agreement  */
/* between Cadence, Inc. and IntegrIT, Ltd.                                 */
/* ------------------------------------------------------------------------ */
/*          Copyright (C) 2015-2018 IntegrIT, Limited.                      */
/*                      All Rights Reserved.                                */
/* ------------------------------------------------------------------------ */

#include "common.h"
#include "NatureDSP_types.h"
/* Library API */
#include "NatureDSP_Signal_vector.h"

/*-------------------------------------------------------------------------
  Common Exponent 
  These functions determine the number of redundant sign bits for each value 
  (as if it was loaded in a 32-bit register) and returns the minimum number 
  over the whole vector. This may be useful for a FFT implementation to 
  normalize data.  
  NOTES:
  Faster version of functions make the same task but in a different manner - 
  they compute exponent of maximum absolute value in the array. It allows 
  faster computations but not bitexact results - if minimum value in the 
  array will be -2^n , fast function returns max(0,30-n) while non-fast 
  function returns (31-n).
  Floating point function returns 0-floor(log2(max(abs(x)))). Returned 
  result will be always in range [-129...146]. 
  Special cases
  x       | result
  --------+-------
  0       |    0
  +/-Inf  | -129
  NaN     |    0

  If dimension N<=0 functions return 0

  Precision: 
  32 32-bit inputs 
  16 16-bit inputs 
  f  single precision floating point

  Input:
  x[N]    input data
  N       length of vector
  Returned value:
  minimum exponent

  Restriction:
  Regular versions (vec_bexp16, vec_bexp32, vec_bexpf):
  none
  Faster versions (vec_bexp16_fast, vec_bexp32_fast):
  x   - aligned on 8-byte boundary
  N   - a multiple of 4
-------------------------------------------------------------------------*/
int vec_bexp32 (const int32_t * restrict x, int N)
{
  ae_int32x2                  vA0, vA1, vB0, vB1;
  ae_int64                    vL;
  int                         k, exp, a, b;
  const ae_int32x2 * restrict px = (const ae_int32x2 *)x;
  if (N<=0) return 0;

  exp = 31;

  vB0 = AE_MOVI(0);
  vB1 = AE_MOVI(0);
  {
      if(((uintptr_t)px)&7)
      {
        AE_L32_IP(vA0, castxcc(ae_int32,px),4);
        vB0 = AE_MIN32(vB0, vA0);
        vB1 = AE_MAX32(vB1, vA0);
        N--;
      }
      for (k=0; k<N-1; k+=2)
      {
        AE_L32X2_IP(vA0, px, 8);
        vB0 = AE_MIN32(vB0, vA0);
        vB1 = AE_MAX32(vB1, vA0);
      }
  }

  if (N&1)
  {
    AE_L32_IP(vA0, castxcc(ae_int32,px),4);
    vB0 = AE_MIN32(vB0, vA0);
    vB1 = AE_MAX32(vB1, vA0);
  }

  vA0 = AE_SEL32_LH(vB0, vB0);
  vA1 = AE_SEL32_LH(vB1, vB1);
  vB0 = AE_MIN32(vA0, vB0);
  vB1 = AE_MAX32(vA1, vB1);
  vL = AE_MOVINT64_FROMINT32X2(vB0);
  a = AE_NSA64(vL);
  vL = AE_MOVINT64_FROMINT32X2(vB1);
  b = AE_NSA64(vL);
  exp = XT_MIN(exp, a);
  exp = XT_MIN(exp, b);

  return exp;
}
