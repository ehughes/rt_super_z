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
  NatureDSP Signal Processing Library. Vector matematics
    Rectifier functions
    Code optimized for HiFi4 core
  IntegrIT, 2006-2018
*/
#include "NatureDSP_Signal_math.h"
#include "NatureDSP_types.h"
#include "common.h"
#include "common_fpu.h"

/*-------------------------------------------------------------------------
  Rectifier function
  The functions compute the rectifier linear unit function of input argument. 
  32-bit fixed-point functions accept inputs in Q6.25 and form outputs in 
  Q16.15 format. Parameter K allows to set upper threshold for proper 
  compression of output signal.

  Precision:
  32x32  32-bit inputs, 32-bit output. Accuracy: 2 LSB.
  f      floating point input, floating point output. Accuracy 2 ULP
  Input:
  x[N]   input data, Q6.25 or floating point
  K      threshold, Q16.15 or floating point
  N      length of vectors
  Output:
  y[N]   result, Q16.15 or floating point

  Restriction:
  x,y should not overlap

  Scalar versions:
  ----------------
  return result, Q16.15 or floating point
-------------------------------------------------------------------------*/
#if !HAVE_VFPU && !HAVE_FPU
DISCARD_FUN(void,vec_reluf,(float32_t * y, const float32_t * x, float32_t K, int N))
#elif HAVE_VFPU
void vec_reluf     (float32_t * y, const float32_t * x, float32_t K, int N)
{
    ae_valign aY;
    const xtfloatx2* restrict pX=(const xtfloatx2*)x;
          xtfloatx2* restrict pY=(      xtfloatx2*)y;
    xtfloatx2 zero=XT_CONST_S(0);
    int n;
    if (N<=0) return;
    if (((uintptr_t)pX)&7)
    {
        xtfloat t;
        XT_LSIP(t,castxcc(xtfloat,pX),sizeof(xtfloat));
        t=XT_MIN_S(K,t);
        t=XT_MAX_S(t,zero);
        XT_SSIP(t,castxcc(xtfloat,pY),sizeof(xtfloat));
        N--;
    }
    aY=AE_ZALIGN64();
    for(n=0; n<(N>>1); n++)
    {
        xtfloatx2 t;
        XT_LSX2IP(t,pX,sizeof(xtfloatx2));
        t=XT_MIN_SX2(K,t);
        t=XT_MAX_SX2(t,zero);
        XT_SASX2IP(t,aY,pY);
    }
    AE_SA64POS_FP(aY,pY);
    if(N&1)
    {
        xtfloat t;
        t=XT_LSI((const xtfloat*)pX,0);
        t=XT_MIN_S(K,t);
        t=XT_MAX_S(t,zero);
        XT_SSI(t,(xtfloat*)pY,0);
    }
}
#else
// code for scalar FPU
void vec_reluf     (float32_t * y, const float32_t * x, float32_t K, int N)
{
    const xtfloat* restrict pX=(const xtfloat*)x;
          xtfloat* restrict pY=(      xtfloat*)y;
    xtfloat t,zero=XT_CONST_S(0);
    xtbool bbig,bneg;
    int n;
    for(n=0; n<N; n++)
    {
        XT_LSIP(t,pX,sizeof(float32_t));
        bbig=XT_OLT_S(K,t);
        XT_MOVT_S(t,K,bbig);
        bneg=XT_OLT_S(t,zero);
        XT_MOVT_S(t,zero,bneg);
        XT_SSIP(t,pY,sizeof(float32_t));
    }
}
#endif
