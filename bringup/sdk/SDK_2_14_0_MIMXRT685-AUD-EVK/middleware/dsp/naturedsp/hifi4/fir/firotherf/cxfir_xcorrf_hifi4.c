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
/*
  NatureDSP Signal Processing Library. FIR part
    complex data circular cross-correlation, complex floating point
    C code optimized for HiFi4
  IntegrIT, 2006-2018
*/

/* Portable data types. */
#include "NatureDSP_types.h"
/* Signal Processing Library API. */
#include "NatureDSP_Signal_fir.h"
/* Common utility and macros declarations. */
#include "common.h"
#include "common_fpu.h"

#if (HAVE_FPU==0 && HAVE_VFPU==0)
DISCARD_FUN(void,cxfir_xcorrf,(   complex_float * restrict r,
               const complex_float * restrict x,
               const complex_float * restrict y,
               int N, int M ))
#elif (HAVE_VFPU)

/*-------------------------------------------------------------------------
  Circular Correlation
  Estimates the circular cross-correlation between vectors x (of length N) 
  and y (of length M)  resulting in vector r of length N. It is a similar 
  to correlation but x is read in opposite direction.

  Precision: 
  16x16     16x16-bit data, 16-bit outputs
  32x16     32x16-bit data, 32-bit outputs
  32x32     32x32-bit data, 32-bit outputs
  32x32ep   the same as above but using 72-bit accumulator for intermediate 
            computations
  f         floating point 


  Input:
  x[N]      input data Q15, Q31 or floating point
  y[M]      input data Q15, Q31 or floating point
  N         length of x
  M         length of y
  Output:
  r[N]      output data, Q15, Q31 or floating point

  Restriction:
  x,y,r     should not overlap
  x,y,r     aligned on an 8-bytes boundary
  N,M       multiples of 4 and >0
-------------------------------------------------------------------------*/
void cxfir_xcorrf(   complex_float * restrict r,
               const complex_float * restrict x,
               const complex_float * restrict y,
               int N, int M )
{
    //
    // Circular cross-correlation algorithm:
    //
    //   r[n] = sum( x[mod(n+m,N)]*y[m] )
    //        m=0..M-1
    //
    //   where n = 0..N-1
    //
    xtfloatx2 A0, A1, A2, A3,
              a0, a1, a2, a3;
    xtfloatx2 X0, X1, X2, X3, Y;
    const xtfloatx2 * restrict pX;
    const xtfloatx2 * restrict pY;
    xtfloatx2 * restrict pR;

    int n, m;
    NASSERT(r);
    NASSERT(x);
    NASSERT(y);
    NASSERT_ALIGN(r,8);
    NASSERT_ALIGN(x,8);
    NASSERT_ALIGN(y,8);
    NASSERT(M>0 && M%4==0);
    NASSERT(N>0 && N%4==0);

    pR=(xtfloatx2*)r;
    /* set circular buffer boundaries */
    WUR_AE_CBEGIN0( (uintptr_t)( x + 0 ) );
    WUR_AE_CEND0  ( (uintptr_t)( x + N ) );

    __Pragma("loop_count min=1")
    for ( n=0; n<N; n+=4)
    {
        pX=(const xtfloatx2 *)(x+n);
        pY=(const xtfloatx2 *)y;
        A0=A1=A2=A3= (xtfloatx2)(0.0f);
        a0=a1=a2=a3= (xtfloatx2)(0.0f);
        /* preload data from x */
        XT_LSX2XC(X0,pX,2*sizeof(float32_t));
        XT_LSX2XC(X1,pX,2*sizeof(float32_t));
        XT_LSX2XC(X2,pX,2*sizeof(float32_t));

        __Pragma("loop_count min=4")
        for ( m=0; m<M; m++ )
        {
            /* load data from x */
            XT_LSX2XC(X3,pX,2*sizeof(float32_t));
            /* load data from y */
            XT_LSX2IP(Y ,pY,2*sizeof(float32_t));
            /* compute correlation of 4 values */
            XT_MADDMUX_S(A0, Y, X0, 4);
            XT_MADDMUX_S(a0, Y, X0, 5);
            XT_MADDMUX_S(A1, Y, X1, 4);
            XT_MADDMUX_S(a1, Y, X1, 5);
            XT_MADDMUX_S(A2, Y, X2, 4);
            XT_MADDMUX_S(a2, Y, X2, 5);
            XT_MADDMUX_S(A3, Y, X3, 4);
            XT_MADDMUX_S(a3, Y, X3, 5);
            /* shift input line for the next iteration */
            X0=X1; X1=X2; X2=X3;
        }
        A0 = A0 + a0;
        A1 = A1 + a1;
        A2 = A2 + a2;
        A3 = A3 + a3;
        /* save computed samples */
        XT_SSX2IP(A0,pR,2*sizeof(float32_t));
        XT_SSX2IP(A1,pR,2*sizeof(float32_t));
        XT_SSX2IP(A2,pR,2*sizeof(float32_t));
        XT_SSX2IP(A3,pR,2*sizeof(float32_t));
    }
} // cxfir_xcorrf()
#else
// for scalar FPU
void cxfir_xcorrf(   complex_float * restrict r,
               const complex_float * restrict x,
               const complex_float * restrict y,
               int N, int M )
{
  /*
  * Circular cross-correlation algorithm:
  *
  *   r[n] = sum( x[mod(n+m,N)]*y[m] )
  *        m=0..M-1
  *
  *   where n = 0..N-1
  */
  xtfloat A0_re, A1_re, A0_im, A1_im;
  xtfloat X0_re, X1_re, X0_im, X1_im,Y0_re, Y0_im;
  const xtfloat * restrict pX;
  const xtfloat* restrict pY;
  xtfloat * restrict pR;
  int n, m;

  NASSERT(r);
  NASSERT(x);
  NASSERT(y);
  NASSERT_ALIGN(r,8);
  NASSERT_ALIGN(x,8);
  NASSERT_ALIGN(y,8);
  NASSERT(M>0 && M%4==0);
  NASSERT(N>0 && N%4==0);
  if (N <= 0 || M <= 0) return;
  pR=(      xtfloat*)r;
  /* set circular buffer boundaries */
  WUR_AE_CBEGIN0( (uintptr_t)( x + 0 ) );
  WUR_AE_CEND0  ( (uintptr_t)( x + N ) );
  for ( n=0; n<N; n+=2,x+=2 )
  {
    A0_re=A1_re=A0_im=A1_im=XT_CONST_S(0);
    pX = (const xtfloat*)(x);
    pY = (const xtfloat*)y;
    XT_LSXC(X0_re, pX, 4);
    XT_LSXC(X0_im, pX, 4);
    XT_LSXC(X1_re, pX, 4);
    XT_LSXC(X1_im, pX, 4);
    __Pragma("loop_count min=1");
    for ( m=0; m<(M); m++ )
    {
      XT_LSIP(Y0_re, pY, 4);
      XT_LSIP(Y0_im, pY, 4);
      XT_MADD_S(A0_re, X0_re, Y0_re);
      XT_MADD_S(A0_re, X0_im, Y0_im);
      XT_MADD_S(A0_im, X0_re, Y0_im);
      XT_MSUB_S(A0_im, X0_im, Y0_re);
      XT_MADD_S(A1_re, X1_re, Y0_re);
      XT_MADD_S(A1_re, X1_im, Y0_im);
      XT_MADD_S(A1_im, X1_re, Y0_im);
      XT_MSUB_S(A1_im, X1_im, Y0_re);
      X0_re = X1_re;
      X0_im = X1_im;
      XT_LSXC(X1_re, pX, 4);
      XT_LSXC(X1_im, pX, 4);
    }
    XT_SSIP(A0_re,pR,4);
    XT_SSIP(A0_im,pR,4);
    XT_SSIP(A1_re,pR,4);
    XT_SSIP(A1_im,pR,4);
  }
} /* cxfir_xcorrf() */

#endif
