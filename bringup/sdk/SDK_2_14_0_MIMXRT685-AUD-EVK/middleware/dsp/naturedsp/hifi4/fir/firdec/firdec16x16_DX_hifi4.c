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
  Decimating block real FIR filter, 16x16-bit
*/

/*-----------------------------------------------------------------------------
* Data processing function of a particular decimating filter. Stores a
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
*   4. M - must be a multiple of 4.
-----------------------------------------------------------------------------*/

/* Portable data types. */
#include "NatureDSP_types.h"
/* Common utility and macros declarations. */
#include "common.h"
#include "firdec16x16_common.h"

int fir16x16_DX_proc(int16_t * restrict y,
                      int16_t * delayLine,
                      int       delayLen,
                const int16_t * restrict x,
                const int16_t * restrict h,
                      int wrIx, int D, int N, int M )
{
    const ae_int16x4  *          D_tmp;
    ae_int16x4        * restrict D_wr;
    const ae_int16x4  *          D_rd0;
    const ae_int16x4  *          D_rd1;
    const ae_int16x4  *          D_rd2;
    const ae_int16x4  *          D_rd3;
    const ae_int16x4  *          X;
    ae_int16          * restrict Y;
    const ae_int16x4  *          C;

    ae_valign D_va1, D_va2, D_va3;
    ae_valign y_align;

    ae_int16x4 d0, d1, d2, d3;
    ae_int16x4 c0;
    ae_int64   q0, q1, q2, q3;
    ae_f32x2   t0, t1;
    ae_int64   z;

    int m, n;

    NASSERT(y && delayLine && x && h && (D > 0) && (N > 0) && (M > 0));
    NASSERT(!(N&7) && !(M&3));
    NASSERT(IS_ALIGN(delayLine) && IS_ALIGN(x) && IS_ALIGN(h));

    //
    // Setup pointers and circular delay line buffer.
    //

    D_wr = (      ae_int16x4 *)(delayLine + wrIx);
    X    = (const ae_int16x4 *)x;
    Y    = (      ae_int16   *)y;

    WUR_AE_CBEGIN0((uintptr_t)(delayLine));
    WUR_AE_CEND0  ((uintptr_t)(delayLine + delayLen));

    z       = AE_ZERO64();
    y_align = AE_ZALIGN64();

    //
    // Break the input signal into 4*D-samples blocks. For each block, store
    // 4*D samples to the delay line buffer, and compute 4 samples of decimated
    // response signal.
    //
    __Pragma("loop_count min=1")
    for (n = 0; n < (N >> 2); n++)
    {
        // Reset the coefficients pointer. Now it looks at the tap corresponding
        // to the oldest sample in the delay line.
        C = (const ae_int16x4*)h;
        __Pragma("loop_count min=5")
        for (m = 0; m<D; m++)
        {
            AE_L16X4_IP(d0, X, +8);
            AE_S16X4_XC(d0, D_wr, +8);
        }

        //
        // Setup 4-way delay line reading pointers, one per an accumulator.
        //
        D_tmp = (const ae_int16x4 *)D_wr;

        D_rd0 = (const ae_int16x4 *)D_wr;
        AE_L16_XC(d0, castxcc(ae_int16, D_tmp), 2 * D);
        D_rd1 = (const ae_int16x4*)D_tmp;
        AE_L16_XC(d0, castxcc(ae_int16, D_tmp), 2 * D);
        D_rd2 = (const ae_int16x4*)D_tmp;
        AE_L16_XC(d0, castxcc(ae_int16, D_tmp), 2 * D);
        D_rd3 = (const ae_int16x4*)D_tmp;

        AE_LA16X4POS_PC(D_va1, D_rd1);
        AE_LA16X4POS_PC(D_va2, D_rd2);
        AE_LA16X4POS_PC(D_va3, D_rd3);

        // Zero the accumulators.
        q0 = z; q1 = z; q2 = z; q3 = z;

        __Pragma("loop_count min=2")
        for (m = 0; m < (M >> 2) + 1; m++)
        {
            AE_L16X4_XC (d0, D_rd0, +8);
            AE_LA16X4_IC(d1, D_va1, D_rd1);
            AE_LA16X4_IC(d2, D_va2, D_rd2);
            AE_LA16X4_IC(d3, D_va3, D_rd3);
            AE_L16X4_IP (c0, C, +8);

            AE_MULAAAAQ16(q0, d0, c0);
            AE_MULAAAAQ16(q1, d1, c0);
            AE_MULAAAAQ16(q2, d2, c0);
            AE_MULAAAAQ16(q3, d3, c0);
        }
        t0 = AE_TRUNCA32X2F64S(q0, q1, 33);
        t1 = AE_TRUNCA32X2F64S(q2, q3, 33);
        AE_SA16X4_IP(AE_ROUND16X4F32SASYM(t0, t1), y_align, castxcc(ae_int16x4, Y));
    }
    AE_SA64POS_FP(y_align, Y);

    return (int)((int16_t *)D_wr - delayLine);
} /* fir16x16_DX_proc() */
