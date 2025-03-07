/* ------------------------------------------------------------------------ */
/* Copyright (c) 2018 by Cadence Design Systems, Inc. ALL RIGHTS RESERVED.  */
/* These coded instructions, statements, and computer programs (�Cadence    */
/* Libraries�) are the copyrighted works of Cadence Design Systems Inc.	    */
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
    NatureDSP Signal Processing Library. IIR part
    Lattice Block Real IIR, 32x16-bit
    C code optimized for HiFi4
    IntegrIT, 2006-2018
*/

/* Portable data types. */
#include "NatureDSP_types.h"
/* Common utility and macros declarations. */
#include "common.h"
#include "latr32x16_common.h"
/* Order 7 lattice filter data processing function. */
void latr32x16_7_proc( int32_t * restrict r,     // r[N]     [out   ] Q31
                  const int32_t * restrict x,     // x[N]     [in    ] Q31
                        int32_t *          dline, // dline[M] [in/out] Q30
                  const int16_t *          coef,  // coef[M]  [in    ] Q15
                  int16_t scale, int N, int M )   // scale    [in    ] Q15
{
        ae_int32   * restrict R;
  const ae_int32   * restrict X;
        ae_int32x2   *          D;
  const ae_int16x4 *          C;

  ae_f32x2   dl_65, dl_43, dl_21, dl_0z;
  ae_f32x2   dl_54, dl_32, dl_10;
  ae_f16x4   cf_6543, cf_210z;
  ae_f16x4   cf_5432, cf_10z6;
  ae_int16x4 s0, s1, s2, s3, s_g;
  ae_f32x2   p0, p1=AE_ZERO32();
  ae_int32x2 t0, t1;
  ae_f64     q0, q1;
  ae_int64   u0;

  int n;

  NASSERT( r && x && dline && coef );

  NASSERT( IS_ALIGN( dline ) && IS_ALIGN( coef ) );

  NASSERT( M == 7 );
  
  p0 = 0;

  //
  // Setup pointers.
  //

  R = (      ae_int32   *)r;
  X = (const ae_int32   *)x;
  D = (      ae_int32x2   *)dline;
  C = (const ae_int16x4 *)coef;

  //
  // Load the prescaling factor.
  //

  // Q15
  s_g = AE_MOVDA16( (unsigned)scale );

  //
  // Pre-load the reflection coefficients and delay line elements.
  //

  // Q15
  s0 = AE_L16X4_I( C, +0 ); // k6 k5 k4 k3
  s1 = AE_L16X4_I( C, +8 ); // k2 k1 k0  0

  // Make skew coefficient quadruples.
  s2 = AE_SEL16_6543( s0, s1 ); // k5 k4 k3 k2
  s3 = AE_SEL16_6543( s1, s0 ); // k1 k0  0 k6

  cf_6543 = ( s0 );
  cf_210z = ( s1 );
  cf_5432 = ( s2 );
  cf_10z6 = ( s3 );

  // Load delay line elements d6..d0.
  // Q1.30
  dl_65 = AE_L32X2_I( D,  +0 );
  dl_43 = AE_L32X2_I( D,  +8 );
  dl_21 = AE_L32X2_I( D, +16 );
  dl_0z = AE_L32X2_I( D, +24 );

  //
  // Pass the input samples block through the AR lattice. n-th response sample
  // and lattice state update are defined as follows:
  //
  //   p5 = g*x[n] - k6*d6 - k5*d5
  //   p4 = p5 - k4*d4; p3 = p4 - k3*d3; p2 = p3 - k2*d2;
  //   p1 = p2 - k1*d1; p0 = p1 - k0*d0;
  //
  //   d6 = d5 + k5*p5; d5 = d4 + k4*p4; d4 = d3 + k3*p3;
  //   d3 = d2 + k2*p2; d2 = d1 + k1*p1; d1 = d0 + k0*p0;
  //   d0 = p0;
  //
  //   r[n] = p0;
  //
  // The inner loop is fully unrolled.
  //

  __Pragma("loop_count min=1")
  for ( n=0; n<N; n++ )
  {
    //--------------------------------------------------------------------------
    // Load the next input sample and prescale it.

    // Q31
    AE_L32_IP( t0, X, +4 );

    // q0 = g*x[n]
    // Q17.46 <- Q15*Q31
    u0 = AE_MUL32X16_L0( t0, s_g );

    q0 = ( u0 );

    //--------------------------------------------------------------------------
    // Update delay elements d6, d5.

    // d5 d4
    select_f32x2_lh( dl_54, dl_65, dl_43,0 );

    // q0 -= k6*d6 + k5*d5
    // Q17.46 <- Q17.46 + [ Q15*Q1.30 + 1 ] + [ Q15*Q1.30 + 1 ]
    AE_MULSSFD32X16_H3_L2( q0, dl_65, cf_6543 );

    q1 = q0;

    // q1 -= k4*d4
    // Q17.46 <- Q17.46 + [ Q15*Q1.30 + 1 ]
    AE_MULSF32X16_L1( q1, dl_54, cf_6543 );

    // p5 = q0; p4 = q1
    // Q1.30 <- Q17.46 - 16 w/ rounding and saturation
    AE_PKSR32( p0, q0, 0 );
    AE_PKSR32( p0, q1, 0 );

    // d6 = d5 + k5*p5; d5 = d4 + k4*p4
    // Q1.30 <- Q1.30 + [ Q15*Q1.30 - 15 ] w/ rounding and saturation
    AE_MULAFP32X16X2RAS_H( dl_54, p0, cf_5432 );

    dl_65 = dl_54;

    //--------------------------------------------------------------------------
    // Update delay elements d4, d3.

    // d3 d2
    select_f32x2_lh( dl_32, dl_43, dl_21,0 );

    // q0 -= k4*d4 + k3*d3; q1 -= k3*d3 + k2*d2
    // Q17.46 <- Q17.46 + [ Q15*Q1.30 + 1 ] + [ Q15*Q1.30 + 1 ]
    AE_MULSSFD32X16_H1_L0( q0, dl_43, cf_6543 );
    AE_MULSSFD32X16_H1_L0( q1, dl_32, cf_5432 );

    // p3 = q0; p2 = q1
    // Q1.30 <- Q17.46 - 16 w/ rounding and saturation
    AE_PKSR32( p0, q0, 0 );
    AE_PKSR32( p0, q1, 0 );

    // d4 = d3 + k3*p3; d3 = d2 + k2*p2
    // Q1.30 <- Q1.30 + [ Q15*Q1.30 - 15 ] w/ rounding and saturation
    AE_MULAFP32X16X2RAS_L( dl_32, p0, cf_5432 );

    dl_43 = dl_32;

    //--------------------------------------------------------------------------
    // Update delay elements d2, d1.

    // d1 d0
    select_f32x2_lh( dl_10, dl_21, dl_0z,0 );

    // q0 -= k2*d2 + k1*d1; q1 -= k1*d1 + k0*d0
    // Q17.46 <- Q17.46 + [ Q15*Q1.30 + 1 ] + [ Q15*Q1.30 + 1 ]
    AE_MULSSFD32X16_H3_L2( q0, dl_21, cf_210z );
    AE_MULSSFD32X16_H3_L2( q1, dl_10, cf_10z6 );

    // p1 = q0; p0 = q1
    // Q1.30 <- Q17.46 - 16 w/ rounding and saturation
    AE_PKSR32( p0, q0, 0 );
    AE_PKSR32( p0, q1, 0 );

    // d2 = d1 + k1*p1; d1 = d0 + k0*p0
    // Q1.30 <- Q1.30 + [ Q15*Q1.30 - 15 ] w/ rounding and saturation
    AE_MULAFP32X16X2RAS_H( dl_10, p0, cf_10z6 );

    dl_21 = dl_10;

    //--------------------------------------------------------------------------
    // Update delay element d0, format and store response sample.

    // d0 = p0; 0
    select_f32x2_ll( dl_0z, p0, dl_0z,0 );

    // r[n] = q1
    // Q31 <- Q17.46 + 1 - 16 w/ rounding and saturation
    AE_PKSR32( p1, q1, 1 );

    t1 = ( p1 );

    // Q31
    AE_S32_L_IP( t1, R, +4 );
  }

  //
  // Store updated delay line elements.
  //

  // Q1.30
  AE_S32X2_I( dl_65, D,  +0 );
  AE_S32X2_I( dl_43, D,  +8 );
  AE_S32X2_I( dl_21, D, +16 );
  AE_S32X2_I( dl_0z, D, +24 );

} // latr32x16_7_proc()
