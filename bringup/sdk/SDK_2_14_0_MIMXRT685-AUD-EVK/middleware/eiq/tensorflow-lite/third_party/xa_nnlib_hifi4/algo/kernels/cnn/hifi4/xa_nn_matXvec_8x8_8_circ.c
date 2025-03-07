/*******************************************************************************
* Copyright (c) 2018-2022 Cadence Design Systems, Inc.
*
* Permission is hereby granted, free of charge, to any person obtaining
* a copy of this software and associated documentation files (the
* "Software"), to use this Software with Cadence processor cores only and
* not with any other processors and platforms, subject to
* the following conditions:
*
* The above copyright notice and this permission notice shall be included
* in all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

******************************************************************************/
#include <stddef.h>
#include "xa_nnlib_common.h"

#define ZERO64   AE_MOVINT64_FROMINT32X2(AE_MOVDA32(0))

#if defined(CUST_UNROLL) && (CUST_UNROLL != 0)
#define UNROLL_D CUST_UNROLL
#else
#define UNROLL_D  4 /// Optimal unroll
#endif

#define SETUP_ROW_D(N) \
  ae_int64 accu1_ ##N, accu2_ ##N;\
  WORD8 *p_mat1_ ##N = p_mat; \
  AE_ADDCIRC16X4_XC((ae_int16x4 *)p_mat1_ ##N, (row+N) * row_offset * sizeof(WORD8)); \
  accu1_ ##N = ZERO64; \
  accu2_ ##N = ZERO64;

#if XCHAL_HAVE_HIFI1
#define KERNEL_ROW_D(N) \
{\
  ae_int16x4 temp_in1; \
  temp_in1 = AE_L8X4S_I(p_mat1_ ##N, 0); \
  AE_ADDCIRC16X4_XC((ae_int16x4 *)p_mat1_ ##N, 4 * sizeof(WORD8)); \
  AE_MULAAAAQ16(accu1_ ##N, temp_src1, temp_in1);\
  AE_MULAAAAQ16(accu2_ ##N, temp_src2, temp_in1); \
}
#define KERNEL_ROW_D_I(N) \
{\
  ae_int16x4 temp_in1; \
  temp_in1 = AE_L8X4S_I(p_mat1_ ##N, 0); \
  AE_ADDCIRC16X4_XC((ae_int16x4 *)p_mat1_ ##N, 4 * sizeof(WORD8)); \
  AE_L8X4S_IP(temp_src1, p_src1, 4); \
  AE_L8X4S_IP(temp_src2, p_src2, 4); \
  AE_MULAAAAQ16(accu1_ ##N, temp_src1, temp_in1);\
  AE_MULAAAAQ16(accu2_ ##N, temp_src2, temp_in1); \
}
#define STORE_ROW_D(N) \
  ae_int64 temp1_ ##N ,temp2_ ##N;  \
  ae_int16x4 _ae_int16x4_temp_var_ ##N, _ae_int16x4_temp1_var_ ##N; \
  _ae_int16x4_temp_var_ ##N = AE_L8S_I(&p_bias[vec], 0); \
  temp1_ ##N = AE_SRAI64(AE_MOVINT64_FROMINT16X4(_ae_int16x4_temp_var_ ##N), 48); \
  _ae_int16x4_temp1_var_ ##N = AE_L8S_I(&p_bias[vec+1], 0); \
  temp2_ ##N = AE_SRAI64(AE_MOVINT64_FROMINT16X4(_ae_int16x4_temp1_var_ ##N), 48); \
  temp1_ ##N = AE_SLAA64S(temp1_ ##N , bias_shift);\
  temp2_ ##N = AE_SLAA64S(temp2_ ##N , bias_shift);\
  accu1_ ##N = AE_ADD64(accu1_ ##N , temp1_ ##N);\
  accu2_ ##N = AE_ADD64(accu2_ ##N , temp2_ ##N);\
  accu1_ ##N = AE_SLAA64S(accu1_ ##N , acc_shift);\
  accu2_ ##N = AE_SLAA64S(accu2_ ##N , acc_shift);\
  ae_f32x2  _ae_f32x2_temp_var_ ##N = AE_ROUND32F64SSYM(accu1_ ##N); \
  ae_f32x2  _ae_f32x2_temp1_var_ ##N = AE_ROUND32F64SSYM(accu2_ ##N); \
  _ae_int16x4_temp_var_ ##N = AE_SAT16X4( _ae_f32x2_temp_var_ ##N, _ae_f32x2_temp_var_ ##N ); \
  _ae_int16x4_temp1_var_ ##N = AE_SAT16X4( _ae_f32x2_temp1_var_ ##N, _ae_f32x2_temp1_var_ ##N ); \
  _ae_int16x4_temp_var_ ##N = AE_SAT8S( _ae_int16x4_temp_var_ ##N ); \
  _ae_int16x4_temp1_var_ ##N = AE_SAT8S( _ae_int16x4_temp1_var_ ##N ); \
  AE_S8_0_I_HIFI1(_ae_int16x4_temp_var_ ##N, (WORD8 *)p_dst1 + (row+N) * out_row_offset, 0 ); \
  AE_S8_0_I_HIFI1(_ae_int16x4_temp1_var_ ##N, (WORD8 *)p_dst2 + (row+N) * out_row_offset, 0 );
#else
#define KERNEL_ROW_D(N) \
{\
  ae_int16x4 temp_in1; \
  temp_in1 = AE_L8X4F_I(p_mat1_ ##N, 0); \
  AE_ADDCIRC16X4_XC((ae_int16x4 *)p_mat1_ ##N, 4 * sizeof(WORD8)); \
  AE_MULAAAAQ16(accu1_ ##N, temp_src1, temp_in1);\
  AE_MULAAAAQ16(accu2_ ##N, temp_src2, temp_in1); \
}
#define KERNEL_ROW_D_I(N) \
{\
  ae_int16x4 temp_in1; \
  temp_in1 = AE_L8X4F_I(p_mat1_ ##N, 0); \
  AE_ADDCIRC16X4_XC((ae_int16x4 *)p_mat1_ ##N, 4 * sizeof(WORD8)); \
  AE_L8X4F_IP(temp_src1, p_src1, 4); \
  AE_L8X4F_IP(temp_src2, p_src2, 4); \
  AE_MULAAAAQ16(accu1_ ##N, temp_src1, temp_in1);\
  AE_MULAAAAQ16(accu2_ ##N, temp_src2, temp_in1); \
}
#define STORE_ROW_D(N) \
  accu1_ ##N = AE_SLAA64S(accu1_ ##N , -16);\
  accu2_ ##N = AE_SLAA64S(accu2_ ##N , -16);\
  ae_int64 temp1_ ##N ,temp2_ ##N;            \
  temp1_ ##N = AE_MOVINT64_FROMINT16X4(AE_MOVDA16(p_bias[vec]));            \
  temp2_ ##N = AE_MOVINT64_FROMINT16X4(AE_MOVDA16(p_bias[vec+1]));            \
  temp1_ ##N = AE_SLAA64S(temp1_ ##N , 8); \
  temp1_ ##N = AE_SLAA64S(temp1_ ##N , -56); \
  temp2_ ##N = AE_SLAA64S(temp2_ ##N , 8); \
  temp2_ ##N = AE_SLAA64S(temp2_ ##N , -56); \
  temp1_ ##N = AE_SLAA64S(temp1_ ##N , bias_shift);\
  temp2_ ##N = AE_SLAA64S(temp2_ ##N , bias_shift);\
  accu1_ ##N = AE_ADD64(accu1_ ##N , temp1_ ##N);\
  accu2_ ##N = AE_ADD64(accu2_ ##N , temp2_ ##N);\
  accu1_ ##N = AE_SLAA64S(accu1_ ##N , acc_shift);\
  accu2_ ##N = AE_SLAA64S(accu2_ ##N , acc_shift);\
  ae_int32 temp_var_ ##N = AE_MOVINT16_FROMINT32(AE_SLAA32S(AE_SLAA32S(AE_ROUND32F64SSYM(accu1_ ##N),24),-24)); \
  ae_int32 temp1_var_ ##N = AE_MOVINT16_FROMINT32(AE_SLAA32S(AE_SLAA32S(AE_ROUND32F64SSYM(accu2_ ##N),24),-24)); \
  (*((WORD8 *) p_dst1 + (row+N) * out_row_offset )) = (*((UWORD32 *)&temp_var_ ##N)); \
  (*((WORD8 *) p_dst2 + (row+N) * out_row_offset )) = (*((UWORD32 *)&temp1_var_ ##N));
#endif

#if (UNROLL_D == 1)
#define SETUP_D SETUP_ROW_D(0)
#define KERNEL_D KERNEL_ROW_D_I(0)
#define STORE_D STORE_ROW_D(0)

#elif (UNROLL_D == 2)
#define SETUP_D  SETUP_ROW_D(0)  SETUP_ROW_D(1)
#define KERNEL_D KERNEL_ROW_D_I(0) KERNEL_ROW_D(1)
#define STORE_D  STORE_ROW_D(0)  STORE_ROW_D(1)

#elif (UNROLL_D == 4)
#define SETUP_D  SETUP_ROW_D(0)  SETUP_ROW_D(1)  SETUP_ROW_D(2)  SETUP_ROW_D(3)
#define KERNEL_D KERNEL_ROW_D_I(0) KERNEL_ROW_D(1) KERNEL_ROW_D(2) KERNEL_ROW_D(3)
#define STORE_D  STORE_ROW_D(0)  STORE_ROW_D(1)  STORE_ROW_D(2)  STORE_ROW_D(3)
#elif (UNROLL_D == 8)
#define SETUP_D   SETUP_ROW_D(0)  SETUP_ROW_D(1)  SETUP_ROW_D(2)  SETUP_ROW_D(3)  SETUP_ROW_D(4)  SETUP_ROW_D(5)  SETUP_ROW_D(6)  SETUP_ROW_D(7)
#define KERNEL_D KERNEL_ROW_D_I(0) KERNEL_ROW_D(1) KERNEL_ROW_D(2) KERNEL_ROW_D(3) KERNEL_ROW_D(4) KERNEL_ROW_D(5) KERNEL_ROW_D(6) KERNEL_ROW_D(7)
#define STORE_D   STORE_ROW_D(0)  STORE_ROW_D(1)  STORE_ROW_D(2)  STORE_ROW_D(3)  STORE_ROW_D(4)  STORE_ROW_D(5)  STORE_ROW_D(6)  STORE_ROW_D(7)

#endif

#if defined(CUST_UNROLL) && (CUST_UNROLL != 0)
#define UNROLL_S CUST_UNROLL
#else
#define UNROLL_S  8 /// Optimal unroll
#endif

#define SETUP_ROW_S(N) \
  ae_int64 accu1_ ##N;\
  WORD8 *p_mat1_ ##N = p_mat; \
  AE_ADDCIRC16X4_XC((ae_int16x4 *)p_mat1_ ##N, (row+N) * row_offset * sizeof(WORD8)); \
  accu1_ ##N = ZERO64;

#if XCHAL_HAVE_HIFI1
#define KERNEL_ROW_S(N) \
{ \
  ae_int16x4 temp_in1; \
  temp_in1 = AE_L8X4S_I(p_mat1_ ##N, 0); \
  AE_ADDCIRC16X4_XC((ae_int16x4 *)p_mat1_ ##N, 4 * sizeof(WORD8)); \
  AE_MULAAAAQ16(accu1_ ##N, temp_src1, temp_in1);\
}
#define KERNEL_ROW_S_I(N) \
{ \
  ae_int16x4 temp_in1; \
  temp_in1 = AE_L8X4S_I(p_mat1_ ##N, 0); \
  AE_ADDCIRC16X4_XC((ae_int16x4 *)p_mat1_ ##N, 4 * sizeof(WORD8)); \
  AE_L8X4S_IP(temp_src1, p_src1, 4); \
  AE_MULAAAAQ16(accu1_ ##N, temp_src1, temp_in1);\
}
#define STORE_ROW_S(N) \
  ae_int64 temp1_ ##N; \
  ae_int16x4 _ae_int16x4_temp_var_ ##N; \
  _ae_int16x4_temp_var_ ##N = AE_L8S_I(&p_bias[vec], 0); \
  temp1_ ##N = AE_SRAI64(AE_MOVINT64_FROMINT16X4(_ae_int16x4_temp_var_ ##N), 48); \
  temp1_ ##N = AE_SLAA64S(temp1_ ##N , bias_shift);\
  accu1_ ##N = AE_ADD64(accu1_ ##N , temp1_ ##N);\
  accu1_ ##N = AE_SLAA64S(accu1_ ##N , acc_shift);\
  ae_f32x2  _ae_f32x2_temp_var_ ##N = AE_ROUND32F64SSYM(accu1_ ##N); \
  _ae_int16x4_temp_var_ ##N = AE_SAT16X4( _ae_f32x2_temp_var_ ##N, _ae_f32x2_temp_var_ ##N ); \
  _ae_int16x4_temp_var_ ##N = AE_SAT8S( _ae_int16x4_temp_var_ ##N ); \
  AE_S8_0_I_HIFI1(_ae_int16x4_temp_var_ ##N, (WORD8 *) p_dst1 + (row+N) * out_row_offset, 0 );
#else
#define KERNEL_ROW_S(N) \
{ \
  ae_int16x4 temp_in1; \
  temp_in1 = AE_L8X4F_I(p_mat1_ ##N, 0); \
  AE_ADDCIRC16X4_XC((ae_int16x4 *)p_mat1_ ##N, 4 * sizeof(WORD8)); \
  AE_MULAAAAQ16(accu1_ ##N, temp_src1, temp_in1);\
}
#define KERNEL_ROW_S_I(N) \
{ \
  ae_int16x4 temp_in1; \
  temp_in1 = AE_L8X4F_I(p_mat1_ ##N, 0); \
  AE_ADDCIRC16X4_XC((ae_int16x4 *)p_mat1_ ##N, 4 * sizeof(WORD8)); \
  AE_L8X4F_IP(temp_src1, p_src1, 4); \
  AE_MULAAAAQ16(accu1_ ##N, temp_src1, temp_in1);\
}
#define STORE_ROW_S(N) \
  accu1_ ##N = AE_SLAA64S(accu1_ ##N , -16);\
  ae_int64 temp1_ ##N; \
  temp1_ ##N = AE_MOVINT64_FROMINT16X4(AE_MOVDA16(p_bias[vec]));            \
  temp1_ ##N = AE_SLAA64S(temp1_ ##N , 8); \
  temp1_ ##N = AE_SLAA64S(temp1_ ##N , -56); \
  temp1_ ##N = AE_SLAA64S(temp1_ ##N , bias_shift);\
  accu1_ ##N = AE_ADD64(accu1_ ##N , temp1_ ##N);\
  accu1_ ##N = AE_SLAA64S(accu1_ ##N , acc_shift);\
  ae_int32 temp_var_ ##N = AE_MOVINT16_FROMINT32(AE_SLAA32S(AE_SLAA32S(AE_ROUND32F64SSYM(accu1_ ##N),24),-24)); \
  (*((WORD8 *) p_dst1 + (row+N) * out_row_offset )) = (*((UWORD32 *)&temp_var_ ##N));
#endif

#if (UNROLL_S == 1)
#define SETUP_S SETUP_ROW_S(0)
#define KERNEL_S KERNEL_ROW_S_I(0)
#define STORE_S STORE_ROW_S(0)

#elif (UNROLL_S == 2)
#define SETUP_S  SETUP_ROW_S(0)  SETUP_ROW_S(1)
#define KERNEL_S KERNEL_ROW_S_I(0) KERNEL_ROW_S(1)
#define STORE_S  STORE_ROW_S(0)  STORE_ROW_S(1)

#elif (UNROLL_S == 4)
#define SETUP_S  SETUP_ROW_S(0)  SETUP_ROW_S(1)  SETUP_ROW_S(2)  SETUP_ROW_S(3)
#define KERNEL_S KERNEL_ROW_S_I(0) KERNEL_ROW_S(1) KERNEL_ROW_S(2) KERNEL_ROW_S(3)
#define STORE_S  STORE_ROW_S(0)  STORE_ROW_S(1)  STORE_ROW_S(2)  STORE_ROW_S(3)
#elif (UNROLL_S == 8)
#define SETUP_S   SETUP_ROW_S(0)  SETUP_ROW_S(1)  SETUP_ROW_S(2)  SETUP_ROW_S(3)  SETUP_ROW_S(4)  SETUP_ROW_S(5)  SETUP_ROW_S(6)  SETUP_ROW_S(7)
#define KERNEL_S KERNEL_ROW_S_I(0) KERNEL_ROW_S(1) KERNEL_ROW_S(2) KERNEL_ROW_S(3) KERNEL_ROW_S(4) KERNEL_ROW_S(5) KERNEL_ROW_S(6) KERNEL_ROW_S(7)
#define STORE_S   STORE_ROW_S(0)  STORE_ROW_S(1)  STORE_ROW_S(2)  STORE_ROW_S(3)  STORE_ROW_S(4)  STORE_ROW_S(5)  STORE_ROW_S(6)  STORE_ROW_S(7)

#endif

WORD32 xa_nn_matXvec_8x8_8_circ(
  WORD8  * __restrict__ p_out,
  WORD8  * __restrict__ p_mat,
  WORD8  * __restrict__ p_vec,
  WORD8  * __restrict__ p_bias,
  WORD32 rows,
  WORD32 cols,
  WORD32 row_offset,
  WORD32 vec_count,
  WORD32 vec_offset,
  WORD32 out_col_offset,
  WORD32 out_row_offset,
  WORD32 bias_shift,
  WORD32 acc_shift)
{
  WORD32 row = 0, col = 0, vec=0;
  ae_int16x4 temp_src1, temp_src2;

  if ((NULL == p_out) || (NULL == p_mat) || (NULL == p_vec))
  {
    return -1;
  }

  if ((0 >= rows ) || (0 >= cols ) || (cols & 0x3))
  {
    return -2;
  }
  if(0 >= vec_count) return -3;

  if(vec_count > 1)
  {
    // Process two vectors at a time
    for(vec = 0; vec < (vec_count & (~0x1)); vec+=2)
    {

      WORD8 *p_dst1 = (WORD8 *)&p_out[vec*out_col_offset];
      WORD8 *p_dst2 = (WORD8 *)&p_out[(vec+1)*out_col_offset];

      row = 0;
      if(rows > UNROLL_D)
      {
        for (row = 0; row < ( rows & ~(UNROLL_D-1)) ; row+=UNROLL_D)
        {
          WORD8 *p_src1 = (WORD8 *)&p_vec[vec * vec_offset];
          WORD8 *p_src2 = (WORD8 *)&p_vec[(vec+1) * vec_offset];
          SETUP_D;
#pragma ymemory (p_mat1_0)
#pragma ymemory (p_mat1_1)
#pragma ymemory (p_mat1_2)
#pragma ymemory (p_mat1_3)
          for (col = 0; col < cols>>2; col++) {
            KERNEL_D ;
          }
          STORE_D;
        }
      }
      // Handle remaining rows
      for (; row < rows ; row++)
      {
        WORD8 *p_src1 = (WORD8 *)&p_vec[vec * vec_offset];
        WORD8 *p_src2 = (WORD8 *)&p_vec[(vec+1) * vec_offset];
        SETUP_ROW_D(0);
        for (col = 0; col < cols>>2; col++) {
          KERNEL_ROW_D_I(0);
        }
        STORE_ROW_D(0);
      }
    }
  }
  if(vec_count & 0x1)
  {
    WORD8 *p_dst1 = (WORD8 *)&p_out[vec*out_col_offset];

    row = 0;
    if(rows > UNROLL_S)
    {
      for (row = 0; row < ( rows & ~(UNROLL_S-1)) ; row+=UNROLL_S)
      {
        WORD8 *p_src1 = (WORD8 *)&p_vec[vec *vec_offset];
        SETUP_S;
#pragma ymemory (p_mat1_0)
#pragma ymemory (p_mat1_1)
#pragma ymemory (p_mat1_2)
#pragma ymemory (p_mat1_3)
        for (col = 0; col < (cols>>2); col++) {
          KERNEL_S;
        }
        STORE_S;
      }
    }
    // Handle remaining rows
    for (; row < rows ; row++)
    {
      WORD8 *p_src1 = (WORD8 *)&p_vec[vec * vec_offset];
      SETUP_ROW_S(0);
      for (col = 0; col < (cols>>2); col++) {
        KERNEL_ROW_S_I(0);
      }
      STORE_ROW_S(0);
    }
  }

  return 0;
}

