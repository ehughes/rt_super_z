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
    helper for correlation/convolution
    C code optimized for HiFi4
*/
/* Portable data types. */
#include "NatureDSP_types.h"
/* Common utility and macros declarations. */
#include "common.h"
#include "raw_lxcorr16x16.h"
#include "raw_corr16x16.h"

/*-----------------------------------------------------
raw linear correlation:
Input:
x[N] padded with extra 6 zeroes
y[M] padded with extra 4 zeroes
Output:
r[N+M-1]
restrictions:
M should be >0
x,y aligned on 8-byte boundary
-----------------------------------------------------*/
void raw_lxcorr16x16(int16_t * r, const int16_t * restrict x, const int16_t * restrict y, int N, int M)
{
    const ae_int16x4 *          X0;
    const ae_int16x4 *          X1;
    const ae_int16x4 *          X2;
    const ae_int16x4 *          X3;
    const ae_int16x4 *          S;
    const ae_int16x4 *          Y;
    ae_int16x4 * restrict R;

    int n, m;

    ae_int64   q0, q1, q2, q3;
    ae_int64   q4, q5, q6, q7;

    ae_int16x4 x0, x1, x2, x3;
    ae_int16x4 y0;
    ae_f32x2   t0, t1;

    ae_valign  ar;

    NASSERT_ALIGN(x, 8);
    NASSERT_ALIGN(y, 8);
    NASSERT(M > 0);
    NASSERT(N > 0);

    /*
    * Compute first M-1 entries.
    */
    {
        ae_valign  ay0, ay1, ay2;

        const ae_int16x4 * Y0;
        const ae_int16x4 * Y1;
        const ae_int16x4 * Y2;
        const ae_int16x4 * Y3;

        ae_int16x4 y1, y2, y3;

        ae_int64   z = AE_ZERO64();

        int n_iter = (M - 1);

        ar = AE_ZALIGN64();
        R = (ae_int16x4 *)(r + M - 1 - 1);

        for (n = 0; n < (n_iter&~7); n += 8)
        {
            Y0 = (const ae_int16x4 *)(y + n + 1);
            Y1 = (const ae_int16x4 *)(y + n + 2);
            Y2 = (const ae_int16x4 *)(y + n + 3);
            Y3 = (const ae_int16x4 *)(y + n + 4);

            ay0 = AE_LA64_PP(Y0);
            ay1 = AE_LA64_PP(Y1);
            ay2 = AE_LA64_PP(Y2);

            X0 = (const ae_int16x4 *)x;

            q0 = z; q1 = z; q2 = z; q3 = z;
            q4 = z; q5 = z; q6 = z; q7 = z;

            AE_LA16X4_IP(y0, ay0, Y0);
            AE_LA16X4_IP(y1, ay1, Y1);
            AE_LA16X4_IP(y2, ay2, Y2);
            AE_L16X4_IP(y3, Y3, 8);

            for (m = 0; m < ((M - 1 - n + 3)>>2) - 1; m++ )
            {
                AE_L16X4_IP(x0, X0, +8);

                AE_MULAAAAQ16(q0, x0, y0);
                AE_MULAAAAQ16(q1, x0, y1);
                AE_MULAAAAQ16(q2, x0, y2);
                AE_MULAAAAQ16(q3, x0, y3);
                    
                AE_LA16X4_IP(y0, ay0, Y0);
                AE_LA16X4_IP(y1, ay1, Y1);
                AE_LA16X4_IP(y2, ay2, Y2);
                AE_L16X4_IP(y3, Y3, 8);
                    
                AE_MULAAAAQ16(q4, x0, y0);
                AE_MULAAAAQ16(q5, x0, y1);
                AE_MULAAAAQ16(q6, x0, y2);
                AE_MULAAAAQ16(q7, x0, y3);
            }
            {
                AE_L16X4_IP(x0, X0, +8);

                AE_MULAAAAQ16(q0, x0, y0);
                AE_MULAAAAQ16(q1, x0, y1);
                AE_MULAAAAQ16(q2, x0, y2);
                AE_MULAAAAQ16(q3, x0, y3);
                
                t0 = AE_TRUNCA32X2F64S(q0, q1, 33);
                t1 = AE_TRUNCA32X2F64S(q2, q3, 33);
                AE_SA16X4_RIP(AE_ROUND16X4F32SASYM(t0, t1), ar, R);
                
                t0 = AE_TRUNCA32X2F64S(q4, q5, 33);
                t1 = AE_TRUNCA32X2F64S(q6, q7, 33);
                AE_SA16X4_RIP(AE_ROUND16X4F32SASYM(t0, t1), ar, R);
            }
        }
        if (n_iter & 4)
        {
            Y0 = (const ae_int16x4 *)(y + n + 1);
            Y1 = (const ae_int16x4 *)(y + n + 2);
            Y2 = (const ae_int16x4 *)(y + n + 3);
            Y3 = (const ae_int16x4 *)(y + n + 4);

            ay0 = AE_LA64_PP(Y0);
            ay1 = AE_LA64_PP(Y1);
            ay2 = AE_LA64_PP(Y2);

            X0 = (const ae_int16x4 *)x;

            AE_LA16X4_IP(y0, ay0, Y0);
            AE_LA16X4_IP(y1, ay1, Y1);
            AE_LA16X4_IP(y2, ay2, Y2);
            AE_L16X4_IP(y3, Y3, 8);
            AE_L16X4_IP(x0, X0, +8);
            q0 = AE_MULZAAAAQ16(x0, y0);
            q1 = AE_MULZAAAAQ16(x0, y1);
            q2 = AE_MULZAAAAQ16(x0, y2);
            q3 = AE_MULZAAAAQ16(x0, y3);
            for (m = 0; m < ((M - 1 + 3 - n) >> 2) - 1; m++)
            {
                AE_LA16X4_IP(y0, ay0, Y0);
                AE_LA16X4_IP(y1, ay1, Y1);
                AE_LA16X4_IP(y2, ay2, Y2);
                AE_L16X4_IP(y3, Y3, 8);
                AE_L16X4_IP(x0, X0, +8);
                AE_MULAAAAQ16(q0, x0, y0);
                AE_MULAAAAQ16(q1, x0, y1);
                AE_MULAAAAQ16(q2, x0, y2);
                AE_MULAAAAQ16(q3, x0, y3);
            }
            t0 = AE_TRUNCA32X2F64S(q0, q1, 33);
            t1 = AE_TRUNCA32X2F64S(q2, q3, 33);
            AE_SA16X4_RIP(AE_ROUND16X4F32SASYM(t0, t1), ar, R);
            n += 4;
        }
        AE_SA64NEG_FP(ar, R);

        if (n_iter & 3)
        {
            Y0 = (const ae_int16x4 *)(y + n + 1);
            Y1 = (const ae_int16x4 *)(y + n + 2);
            Y2 = (const ae_int16x4 *)(y + n + 3);

            ay0 = AE_LA64_PP(Y0);
            ay1 = AE_LA64_PP(Y1);
            ay2 = AE_LA64_PP(Y2);

            X0 = (const ae_int16x4 *)x;
          
            AE_LA16X4_IP(y0, ay0, Y0);
            AE_LA16X4_IP(y1, ay1, Y1);
            AE_LA16X4_IP(y2, ay2, Y2);

            AE_L16X4_IP(x0, X0, +8);
            q0 = AE_MULZAAAAQ16(x0, y0);
            q1 = AE_MULZAAAAQ16(x0, y1);
            q2 = AE_MULZAAAAQ16(x0, y2);

            t0 = AE_TRUNCA32X2F64S(q1, q0, 33);
            t1 = AE_TRUNCA32X2F64S(q2, q2, 33);
            x0 = AE_ROUND16X4F32SASYM(t1, t0);

            AE_S16_0_IP(x0, castxcc(ae_int16, R), -2);
            if ((n_iter & 3) > 1) { x0 = AE_SEL16_4321(x0, x0); AE_S16_0_IP(x0, castxcc(ae_int16, R), -2); }
            if ((n_iter & 3) > 2) { x0 = AE_SEL16_4321(x0, x0); AE_S16_0_IP(x0, castxcc(ae_int16, R), -2); }
        }
    }

    /*
    * Compute r[M]...r[MIN(N + M - 1,(N + 1)&~1] entries.
    */

    {
        ae_valign  ax1, ax2, ax3;

        int n_iter = XT_MIN(N, ((N - M + 1) + 3)&~3);
        int m_iter = ((M + 3)&~3);

        /* compute N&~3 correlation results */
        ar = AE_ZALIGN64();

        X0 = (const ae_int16x4 *)x;
        R = (ae_int16x4 *)(r + M - 1);


        for (n = 0; n < (n_iter&~7); n += 8)
        {
            Y = (const ae_int16x4 *)y;
            X1 = (const ae_int16x4 *)((int16_t *) X0 + 1);
            X2 = (const ae_int16x4 *)((int16_t *) X0 + 2);
            X3 = (const ae_int16x4 *)((int16_t *) X0 + 3);

            ax1 = AE_LA64_PP(X1);
            ax2 = AE_LA64_PP(X2);
            ax3 = AE_LA64_PP(X3);

            AE_L16X4_IP(x0, X0, +8);
            AE_LA16X4_IP(x1, ax1, X1);
            AE_LA16X4_IP(x2, ax2, X2);
            AE_LA16X4_IP(x3, ax3, X3);

            q0 = q1 = q2 = q3 = q4 = q5 = q6 = q7 = AE_ZERO64();
            S = X0;
            X0 = (const ae_int16x4 *)XT_ADDX8(1, (uintptr_t)X0);

            __Pragma("loop_count min=1")
            for (m = 0; m < (m_iter >> 2); m++)
            {
                AE_L16X4_IP(y0, Y, 8);

                AE_MULAAAAQ16(q0, y0, x0);
                AE_MULAAAAQ16(q1, y0, x1);
                AE_MULAAAAQ16(q2, y0, x2);
                AE_MULAAAAQ16(q3, y0, x3);
                  
                AE_L16X4_IP(x0, S, +8); 
                AE_LA16X4_IP(x1, ax1, X1);
                AE_LA16X4_IP(x2, ax2, X2);
                AE_LA16X4_IP(x3, ax3, X3);
                  
                AE_MULAAAAQ16(q4, y0, x0);
                AE_MULAAAAQ16(q5, y0, x1);
                AE_MULAAAAQ16(q6, y0, x2);
                AE_MULAAAAQ16(q7, y0, x3);
            }
            t0 = AE_TRUNCA32X2F64S(q0, q1, 33);
            t1 = AE_TRUNCA32X2F64S(q2, q3, 33);
            AE_SA16X4_IP(AE_ROUND16X4F32SASYM(t0, t1), ar, R);

            t0 = AE_TRUNCA32X2F64S(q4, q5, 33);
            t1 = AE_TRUNCA32X2F64S(q6, q7, 33);
            AE_SA16X4_IP(AE_ROUND16X4F32SASYM(t0, t1), ar, R);
        }
        n_iter &= 7;
        if (n_iter & 4)
        {
            Y = (const ae_int16x4 *)y;
            X1 = (const ae_int16x4 *)((int16_t *)X0 + 1);
            X2 = (const ae_int16x4 *)((int16_t *)X0 + 2);
            X3 = (const ae_int16x4 *)((int16_t *)X0 + 3);

            ax1 = AE_LA64_PP(X1);
            ax2 = AE_LA64_PP(X2);
            ax3 = AE_LA64_PP(X3);

            S = X0;
            X0 = (const ae_int16x4 *)XT_ADDX8(1, (uintptr_t)X0);
            q0 = q1 = q2 = q3 = AE_ZERO64();

            __Pragma("loop_count min=1")
            for (m = 0; m < (m_iter >> 2); m++)
            {
                AE_L16X4_IP(x0, S, +8);
                AE_LA16X4_IP(x1, ax1, X1);
                AE_LA16X4_IP(x2, ax2, X2);
                AE_LA16X4_IP(x3, ax3, X3);

                AE_L16X4_IP(y0, Y, 8);
                AE_MULAAAAQ16(q0, y0, x0);
                AE_MULAAAAQ16(q1, y0, x1);
                AE_MULAAAAQ16(q2, y0, x2);
                AE_MULAAAAQ16(q3, y0, x3);
            }
            t0 = AE_TRUNCA32X2F64S(q0, q1, 33);
            t1 = AE_TRUNCA32X2F64S(q2, q3, 33);
            AE_SA16X4_IP(AE_ROUND16X4F32SASYM(t0, t1), ar, R);
        }

        AE_SA64POS_FP(ar, R);
        /* process last 1...3 samples */
        n_iter &= 3;
        if (n_iter)
        {
            Y = (const ae_int16x4 *)y;
            X1 = (const ae_int16x4 *)((int16_t *)X0 + 1);
            X2 = (const ae_int16x4 *)((int16_t *)X0 + 2);

            ax1 = AE_LA64_PP(X1);
            ax2 = AE_LA64_PP(X2);

            q0 = q1 = q2 = AE_ZERO64();
            __Pragma("loop_count min=1")
            for (m = 0; m < (m_iter >> 2); m++)
            {
                AE_L16X4_IP(x0, X0, +8);
                AE_LA16X4_IP(x1, ax1, X1);
                AE_LA16X4_IP(x2, ax2, X2);

                AE_L16X4_IP(y0, Y, 8);
                AE_MULAAAAQ16(q0, y0, x0);
                AE_MULAAAAQ16(q1, y0, x1);
                AE_MULAAAAQ16(q2, y0, x2);
            }
            t0 = AE_TRUNCA32X2F64S(q1, q0, 33);
            t1 = AE_TRUNCA32X2F64S(q2, q2, 33);
            x0 = AE_ROUND16X4F32SASYM(t1, t0);

            AE_S16_0_IP(x0, castxcc(ae_int16, R), +2);
            if (n_iter > 1) { x0 = AE_SEL16_4321(x0, x0); AE_S16_0_IP(x0, castxcc(ae_int16, R), +2); }
            if (n_iter > 2) { x0 = AE_SEL16_4321(x0, x0); AE_S16_0_IP(x0, castxcc(ae_int16, R), +2); }
        }
    }

    /*
    * Compute r[MIN(N + M - 1,(N + 1)&~1]....r[N+M-1] entries.
    */
    {
        ae_valign  ax1, ax2, ax3;

        int offset = ( -((N - (M - 1))) & 3 );
        int n_iter = XT_MAX(0, (M - 1) - offset);
        int m_iter = ((M + 2 - offset) >> 2) - 1;

        X0 = (const ae_int16x4 *)(x + N + offset - (M - 1));
        R = (ae_int16x4 *)(r + N + offset);

        ar = AE_ZALIGN64();

        for (n = 0; n < (n_iter&~7); n += 8, m_iter -= 2)
        {
            X1 = (const ae_int16x4 *)((int16_t *)X0 + 1);
            X2 = (const ae_int16x4 *)((int16_t *)X0 + 2);
            X3 = (const ae_int16x4 *)((int16_t *)X0 + 3);

            ax1 = AE_LA64_PP(X1);
            ax2 = AE_LA64_PP(X2);
            ax3 = AE_LA64_PP(X3);

            AE_L16X4_IP(x0, X0, +8);
            AE_LA16X4_IP(x1, ax1, X1);
            AE_LA16X4_IP(x2, ax2, X2);
            AE_LA16X4_IP(x3, ax3, X3);

            Y = (const ae_int16x4 *)y;
            AE_L16X4_IP(y0, Y, 8);

            q0 = AE_MULZAAAAQ16(y0, x0);
            q1 = AE_MULZAAAAQ16(y0, x1);
            q2 = AE_MULZAAAAQ16(y0, x2);
            q3 = AE_MULZAAAAQ16(y0, x3);
                
            AE_L16X4_IP(x0, X0, +8);
            S = X0;
            AE_LA16X4_IP(x1, ax1, X1);
            AE_LA16X4_IP(x2, ax2, X2);
            AE_LA16X4_IP(x3, ax3, X3);
                
            q4 = AE_MULZAAAAQ16(y0, x0);
            q5 = AE_MULZAAAAQ16(y0, x1);
            q6 = AE_MULZAAAAQ16(y0, x2);
            q7 = AE_MULZAAAAQ16(y0, x3);

            for (m = 0; m < m_iter - 1; m++)
            {
                AE_L16X4_IP(y0, Y, 8);
                AE_MULAAAAQ16(q0, y0, x0);
                AE_MULAAAAQ16(q1, y0, x1);
                AE_MULAAAAQ16(q2, y0, x2);
                AE_MULAAAAQ16(q3, y0, x3);
                    
                AE_L16X4_IP (x0, S, +8);
                AE_LA16X4_IP(x1, ax1, X1);
                AE_LA16X4_IP(x2, ax2, X2);
                AE_LA16X4_IP(x3, ax3, X3);
                    
                AE_MULAAAAQ16(q4, y0, x0);
                AE_MULAAAAQ16(q5, y0, x1);
                AE_MULAAAAQ16(q6, y0, x2);
                AE_MULAAAAQ16(q7, y0, x3);
            }

            AE_L16X4_IP(y0, Y, 8);
            AE_MULAAAAQ16(q0, y0, x0);
            AE_MULAAAAQ16(q1, y0, x1);
            AE_MULAAAAQ16(q2, y0, x2);
            AE_MULAAAAQ16(q3, y0, x3);
            t0 = AE_TRUNCA32X2F64S(q0, q1, 33);
            t1 = AE_TRUNCA32X2F64S(q2, q3, 33);
            AE_SA16X4_IP(AE_ROUND16X4F32SASYM(t0, t1), ar, R);
                
            t0 = AE_TRUNCA32X2F64S(q4, q5, 33);
            t1 = AE_TRUNCA32X2F64S(q6, q7, 33);
            AE_SA16X4_IP(AE_ROUND16X4F32SASYM(t0, t1), ar, R);
        }
        n_iter &= 7;
        if (n_iter & 4)
        {
            X1 = (const ae_int16x4 *)((int16_t *)X0 + 1);
            X2 = (const ae_int16x4 *)((int16_t *)X0 + 2);
            X3 = (const ae_int16x4 *)((int16_t *)X0 + 3);

            ax1 = AE_LA64_PP(X1);
            ax2 = AE_LA64_PP(X2);
            ax3 = AE_LA64_PP(X3);

            AE_L16X4_IP(x0, X0, +8);
            AE_LA16X4_IP(x1, ax1, X1);
            AE_LA16X4_IP(x2, ax2, X2);
            AE_LA16X4_IP(x3, ax3, X3);

            S = X0;

            Y = (const ae_int16x4 *)y;
            AE_L16X4_IP(y0, Y, 8);

            q0 = AE_MULZAAAAQ16(y0, x0);
            q1 = AE_MULZAAAAQ16(y0, x1);
            q2 = AE_MULZAAAAQ16(y0, x2);
            q3 = AE_MULZAAAAQ16(y0, x3);

            for (m = 0; m < m_iter; m++)
            {
                AE_L16X4_IP(x0, S, +8);
                AE_LA16X4_IP(x1, ax1, X1);
                AE_LA16X4_IP(x2, ax2, X2);
                AE_LA16X4_IP(x3, ax3, X3);

                AE_L16X4_IP(y0, Y, 8);
                AE_MULAAAAQ16(q0, y0, x0);
                AE_MULAAAAQ16(q1, y0, x1);
                AE_MULAAAAQ16(q2, y0, x2);
                AE_MULAAAAQ16(q3, y0, x3);
            }
            t0 = AE_TRUNCA32X2F64S(q0, q1, 33);
            t1 = AE_TRUNCA32X2F64S(q2, q3, 33);
            AE_SA16X4_IP(AE_ROUND16X4F32SASYM(t0, t1), ar, R);
            m_iter -= 1;
        }

        AE_SA64POS_FP(ar, R);
        /* process last 1...3 samples */
        n_iter &= 3;
        if (n_iter)
        {
            X1 = (const ae_int16x4 *)((int16_t *)X0 + 1);
            X2 = (const ae_int16x4 *)((int16_t *)X0 + 2);

            ax1 = AE_LA64_PP(X1);
            ax2 = AE_LA64_PP(X2);

            AE_L16X4_IP(x0, X0, +8);
            AE_LA16X4_IP(x1, ax1, X1);
            AE_LA16X4_IP(x2, ax2, X2);

            Y = (const ae_int16x4 *)y;
            AE_L16X4_IP(y0, Y, 8);
            q0 = AE_MULZAAAAQ16(y0, x0);
            q1 = AE_MULZAAAAQ16(y0, x1);
            q2 = AE_MULZAAAAQ16(y0, x2);

            t0 = AE_TRUNCA32X2F64S(q1, q0, 33);
            t1 = AE_TRUNCA32X2F64S(q2, q2, 33);
            x0 = AE_ROUND16X4F32SASYM(t1, t0);

            AE_S16_0_IP(x0, castxcc(ae_int16, R), +2);
            if (n_iter > 1) { x0 = AE_SEL16_4321(x0, x0); AE_S16_0_IP(x0, castxcc(ae_int16, R), +2); }
            if (n_iter > 2) { x0 = AE_SEL16_4321(x0, x0); AE_S16_0_IP(x0, castxcc(ae_int16, R), +2); }
        }
    }
} /* raw_lxcorr16x16() */
