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

/* DSP Library API */
#include "NatureDSP_Signal_math.h"
/* Common helper macros. */
#include "common_fpu.h"
/* Tables */
#include "inv2pif_tbl.h"
#include "sinf_tbl.h"
/* sNaN/qNaN, single precision. */
#include "nanf_tbl.h"

#if !HAVE_VFPU && !HAVE_FPU
DISCARD_FUN(void,vec_cosinef,( float32_t * restrict y, const float32_t * restrict x, int N ))
#elif HAVE_VFPU
#define sz_f32    (int)sizeof(float32_t)

/*===========================================================================
  Vector matematics:
  vec_cosine            Cosine    
===========================================================================*/
/*-------------------------------------------------------------------------
  Sine/Cosine 
  Fixed-point functions calculate sin(pi*x) or cos(pi*x) for numbers written 
  in Q31 or Q15 format. Return results in the same format. 
  Floating point functions compute sin(x) or cos(x)
  Two versions of functions available: regular version (vec_sine32x32, 
  vec_cosine32x32, , vec_sinef, vec_cosinef) 
  with arbitrary arguments and faster version (vec_sine32x32_fast, 
  vec_cosine32x32_fast) that apply some restrictions.
  NOTE:
  1.  Scalar floating point functions are compatible with standard ANSI C
      routines and set errno and exception flags accordingly
  2.  Floating point functions limit the range of allowable input values:
      [-102940.0, 102940.0] Whenever the input value does not belong to this
      range, the result is set to NaN.

  Precision: 
  32x32  32-bit inputs, 32-bit output. Accuracy: 1700 (7.9e-7)
  f      floating point. Accuracy 2 ULP

  Input:
  x[N]  input data,Q31 or floating point
  N     length of vectors
  Output:
  y[N]  output data,Q31 or floating point

  Restriction:
  Regular versions (vec_sine32x32, vec_cosine32x32, vec_sinef, 
  vec_cosinef):
  x,y - should not overlap

  Faster versions (vec_sine32x32_fast, vec_cosine32x32_fast):
  x,y - should not overlap
  x,y - aligned on 8-byte boundary
  N   - multiple of 2

  Scalar versions:
  ----------------
  return result in Q31 or floating point
-------------------------------------------------------------------------*/
void vec_cosinef   (  float32_t * restrict y,
                const float32_t * restrict x,
                int N)
{
  /*
  float32_t x2,y,ys,yc;
  int sx,n,j,k,sc;
  sx=takesignf(x);
  x=sx?-x:x;
  argument reduction
  k = (int)STDLIB_MATH(floorf)(x*inv4pif.f);
  n = k + 1;
  j = n&~1;

  {
  float32_t dx, t, y = x, jj = (float32_t)j;
  const union ufloat32uint32 c[6] = {
  { 0x3f4a0000 },
  { 0xbb700000 },
  { 0xb6160000 },
  { 0x32080000 },
  { 0x2e060000 },
  { 0xa9b9ee5a } };
  dx = 0.f;
  y -= c[0].f*jj;
  y -= c[1].f*jj;
  y -= c[2].f*jj;
  t = y; y -= c[3].f*jj; t = (t - y); t -= c[3].f*jj; dx = t;
  t = y; y -= c[4].f*jj; t = (t - y); t -= c[4].f*jj; dx = (dx + t);
  t = y; y -= c[5].f*jj; t = (t - y); t -= c[5].f*jj; dx = (dx + t);
  y = (y + dx);
  x = y;
  }
  adjust signs
  sc = ((n + 2) >> 2) & 1;
  compute sine/cosine via minmax polynomial
  x2 = x*x;
  ys = polysinf_tbl[0].f;
  ys = ys*x2 + polysinf_tbl[1].f;
  ys = ys*x2 + polysinf_tbl[2].f;
  ys = ys*x2;
  ys = ys*x + x;
  yc = polycosf_tbl[0].f;
  yc = yc*x2 + polycosf_tbl[1].f;
  yc = yc*x2 + polycosf_tbl[2].f;
  yc = yc*x2 + 1.f;
  select sine/cosine
  y = (n & 2) ? ys : yc;
  apply the sign
  y = changesignf(y, sc);
  return y;
  */

  const xtfloatx2 *          X;
        xtfloatx2 * restrict Y;
  const xtfloatx2 *          S_rd;
        xtfloatx2 * restrict S_wr;
  const xtfloat   *          T;

  ae_valign X_va, Y_va;

  /* Current block index; overall number of blocks; number of values in the current block */
  int blkIx,blkNum,blkLen;
  /* Block size, blkLen <= blkSize */
  const int blkSize = MAX_ALLOCA_SZ/sz_f32;
  /* Allocate a fixed-size scratch area on the stack. */
  float32_t ALIGN(8) scr[blkSize];

  int n;

  if ( N<=0 ) return;

  NASSERT_ALIGN8( scr );

  /*
   * Data are processed in blocks of scratch area size. Further, the algorithm
   * implementation is splitted in order to feed the optimizing compiler with a
   * few loops of managable size.
   */

  blkNum = (N + blkSize-1)/blkSize;

  for ( blkIx=0; blkIx<blkNum; blkIx++ )
  {
    blkLen = XT_MIN(N-blkIx*blkSize,blkSize);

    /*
     * Part I, range reduction. Reference C code:
     *
     *   {
     *     float32_t xn, p, dp, t;
     *     int ji;
     *     float32_t jf;
     *   
     *     // pi/2 splitted into 7-bit chunks.
     *     static const union ufloat32uint32 c[6] = {
     *       { 0x3fca0000 }, { 0xbbf00000 },
     *       { 0xb6960000 }, { 0x32880000 },
     *       { 0x2e860000 }, { 0xaa39ee5a }
     *     };
     *   
     *     for ( n=0; n<blkLen; n++ )
     *     {
     *       xn = fabsf( x[blkIx*blkSize+n] );
     *   
     *       // Determine the pi/2-wide segment the input value belongs to.
     *       jf = roundf( xn*inv2pif.f );
     *   
     *       // Calculate the difference between the segment midpoint and input value.
     *       p = xn;
     *       p -= c[0].f*jf;
     *       p -= c[1].f*jf;
     *       p -= c[2].f*jf;
     *       t = p; p -= c[3].f*jf; t = t - p; t -= c[3].f*jf; dp = t;
     *       t = p; p -= c[4].f*jf; t = t - p; t -= c[4].f*jf; dp += t;
     *       t = p; p -= c[5].f*jf; t = t - p; t -= c[5].f*jf; dp += t;
     *       p += dp;
     *   
     *       scr[n] = p;
     *     }
     *   }
     */

    {
      /* Input value; reducted input value; correction term. */
      xtfloatx2 xn, p, dp;
      /* Auxiliary floating-point vars. */
      xtfloatx2 t, r;
      /* Input value segment number. */
      xtfloatx2 jf;
      /* pi/2 splitted into 7-bit chunks. */
      xtfloatx2 c0, c1, c2, c3, c4, c5;
      /* Scalar auxiliary var.  */
      xtfloat cs;

      /* pi/2 splitted into 7-bit chunks. */
      static const union ufloat32uint32 c[6] = {
        { 0x3fca0000 }, { 0xbbf00000 },
        { 0xb6960000 }, { 0x32880000 },
        { 0x2e860000 }, { 0xaa39ee5a }
      };

      X    = (xtfloatx2*)( (uintptr_t)x + blkIx*blkSize*sz_f32 );
      S_wr = (xtfloatx2*)scr;
      T    = (xtfloat  *)c;

      X_va = AE_LA64_PP( X );

      __Pragma( "loop_count min=1" );
      for ( n=0; n<(blkLen+1)/2; n++ )
      {
        XT_LASX2IP( xn, X_va, X );

        /*
         * Determine the pi/2-wide segment the input value belongs to.
         */
        
        xn = XT_ABS_SX2( xn );
        jf = XT_MUL_SX2( xn, inv2pif.f );
        jf = XT_FIROUND_SX2( jf );

        /*
         * Calculate the difference between the segment midpoint and input value.
         */

        /* For this particular loop, XP address update results in a better schedule if compared with IP. */
        XT_LSXP( cs, T, +1*sz_f32 ); c0 = cs;
        XT_LSXP( cs, T, +1*sz_f32 ); c1 = cs;
        XT_LSXP( cs, T, +1*sz_f32 ); c2 = cs;
        XT_LSXP( cs, T, +1*sz_f32 ); c3 = cs;
        XT_LSXP( cs, T, +1*sz_f32 ); c4 = cs;
        XT_LSXP( cs, T, -5*sz_f32 ); c5 = cs;

        p = xn;
        XT_MSUB_SX2( p, jf, c0 );
        XT_MSUB_SX2( p, jf, c1 );
        XT_MSUB_SX2( p, jf, c2 );

        r = XT_MUL_SX2(jf,c3); t = p; p = XT_SUB_SX2(p,r); t = XT_SUB_SX2(t,p); t = XT_SUB_SX2(t,r); dp = t;
        r = XT_MUL_SX2(jf,c4); t = p; p = XT_SUB_SX2(p,r); t = XT_SUB_SX2(t,p); t = XT_SUB_SX2(t,r); dp = XT_ADD_SX2(t,dp);
        r = XT_MUL_SX2(jf,c5); t = p; p = XT_SUB_SX2(p,r); t = XT_SUB_SX2(t,p); t = XT_SUB_SX2(t,r); dp = XT_ADD_SX2(t,dp);

        p = XT_ADD_SX2( p, dp );

        XT_SSX2IP( p, S_wr, +2*sz_f32 );
      }
    }

    __Pragma( "no_reorder" );

    /*
     * Part II, polynomial approximation. Reference C code:
     *
     *   {
     *     float32_t xn, yn, ys, yc, p, p2;
     *     int ji, sy;
     *   
     *     for ( n=0; n<blkLen; n++ )
     *     {
     *       xn = x[blkIx*blkSize+n];
     *   
     *       // Determine the pi/2-wide segment the input value belongs to.
     *       ji = (int)roundf( fabsf(xn)*inv2pif.f );
     *   
     *       // Adjust the sign.
     *       sy = (((ji+1)>>1)&1);
     *   
     *       //
     *       // Compute sine/cosine approximation via minmax polynomials.
     *       //
     *   
     *       p = scr[n];
     *       p2 = p*p;
     *   
     *       ys = polysinf_tbl[0].f;
     *       ys = polysinf_tbl[1].f + ys*p2;
     *       ys = polysinf_tbl[2].f + ys*p2;
     *       ys = ys*p2;
     *       ys = ys*p + p;
     *   
     *       yc = polycosf_tbl[0].f;
     *       yc = polycosf_tbl[1].f + yc*p2;
     *       yc = polycosf_tbl[2].f + yc*p2;
     *       yc = yc*p2 + 1.f;
     *   
     *       // Select sine or cosine.
     *       yn = ( (ji&1) ? ys : yc );
     *       // Check for input domain.
     *       if ( fabsf(xn) > sinf_maxval.f ) yn = qNaNf.f;
     *       // Apply the sign.
     *       y[blkIx*blkSize+n] = changesignf( yn, sy );
     *     }
     *   }
     */

    {
      /* Input value; reducted input value and its 2nd power; auxiliary var */
      xtfloatx2 xn, p, p2, t;
      /* Input value segment number; output sign; integer reprentation of output value */
      ae_int32x2 ji, sy, yi;
      /* Cosine and sine approximations; output value */
      xtfloatx2 yc, ys, yn;
      /* Polynomial coefficients for sine and cosine. */
      xtfloatx2 cf_s0, cf_s1, cf_s2;
      xtfloatx2 cf_c0, cf_c1, cf_c2;
      /* Cosine/sine selection; out-of-domain flags */
      xtbool2 b_cs, b_ndom;

      X    = (xtfloatx2*)( (uintptr_t)x + blkIx*blkSize*sz_f32 );
      Y    = (xtfloatx2*)( (uintptr_t)y + blkIx*blkSize*sz_f32 );
      S_rd = (xtfloatx2*)scr;

      X_va = AE_LA64_PP( X );
      Y_va = AE_ZALIGN64();

      for ( n=0; n<blkLen/2; n++ )
      {
        XT_LASX2IP( xn, X_va, X );
        
        /* Determine the pi/2-wide segment the input value belongs to. */
        xn = XT_ABS_SX2( xn );
        t = XT_MUL_SX2( xn, inv2pif.f );
        t = XT_FIROUND_SX2( t );
        ji = XT_TRUNC_SX2( t, 0 );

        /*
         * Compute polynomial approximations of sine and cosine for the
         * reducted input value.
         */

        cf_s0 = XT_LSI( (xtfloat*)polysinf_tbl, 0*sz_f32 );
        cf_s1 = XT_LSI( (xtfloat*)polysinf_tbl, 1*sz_f32 );
        cf_s2 = XT_LSI( (xtfloat*)polysinf_tbl, 2*sz_f32 );

        /* Here, the XT_LSX provides better schedule than XT_LSI. */
        cf_c0 = XT_LSX( (xtfloat*)polycosf_tbl, 0*sz_f32 );
        cf_c1 = XT_LSX( (xtfloat*)polycosf_tbl, 1*sz_f32 );
        cf_c2 = XT_LSX( (xtfloat*)polycosf_tbl, 2*sz_f32 );

        XT_LSX2IP( p, S_rd, +2*sz_f32 );
        p2 = XT_MUL_SX2( p, p );

        ys = cf_s0;
        XT_MADD_SX2( cf_s1, ys, p2 ); ys = cf_s1;
        XT_MADD_SX2( cf_s2, ys, p2 ); ys = cf_s2;
        ys = XT_MUL_SX2( ys, p2 );
        t = p; XT_MADD_SX2( t, ys, p ); ys = t;

        yc = cf_c0;
        XT_MADD_SX2( cf_c1, yc, p2 ); yc = cf_c1;
        XT_MADD_SX2( cf_c2, yc, p2 ); yc = cf_c2;

        t = (xtfloatx2)1.0f; XT_MADD_SX2( t, yc, p2 ); yc = t;

        /* Select sine or cosine. */
        b_cs = AE_LT32( AE_SLAI32( ji, 31 ), AE_ZERO32() );
        yn = yc; XT_MOVT_SX2( yn, ys, b_cs );

        /* Adjust the sign. */
        sy = AE_ADD32( ji, AE_MOVI(1) );
        sy = AE_SRLI32( sy, 1 );
        sy = AE_SLLI32( sy, 31 );
        yi = XT_AE_MOVINT32X2_FROMXTFLOATX2( yn );
        yi = AE_XOR32( sy, yi );
        yn = XT_AE_MOVXTFLOATX2_FROMINT32X2( yi );

        /* Set result to NaN for an out-of-domain input value. */
        b_ndom = XT_OLT_SX2( sinf_maxval.f, xn );
        XT_MOVT_SX2( yn, qNaNf.f, b_ndom );

        XT_SASX2IP( yn, Y_va, Y );
      }

      XT_SASX2POSFP( Y_va, Y );

      /* Deliberately process the last input value if it's even-numbered. */
      if ( blkLen & 1 )
      {
        xn = XT_LSI( (xtfloat*)X, 0 );
        
        /* Determine the pi/2-wide segment the input value belongs to. */
        xn = XT_ABS_SX2( xn );
        t = XT_MUL_SX2( xn, inv2pif.f );
        t = XT_FIROUND_SX2( t );
        ji = XT_TRUNC_SX2( t, 0 );

        /*
         * Compute polynomial approximations of sine and cosine for the
         * reducted input value.
         */

        cf_s0 = (xtfloatx2)polysinf_tbl[0].f;
        cf_s1 = (xtfloatx2)polysinf_tbl[1].f;
        cf_s2 = (xtfloatx2)polysinf_tbl[2].f;

        cf_c0 = (xtfloatx2)polycosf_tbl[0].f;
        cf_c1 = (xtfloatx2)polycosf_tbl[1].f;
        cf_c2 = (xtfloatx2)polycosf_tbl[2].f;

        p = XT_LSI( (xtfloat*)S_rd, 0 );
        p2 = XT_MUL_SX2( p, p );

        ys = cf_s0;
        XT_MADD_SX2( cf_s1, ys, p2 ); ys = cf_s1;
        XT_MADD_SX2( cf_s2, ys, p2 ); ys = cf_s2;
        ys = XT_MUL_SX2( ys, p2 );
        t = p; XT_MADD_SX2( t, ys, p ); ys = t;

        yc = cf_c0;
        XT_MADD_SX2( cf_c1, yc, p2 ); yc = cf_c1;
        XT_MADD_SX2( cf_c2, yc, p2 ); yc = cf_c2;
        t = (xtfloatx2)1.0f; XT_MADD_SX2( t, yc, p2 ); yc = t;

        /* Select sine or cosine. */
        b_cs = AE_LT32( AE_SLAI32( ji, 31 ), AE_ZERO32() );
        yn = yc; XT_MOVT_SX2( yn, ys, b_cs );

        /* Adjust the sign. */
        sy = AE_ADD32( ji, AE_MOVI(1) );
        sy = AE_SRLI32( sy, 1 );
        sy = AE_SLLI32( sy, 31 );
        yi = XT_AE_MOVINT32X2_FROMXTFLOATX2( yn );
        yi = AE_XOR32( sy, yi );
        yn = XT_AE_MOVXTFLOATX2_FROMINT32X2( yi );

        /* Set result to NaN for an out-of-domain input value. */
        b_ndom = XT_OLT_SX2( sinf_maxval.f, xn );
        XT_MOVT_SX2( yn, qNaNf.f, b_ndom );

        XT_SSI( yn, (xtfloat*)Y, 0 );
      }
    }

  } /* for ( blkIx=0; blkIx<blkNum; blkIx++ ) */

} /* vec_cosinef() */
#elif HAVE_FPU
#define sz_f32    (int)sizeof(float32_t)

/*===========================================================================
  Vector matematics:
  vec_cosine            Cosine    
===========================================================================*/

/*-------------------------------------------------------------------------
  Sine/Cosine 
  Fixed-point functions calculate sin(pi*x) or cos(pi*x) for numbers written 
  in Q31 or Q15 format. Return results in the same format. 
  Floating point functions compute sin(x) or cos(x)
  Two versions of functions available: regular version (vec_sine32x32, 
  vec_cosine32x32, , vec_sinef, vec_cosinef) 
  with arbitrary arguments and faster version (vec_sine32x32_fast, 
  vec_cosine32x32_fast) that 
  apply some restrictions.
  NOTE:
  1.  Scalar floating point functions are compatible with standard ANSI C
      routines and set errno and exception flags accordingly
  2.  Floating point functions limit the range of allowable input values:
      [-102940.0, 102940.0] Whenever the input value does not belong to this
      range, the result is set to NaN.

  Precision: 
  32x32  32-bit inputs, 32-bit output. Accuracy: 1700 (7.9e-7)
  f      floating point. Accuracy 2 ULP

  Input:
  x[N]  input data,Q31 or floating point
  N     length of vectors
  Output:
  y[N]  output data,Q31 or floating point

  Restriction:
  Regular versions (vec_sine32x32, vec_cosine32x32, vec_sinef, vec_cosinef):
  x,y - should not overlap

  Faster versions (vec_sine32x32_fast, vec_cosine32x32_fast):
  x,y - should not overlap
  x,y - aligned on 8-byte boundary
  N   - multiple of 2

  Scalar versions:
  ----------------
  return result in Q31 or floating point
-------------------------------------------------------------------------*/

// Taken from Fusion
void vec_cosinef( float32_t * restrict y, const float32_t * restrict x, int N )
{
  /*
  * float32_t x2,y,ys,yc;
  * int sx,n,j,k,sc;
  * sx=takesignf(x);
  * x=sx?-x:x;
  * argument reduction
  * k = (int)STDLIB_MATH(floorf)(x*inv4pif.f);
  * n = k + 1;
  * j = n&~1;
  * 
  * {
  * float32_t dx, t, y = x, jj = (float32_t)j;
  * const union ufloat32uint32 c[6] = {
  * { 0x3f4a0000 },
  * { 0xbb700000 },
  * { 0xb6160000 },
  * { 0x32080000 },
  * { 0x2e060000 },
  * { 0xa9b9ee5a } };
  * dx = 0.f;
  * y -= c[0].f*jj;
  * y -= c[1].f*jj;
  * y -= c[2].f*jj;
  * t = y; y -= c[3].f*jj; t = (t - y); t -= c[3].f*jj; dx = t;
  * t = y; y -= c[4].f*jj; t = (t - y); t -= c[4].f*jj; dx = (dx + t);
  * t = y; y -= c[5].f*jj; t = (t - y); t -= c[5].f*jj; dx = (dx + t);
  * y = (y + dx);
  * x = y;
  * }
  * adjust signs
  * sc = ((n + 2) >> 2) & 1;
  * compute sine/cosine via minmax polynomial
  * x2 = x*x;
  * ys = polysinf_tbl[0].f;
  * ys = ys*x2 + polysinf_tbl[1].f;
  * ys = ys*x2 + polysinf_tbl[2].f;
  * ys = ys*x2;
  * ys = ys*x + x;
  * yc = polycosf_tbl[0].f;
  * yc = yc*x2 + polycosf_tbl[1].f;
  * yc = yc*x2 + polycosf_tbl[2].f;
  * yc = yc*x2 + 1.f;
  * select sine/cosine
  * y = (n & 2) ? ys : yc;
  * apply the sign
  * y = changesignf(y, sc);
  * return y;
  */
  const xtfloat *          X;
        int32_t * restrict Y;
  const xtfloat *          S_rd;
        xtfloat * restrict S_wr;
  const xtfloat *          T;
  const int     *          pT;
  const xtfloat * restrict TBLS;
  const xtfloat * restrict TBLC;

  /* Current block index; overall number of blocks; number of values in the current block */
  int blkIx, blkNum, blkLen;
  /* Block size, blkLen <= blkSize */
  const int blkSize = MAX_ALLOCA_SZ / sz_f32;
  /* Allocate a fixed-size scratch area on the stack. */
  float32_t ALIGN(8) scr[blkSize];

  int n;
  if ( N<=0 ) return;

  NASSERT_ALIGN8(scr);
  /*
  * Data are processed in blocks of scratch area size. Further, the algorithm
  * implementation is splitted in order to feed the optimizing compiler with a
  * few loops of managable size.
  */

  blkNum = (N + blkSize - 1) / blkSize;
  for (blkIx = 0; blkIx<blkNum; blkIx++)
  {
    blkLen = XT_MIN(N - blkIx*blkSize, blkSize);
    /*
    * Part I, range reduction. Reference C code:
    *
    *   {
    *     float32_t xn, p, dp, t;
    *     int ji;
    *     float32_t jf;
    *
    *     static const union ufloat32uint32 c[6] = {
    *       { 0x3f4a0000 }, { 0xbb700000 },
    *       { 0xb6160000 }, { 0x32080000 },
    *       { 0x2e060000 }, { 0xa9b9ee5a }
    *     };
    *
    *     for ( n=0; n<blkLen; n++ )
    *     {
    *       xn = fabsf( x[blkIx*blkSize+n] );
    *
    *       // Determine the pi/2-wide segment the input value belongs to.
    *       ji = ( ( (int)floorf( xn*inv4pif.f ) + 1 ) & ~1 );
    *       jf = (float32_t)ji;
    *
    *       // Calculate the difference between the segment midpoint and input value.
    *       p = xn;
    *       p -= c[0].f*jf;
    *       p -= c[1].f*jf;
    *       p -= c[2].f*jf;
    *       t = p; p -= c[3].f*jf; t = t - p; t -= c[3].f*jf; dp = t;
    *       t = p; p -= c[4].f*jf; t = t - p; t -= c[4].f*jf; dp += t;
    *       t = p; p -= c[5].f*jf; t = t - p; t -= c[5].f*jf; dp += t;
    *       p += dp;
    *
    *       scr[n] = p;
    *     }
    *   }
    */
    {
      /* Input value; reducted input value; correction term. */
      xtfloat xn, p, dp;
      /* Auxiliary floating-point vars. */
      xtfloat t, r;
      /* Input value segment number. */
      int32_t ji, i0;
      xtfloat jf;
      /* pi/4 splitted into 7-bit chunks. */
      xtfloat c0, c1, c2, c3, c4, c5;

      static const uint32_t c[6] = {
        0x3f4a0000, 0xbb700000,
        0xb6160000, 0x32080000,
        0x2e060000, 0xa9b9ee5a
      };
      /* 4/pi, 1, ~1 */
      static const uint32_t TAB[3] = { 0x3fa2f983, 0x00000001,
        0xFFFFFFFE
      };
      X = (xtfloat*)((uintptr_t)x + blkIx*blkSize*sz_f32);
      S_wr = (xtfloat*)scr;
      T = (xtfloat  *)c;

	  pT = (int  *)TAB;
      for (n = 0; n<(blkLen); n++)
      {
        XT_LSIP(xn, X, sz_f32);
        /*
        * Determine the pi/2-wide segment the input value belongs to.
        */
        xn = XT_ABS_S(xn);
        XT_LSIP(c0, castxcc(const xtfloat, pT), sz_f32);
        t = XT_MUL_S(xn, c0);
        ji = XT_TRUNC_S(t, 0);
        i0=XT_L32I(pT,0); 
        ji = XT_ADD(ji, i0);
        i0=XT_L32I(pT, sz_f32); pT--;
        ji = XT_AND(ji, i0);
        jf = XT_FLOAT_S(ji, 0);

        /*
        * Calculate the difference between the segment midpoint and input value.
        */

        c0 = XT_LSI( T, 0 * sz_f32);
        c1 = XT_LSI( T, 1 * sz_f32);
        c2 = XT_LSI( T, 2 * sz_f32);
        c3 = XT_LSI( T, 3 * sz_f32);
        c4 = XT_LSI( T, 4 * sz_f32);
        c5 = XT_LSI( T, 5 * sz_f32);

        p = xn;
        XT_MSUB_S(p, jf, c0);
        XT_MSUB_S(p, jf, c1);
        XT_MSUB_S(p, jf, c2);

        r = XT_MUL_S(jf, c3); t = p; p = XT_SUB_S(p, r); t = XT_SUB_S(t, p); t = XT_SUB_S(t, r); dp = t;
        r = XT_MUL_S(jf, c4); t = p; p = XT_SUB_S(p, r); t = XT_SUB_S(t, p); t = XT_SUB_S(t, r); dp = XT_ADD_S(t, dp);
        r = XT_MUL_S(jf, c5); t = p; p = XT_SUB_S(p, r); t = XT_SUB_S(t, p); t = XT_SUB_S(t, r); dp = XT_ADD_S(t, dp);

        p = XT_ADD_S(p, dp);

        XT_SSIP(p, S_wr, sz_f32);
      }
    }
    __Pragma("no_reorder");
    /*
    * Part II, polynomial approximation. Reference C code:
    *
    *   {
    *     float32_t xn, yn, ys, yc, p, p2;
    *     int ji, sy;
    *
    *     for ( n=0; n<blkLen; n++ )
    *     {
    *       xn = x[blkIx*blkSize+n];
    *
    *       // Determine the pi/2-wide segment the input value belongs to.
    *       ji = (int)floorf( fabsf(xn)*inv4pif.f ) + 1;
    *
    *       // Adjust the sign.
    *       sy = (((ji+2)>>2)&1);
    *
    *       //
    *       // Compute sine/cosine approximation via minmax polynomials.
    *       //
    *
    *       p = scr[n];
    *       p2 = p*p;
    *
    *       ys = polysinf_tbl[0].f;
    *       ys = polysinf_tbl[1].f + ys*p2;
    *       ys = polysinf_tbl[2].f + ys*p2;
    *       ys = ys*p2;
    *       ys = ys*p + p;
    *
    *       yc = polycosf_tbl[0].f;
    *       yc = polycosf_tbl[1].f + yc*p2;
    *       yc = polycosf_tbl[2].f + yc*p2;
    *       yc = yc*p2 + 1.f;
    *
    *       // Select sine or cosine.
    *       yn = ( (ji&2) ? ys : yc );
    *       // Check for input domain.
    *       if ( fabsf(xn) > sinf_maxval.f ) yn = qNaNf.f;
    *       // Apply the sign.
    *       y[blkIx*blkSize+n] = changesignf( yn, sy );
    *
    *       //
    *       // Perform additional analysis of input data for Error Handling.
    *       //
    *
    *       #if VEC_COSINEF_ERRH != 0
    *       {
    *         if ( isnan(xn)    || fabsf(xn) > sinf_maxval.f ) i2_edom    = 1;
    *         if ( is_snanf(xn) || fabsf(xn) > sinf_maxval.f ) i2_fe_inv  = 1;
    *       }
    *       #endif
    *     }
    *   }
    */
    {
	  /* max allowable input value; 4/pi */
      static const uint32_t TAB[2] = { 0x47c90e00, 0x3fa2f983        
      };
      /* Input value; reducted input value and its 2nd power; auxiliary var */
      xtfloat xn, p, p2, i0;
      /* Polynomial coefficients for sine and cosine. */
      xtfloat s0, s1, s2, c0, c1, c2;
      /* Cosine/sine selection; out-of-domain flags */
      xtbool b_cs, b_ndom;
      X = (xtfloat*)((uintptr_t)x + blkIx*blkSize*sz_f32);
      Y = (int32_t *)((uintptr_t)y + blkIx*blkSize*sz_f32);
      S_rd = (xtfloat*)scr;
      TBLS = (const xtfloat *)polysinf_tbl;
      TBLC = (const xtfloat *)polycosf_tbl;
      pT = (int  *)TAB;
      __Pragma("loop_count min=1");
      for (n = 0; n<blkLen; n++)
      {
        int32_t sc, n0, tmp;
        xtfloat t0, y0, r0;
        int32_t j0;

        XT_LSIP(xn, X, sz_f32);
        /* Determine the pi/2-wide segment the input value belongs to. */
        xn = XT_ABS_S(xn);
        i0 = ((xtfloat *)pT)[0];
        b_ndom = XT_OLT_S(i0, xn);
        i0 = ((xtfloat *)pT)[1];
        t0 = XT_MUL_S(xn, i0);
        j0 = (int)XT_TRUNC_S(t0, 0);

        n0 = j0 + 1;
        /* adjust signs  */
        tmp = j0 + 3;
        tmp = tmp & 4;
        sc = tmp << 29;
        /*
        * Compute polynomial approximations of sine and cosine for the
        * reducted input value.
        */

        s0 = XT_LSI(TBLS, 0 * sz_f32);
        s1 = XT_LSI(TBLS, 1 * sz_f32);
        s2 = XT_LSI(TBLS, 2 * sz_f32);
        c0 = XT_LSI(TBLC, 0 * sz_f32);
        c1 = XT_LSI(TBLC, 1 * sz_f32);
        c2 = XT_LSI(TBLC, 2 * sz_f32);

        XT_LSIP(p, S_rd, sz_f32);

        p2 = XT_MUL_S(p, p);
        y0 = s1;
        XT_MADD_S(y0, s0, p2); r0 = y0; y0 = s2;
        XT_MADD_S(y0, r0, p2);
        y0 = XT_MUL_S(y0, p2);
        t0 = p;
        XT_MADD_S(t0, y0, p); s0 = t0;

        y0 = c1;
        XT_MADD_S(y0, c0, p2); r0 = y0; y0 = c2;
        XT_MADD_S(y0, r0, p2);
        t0 = XT_CONST_S(1);
        XT_MADD_S(t0, y0, p2); c0 = t0;

        /* Select sine or cosine. */
        n0 = n0 & 2;
        b_cs = AE_INT64_EQ(AE_ZERO64(), AE_MOVINT64_FROMINT32(n0));
        /* Determine the input sign. */
        n0 = XT_RFR(c0); tmp = XT_RFR(s0);
        {
          unsigned int t = n0;
          unsigned int _t = tmp;
          XT_MOVF(t, _t, b_cs); n0 = t;
        }
        n0 = XT_XOR(n0, sc);
        {
          unsigned int t = n0;
          XT_MOVT(t, qNaNf.u, b_ndom); n0 = t;
        }
        /* Set result to NaN for an out-of-domain input value. */
        *Y++=n0;
      }
    }
  }
} /* vec_cosinef() */
#endif
