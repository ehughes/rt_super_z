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

#include "NatureDSP_Signal_matop.h"
#include "NatureDSP_types.h"
#include "common.h"

/*-------------------------------------------------------------------------
  Matrix Multiply
  These functions compute the expression z = 2^lsh * x * y for the matrices 
  x and y. The columnar dimension of x must match the row dimension of y. 
  The resulting matrix has the same number of rows as x and the same number 
  of columns as y.
  Transposing API allows to interpret input yt as transposed matrix y.

  NOTE: lsh factor is not relevant for floating point routines.

  Functions require scratch memory for storing intermediate data. This 
  scratch memory area should be aligned on 8 byte boundary and its size is 
  calculated by dedicated scratch allocation functions.

  Two versions of functions available: regular version (mtx_mpy[t]32x32, 
  mtx_mpy[t]16x16, mtx_mpy[t]8x16, mtx_mpy[t]8x8, mtx[t]_mpyf) with 
  arbitrary arguments and faster version (mtx_mpy[t]32x32_fast, 
  mtx_mpy[t]16x16_fast, mtx_mpy[t]8x16_fast, mtx_mpy[t]8x8_fast, 
  mtx_mpy[t]f_fast) that apply some restrictions

  Precision:
  32x32 32-bit inputs, 32-bit output
  16x16 16-bit inputs, 16-bit output
  8x8   8-bit inputs, 8-bit output
  8x16  8/16-bit inputs, 16-bit output
  f     floating point

  Input:
  x[M*N]      input matrix x, Q7, Q15, Q31 or floating point
  y[N*P]      input matrix y, Q7, Q15, Q31 or floating point
  yt[P*N]     transposed input matrix y. Q31,Q15, Q7 floating point. (for 
              transposing API only)
  M           number of rows in matrix x and z
  N           number of columns in matrix x and number of rows in matrix y
  P           number of columns in matrices y and z
  lsh         left shift applied to the result (applied to the fixed-
              point functions only) 
  Output:
  z[M*P]      output matrix z, Q7, Q15, Q31 or floating point 
  Scratch:
  pScr        size in bytes defined by corresponding scratch allocation 
              functions

  Restrictions:
  For regular routines mpy[t]32x32, mtx_mpy[t]16x16, mtx_mpy[t]8x8, 
  mtx_mpy[t]8x16, mtx_mpy[t]f):
  pScr    aligned on 8-byte boundary
  x,y,z   should not overlap

  For faster routines (mtx_mpy[t]32x32_fast, mtx_mpy[t]16x16_fast, 
  mtx_mpy[t]8x8_fast, mtx_mpy[t]8x16_fast, 
  mtx_mpy[t]f_fast):
  x,y,z        should not overlap
  x,y,z,pScr   aligned on 8-byte boundary
  M,N,P        multiplies of 4
  lsh         should be in range:
              -31...31 for mtx_mpy32x32, mtx_mpy32x32_fast
              -15...15 for mtx_mpy16x16, mtx_mpy16x16_fast, mtx_mpy[t]8x8, 
                       mtx_mpy[t]8x8_fast, mtx_mpy[t]8x16, 
                       mtx_mpy[t]8x16_fast 

-------------------------------------------------------------------------*/
void mtx_mpy16x16_fast ( void* pScr,
                         int16_t* restrict z,
                   const int16_t* restrict x,
                   const int16_t* restrict y,
                   int M, int N, int P, int lsh )
{
    int m, n, p;
    const ae_int16x4 * restrict px;
    const ae_int16x4 * restrict px0;
    const ae_int16x4 * restrict px1;
    const ae_int16x4 * restrict px2;
    const ae_int16x4 * restrict px3;
    const ae_int32   * restrict py0;
    const ae_int32   * restrict py1;
          ae_int32   * restrict pz;
    ae_int16x4 t0, t1;
    ae_int16x4 x0, x1, x2, x3;
    ae_int16x4 y0, y1, y2, y3;
    ae_int32x2 T0, T1, T2, T3;

    (void)pScr;
    NASSERT(N % 4 == 0);
    NASSERT(M % 4 == 0);
    NASSERT(P % 4 == 0);
    NASSERT_ALIGN8(x);
    NASSERT_ALIGN8(y);
    NASSERT_ALIGN8(z);
    if ((M <= 0) || (P <= 0)) return;
    if (N <= 0)
    {
        for (m = 0; m < M * P; m++) z[m] = 0;
        return;
    }

    __Pragma("loop_count min=1");
    for (p = 0; p < (P>>1); p++)
    {
        px = (const ae_int16x4 *)x;
        pz = (      ae_int32   *)z;

        __Pragma("loop_count min=1");
        for (m = 0; m < (M>>2); m++)
        {
            ae_int32x2 a0, a1, a2, a3;
            ae_int64 B0, B1, B2, B3, B4, B5, B6, B7;
            py0 = (const ae_int32   *)(y);
            py1 = (const ae_int32   *)(y+2*P);
            px0 = (const ae_int16x4 *)px;
            px1 = (const ae_int16x4 *)XT_ADDX2(N, (uintptr_t)px0);
            px2 = (const ae_int16x4 *)XT_ADDX2(N, (uintptr_t)px1);
            px3 = (const ae_int16x4 *)XT_ADDX2(N, (uintptr_t)px2);

            B0 = B1 = B2 = B3 = B4 = B5 = B6 = B7 = AE_ZERO64();

            __Pragma("loop_count min=1");
            for (n = 0; n < (N>>2); n++)
            {
                AE_L32_XP(T0, py0, P*sizeof(int16_t));
                AE_L32_XP(T1, py0, P*sizeof(int16_t)*3);
                AE_L32_XP(T2, py1, P*sizeof(int16_t));
                AE_L32_XP(T3, py1, P*sizeof(int16_t)*3);
                y0 = AE_MOVINT16X4_FROMINT32X2(T0);
                y1 = AE_MOVINT16X4_FROMINT32X2(T1);
                y2 = AE_MOVINT16X4_FROMINT32X2(T2);
                y3 = AE_MOVINT16X4_FROMINT32X2(T3);

                t0 = AE_SEL16_7632(y0, y1);
                t1 = AE_SEL16_7632(y2, y3);
                y0 = AE_SEL16_6420(t0, t1);
                y1 = AE_SEL16_7531(t0, t1);

                AE_L16X4_IP(x0, px0, sizeof(ae_int16x4));
                AE_L16X4_IP(x1, px1, sizeof(ae_int16x4));
                AE_L16X4_IP(x2, px2, sizeof(ae_int16x4));
                AE_L16X4_IP(x3, px3, sizeof(ae_int16x4));

                AE_MULAAAAQ16(B0, x0, y0);
                AE_MULAAAAQ16(B1, x0, y1);
                AE_MULAAAAQ16(B2, x1, y0);
                AE_MULAAAAQ16(B3, x1, y1);
                AE_MULAAAAQ16(B4, x2, y0);
                AE_MULAAAAQ16(B5, x2, y1);
                AE_MULAAAAQ16(B6, x3, y0);
                AE_MULAAAAQ16(B7, x3, y1);
            }
            a0 = AE_TRUNCA32X2F64S(B1, B0, lsh + 33);
            a1 = AE_TRUNCA32X2F64S(B3, B2, lsh + 33);
            a2 = AE_TRUNCA32X2F64S(B5, B4, lsh + 33);
            a3 = AE_TRUNCA32X2F64S(B7, B6, lsh + 33);
            t0 = AE_ROUND16X4F32SASYM(a1, a0);
            t1 = AE_ROUND16X4F32SASYM(a3, a2);
            T0 = AE_MOVINT32X2_FROMINT16X4(t0);
            T1 = AE_SEL32_LH(T0, T0);
            T2 = AE_MOVINT32X2_FROMINT16X4(t1);
            T3 = AE_SEL32_LH(T2, T2);
            AE_S32_L_XP(T0, pz, P*sizeof(int16_t));
            AE_S32_L_XP(T1, pz, P*sizeof(int16_t));
            AE_S32_L_XP(T2, pz, P*sizeof(int16_t));
            AE_S32_L_XP(T3, pz, P*sizeof(int16_t));

            px = (const ae_int16x4 *)XT_ADDX8(N, (uintptr_t)px);
        }
        z += 2;
        y += 2;
    }
} /* mtx_mpy16x16_fast() */

size_t mtx_mpy16x16_fast_getScratchSize( int M, int N, int P )
{
    (void)M;(void)N;(void)P;
    return 0;
}
