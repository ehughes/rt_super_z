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
#ifndef STEREO_BQRIIR32X32_DF1_ND_COMMON_H__
#define STEREO_BQRIIR32X32_DF1_ND_COMMON_H__

/* Portable data types. */
#include "NatureDSP_types.h"
/* Common utility and macros declarations. */
#include "common.h"

/* Instance pointer validation number. */
#define STEREO_BQRIIR32X32_DF1_ND_MAGIC 0xe937a78b

/* Filter instance structure. */
typedef struct tag_stereo_bqriir32x32_df1_nd_t
{
  uint32_t          magic;      // Instance pointer validation number
  int               M;          // Number of sections
  int16_t           gainl;      // Gain shift amount applied after the last section
  int16_t           gainr;      // Gain shift amount applied after the last section
  const int32_t *   coef_gsosl; // Num/den coefs for each section, Q30
  const int32_t *   coef_gsosr; // Num/den coefs for each section, Q30
  int32_t *         statel;     // 4 state elements per section, Q31
  int32_t *         stater;     // 4 state elements per section, Q31
} stereo_bqriir32x32_df1_nd_t, *stereo_bqriir32x32_df1_nd_ptr_t;

#endif /* STEREO_BQRIIR32X32_DF1_ND_COMMON_H__ */
