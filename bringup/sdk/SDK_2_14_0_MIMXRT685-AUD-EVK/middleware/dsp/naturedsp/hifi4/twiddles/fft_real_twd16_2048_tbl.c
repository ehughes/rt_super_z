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
#include "NatureDSP_Signal_fft.h"
#include "common.h"
#include "fft_x16_common.h"

/* Twiddles tables for fft_real32x16, ifft_real32x16, fft_real16x16, ifft_real16x16, N=2048 */
ALIGN(8) static const int16_t __fft_real16_tw[] =
{
    (int16_t)0x0000,(int16_t)0x7fff,
    (int16_t)0x0065,(int16_t)0x7fff,
    (int16_t)0x00c9,(int16_t)0x7fff,
    (int16_t)0x012e,(int16_t)0x7fff,
    (int16_t)0x0192,(int16_t)0x7ffe,
    (int16_t)0x01f7,(int16_t)0x7ffc,
    (int16_t)0x025b,(int16_t)0x7ffa,
    (int16_t)0x02c0,(int16_t)0x7ff8,
    (int16_t)0x0324,(int16_t)0x7ff6,
    (int16_t)0x0389,(int16_t)0x7ff4,
    (int16_t)0x03ed,(int16_t)0x7ff1,
    (int16_t)0x0452,(int16_t)0x7fed,
    (int16_t)0x04b6,(int16_t)0x7fea,
    (int16_t)0x051b,(int16_t)0x7fe6,
    (int16_t)0x057f,(int16_t)0x7fe2,
    (int16_t)0x05e3,(int16_t)0x7fdd,
    (int16_t)0x0648,(int16_t)0x7fd9,
    (int16_t)0x06ac,(int16_t)0x7fd3,
    (int16_t)0x0711,(int16_t)0x7fce,
    (int16_t)0x0775,(int16_t)0x7fc8,
    (int16_t)0x07d9,(int16_t)0x7fc2,
    (int16_t)0x083e,(int16_t)0x7fbc,
    (int16_t)0x08a2,(int16_t)0x7fb5,
    (int16_t)0x0906,(int16_t)0x7fae,
    (int16_t)0x096b,(int16_t)0x7fa7,
    (int16_t)0x09cf,(int16_t)0x7fa0,
    (int16_t)0x0a33,(int16_t)0x7f98,
    (int16_t)0x0a97,(int16_t)0x7f90,
    (int16_t)0x0afb,(int16_t)0x7f87,
    (int16_t)0x0b60,(int16_t)0x7f7e,
    (int16_t)0x0bc4,(int16_t)0x7f75,
    (int16_t)0x0c28,(int16_t)0x7f6c,
    (int16_t)0x0c8c,(int16_t)0x7f62,
    (int16_t)0x0cf0,(int16_t)0x7f58,
    (int16_t)0x0d54,(int16_t)0x7f4e,
    (int16_t)0x0db8,(int16_t)0x7f43,
    (int16_t)0x0e1c,(int16_t)0x7f38,
    (int16_t)0x0e80,(int16_t)0x7f2d,
    (int16_t)0x0ee4,(int16_t)0x7f22,
    (int16_t)0x0f47,(int16_t)0x7f16,
    (int16_t)0x0fab,(int16_t)0x7f0a,
    (int16_t)0x100f,(int16_t)0x7efd,
    (int16_t)0x1073,(int16_t)0x7ef0,
    (int16_t)0x10d6,(int16_t)0x7ee3,
    (int16_t)0x113a,(int16_t)0x7ed6,
    (int16_t)0x119e,(int16_t)0x7ec8,
    (int16_t)0x1201,(int16_t)0x7eba,
    (int16_t)0x1265,(int16_t)0x7eac,
    (int16_t)0x12c8,(int16_t)0x7e9d,
    (int16_t)0x132b,(int16_t)0x7e8e,
    (int16_t)0x138f,(int16_t)0x7e7f,
    (int16_t)0x13f2,(int16_t)0x7e70,
    (int16_t)0x1455,(int16_t)0x7e60,
    (int16_t)0x14b9,(int16_t)0x7e50,
    (int16_t)0x151c,(int16_t)0x7e3f,
    (int16_t)0x157f,(int16_t)0x7e2f,
    (int16_t)0x15e2,(int16_t)0x7e1e,
    (int16_t)0x1645,(int16_t)0x7e0c,
    (int16_t)0x16a8,(int16_t)0x7dfb,
    (int16_t)0x170b,(int16_t)0x7de9,
    (int16_t)0x176e,(int16_t)0x7dd6,
    (int16_t)0x17d1,(int16_t)0x7dc4,
    (int16_t)0x1833,(int16_t)0x7db1,
    (int16_t)0x1896,(int16_t)0x7d9e,
    (int16_t)0x18f9,(int16_t)0x7d8a,
    (int16_t)0x195b,(int16_t)0x7d77,
    (int16_t)0x19be,(int16_t)0x7d63,
    (int16_t)0x1a20,(int16_t)0x7d4e,
    (int16_t)0x1a83,(int16_t)0x7d3a,
    (int16_t)0x1ae5,(int16_t)0x7d25,
    (int16_t)0x1b47,(int16_t)0x7d0f,
    (int16_t)0x1ba9,(int16_t)0x7cfa,
    (int16_t)0x1c0c,(int16_t)0x7ce4,
    (int16_t)0x1c6e,(int16_t)0x7cce,
    (int16_t)0x1cd0,(int16_t)0x7cb7,
    (int16_t)0x1d31,(int16_t)0x7ca0,
    (int16_t)0x1d93,(int16_t)0x7c89,
    (int16_t)0x1df5,(int16_t)0x7c72,
    (int16_t)0x1e57,(int16_t)0x7c5a,
    (int16_t)0x1eb8,(int16_t)0x7c42,
    (int16_t)0x1f1a,(int16_t)0x7c2a,
    (int16_t)0x1f7b,(int16_t)0x7c11,
    (int16_t)0x1fdd,(int16_t)0x7bf9,
    (int16_t)0x203e,(int16_t)0x7bdf,
    (int16_t)0x209f,(int16_t)0x7bc6,
    (int16_t)0x2101,(int16_t)0x7bac,
    (int16_t)0x2162,(int16_t)0x7b92,
    (int16_t)0x21c3,(int16_t)0x7b78,
    (int16_t)0x2224,(int16_t)0x7b5d,
    (int16_t)0x2284,(int16_t)0x7b42,
    (int16_t)0x22e5,(int16_t)0x7b27,
    (int16_t)0x2346,(int16_t)0x7b0b,
    (int16_t)0x23a7,(int16_t)0x7aef,
    (int16_t)0x2407,(int16_t)0x7ad3,
    (int16_t)0x2467,(int16_t)0x7ab7,
    (int16_t)0x24c8,(int16_t)0x7a9a,
    (int16_t)0x2528,(int16_t)0x7a7d,
    (int16_t)0x2588,(int16_t)0x7a60,
    (int16_t)0x25e8,(int16_t)0x7a42,
    (int16_t)0x2648,(int16_t)0x7a24,
    (int16_t)0x26a8,(int16_t)0x7a06,
    (int16_t)0x2708,(int16_t)0x79e7,
    (int16_t)0x2768,(int16_t)0x79c9,
    (int16_t)0x27c7,(int16_t)0x79aa,
    (int16_t)0x2827,(int16_t)0x798a,
    (int16_t)0x2886,(int16_t)0x796a,
    (int16_t)0x28e5,(int16_t)0x794a,
    (int16_t)0x2945,(int16_t)0x792a,
    (int16_t)0x29a4,(int16_t)0x790a,
    (int16_t)0x2a03,(int16_t)0x78e9,
    (int16_t)0x2a62,(int16_t)0x78c8,
    (int16_t)0x2ac1,(int16_t)0x78a6,
    (int16_t)0x2b1f,(int16_t)0x7885,
    (int16_t)0x2b7e,(int16_t)0x7863,
    (int16_t)0x2bdc,(int16_t)0x7840,
    (int16_t)0x2c3b,(int16_t)0x781e,
    (int16_t)0x2c99,(int16_t)0x77fb,
    (int16_t)0x2cf7,(int16_t)0x77d8,
    (int16_t)0x2d55,(int16_t)0x77b4,
    (int16_t)0x2db3,(int16_t)0x7790,
    (int16_t)0x2e11,(int16_t)0x776c,
    (int16_t)0x2e6f,(int16_t)0x7748,
    (int16_t)0x2ecc,(int16_t)0x7723,
    (int16_t)0x2f2a,(int16_t)0x76fe,
    (int16_t)0x2f87,(int16_t)0x76d9,
    (int16_t)0x2fe5,(int16_t)0x76b4,
    (int16_t)0x3042,(int16_t)0x768e,
    (int16_t)0x309f,(int16_t)0x7668,
    (int16_t)0x30fc,(int16_t)0x7642,
    (int16_t)0x3159,(int16_t)0x761b,
    (int16_t)0x31b5,(int16_t)0x75f4,
    (int16_t)0x3212,(int16_t)0x75cd,
    (int16_t)0x326e,(int16_t)0x75a6,
    (int16_t)0x32cb,(int16_t)0x757e,
    (int16_t)0x3327,(int16_t)0x7556,
    (int16_t)0x3383,(int16_t)0x752d,
    (int16_t)0x33df,(int16_t)0x7505,
    (int16_t)0x343b,(int16_t)0x74dc,
    (int16_t)0x3497,(int16_t)0x74b3,
    (int16_t)0x34f2,(int16_t)0x7489,
    (int16_t)0x354e,(int16_t)0x7460,
    (int16_t)0x35a9,(int16_t)0x7436,
    (int16_t)0x3604,(int16_t)0x740b,
    (int16_t)0x365f,(int16_t)0x73e1,
    (int16_t)0x36ba,(int16_t)0x73b6,
    (int16_t)0x3715,(int16_t)0x738b,
    (int16_t)0x3770,(int16_t)0x735f,
    (int16_t)0x37ca,(int16_t)0x7334,
    (int16_t)0x3825,(int16_t)0x7308,
    (int16_t)0x387f,(int16_t)0x72dc,
    (int16_t)0x38d9,(int16_t)0x72af,
    (int16_t)0x3933,(int16_t)0x7282,
    (int16_t)0x398d,(int16_t)0x7255,
    (int16_t)0x39e7,(int16_t)0x7228,
    (int16_t)0x3a40,(int16_t)0x71fa,
    (int16_t)0x3a9a,(int16_t)0x71cc,
    (int16_t)0x3af3,(int16_t)0x719e,
    (int16_t)0x3b4c,(int16_t)0x7170,
    (int16_t)0x3ba5,(int16_t)0x7141,
    (int16_t)0x3bfe,(int16_t)0x7112,
    (int16_t)0x3c57,(int16_t)0x70e3,
    (int16_t)0x3caf,(int16_t)0x70b3,
    (int16_t)0x3d08,(int16_t)0x7083,
    (int16_t)0x3d60,(int16_t)0x7053,
    (int16_t)0x3db8,(int16_t)0x7023,
    (int16_t)0x3e10,(int16_t)0x6ff2,
    (int16_t)0x3e68,(int16_t)0x6fc2,
    (int16_t)0x3ec0,(int16_t)0x6f90,
    (int16_t)0x3f17,(int16_t)0x6f5f,
    (int16_t)0x3f6f,(int16_t)0x6f2d,
    (int16_t)0x3fc6,(int16_t)0x6efb,
    (int16_t)0x401d,(int16_t)0x6ec9,
    (int16_t)0x4074,(int16_t)0x6e97,
    (int16_t)0x40cb,(int16_t)0x6e64,
    (int16_t)0x4121,(int16_t)0x6e31,
    (int16_t)0x4178,(int16_t)0x6dfe,
    (int16_t)0x41ce,(int16_t)0x6dca,
    (int16_t)0x4224,(int16_t)0x6d96,
    (int16_t)0x427a,(int16_t)0x6d62,
    (int16_t)0x42d0,(int16_t)0x6d2e,
    (int16_t)0x4326,(int16_t)0x6cf9,
    (int16_t)0x437b,(int16_t)0x6cc4,
    (int16_t)0x43d1,(int16_t)0x6c8f,
    (int16_t)0x4426,(int16_t)0x6c5a,
    (int16_t)0x447b,(int16_t)0x6c24,
    (int16_t)0x44d0,(int16_t)0x6bee,
    (int16_t)0x4524,(int16_t)0x6bb8,
    (int16_t)0x4579,(int16_t)0x6b82,
    (int16_t)0x45cd,(int16_t)0x6b4b,
    (int16_t)0x4621,(int16_t)0x6b14,
    (int16_t)0x4675,(int16_t)0x6add,
    (int16_t)0x46c9,(int16_t)0x6aa5,
    (int16_t)0x471d,(int16_t)0x6a6e,
    (int16_t)0x4770,(int16_t)0x6a36,
    (int16_t)0x47c4,(int16_t)0x69fd,
    (int16_t)0x4817,(int16_t)0x69c5,
    (int16_t)0x486a,(int16_t)0x698c,
    (int16_t)0x48bd,(int16_t)0x6953,
    (int16_t)0x490f,(int16_t)0x691a,
    (int16_t)0x4962,(int16_t)0x68e0,
    (int16_t)0x49b4,(int16_t)0x68a7,
    (int16_t)0x4a06,(int16_t)0x686d,
    (int16_t)0x4a58,(int16_t)0x6832,
    (int16_t)0x4aaa,(int16_t)0x67f8,
    (int16_t)0x4afb,(int16_t)0x67bd,
    (int16_t)0x4b4d,(int16_t)0x6782,
    (int16_t)0x4b9e,(int16_t)0x6747,
    (int16_t)0x4bef,(int16_t)0x670b,
    (int16_t)0x4c40,(int16_t)0x66d0,
    (int16_t)0x4c91,(int16_t)0x6693,
    (int16_t)0x4ce1,(int16_t)0x6657,
    (int16_t)0x4d31,(int16_t)0x661b,
    (int16_t)0x4d81,(int16_t)0x65de,
    (int16_t)0x4dd1,(int16_t)0x65a1,
    (int16_t)0x4e21,(int16_t)0x6564,
    (int16_t)0x4e71,(int16_t)0x6526,
    (int16_t)0x4ec0,(int16_t)0x64e9,
    (int16_t)0x4f0f,(int16_t)0x64ab,
    (int16_t)0x4f5e,(int16_t)0x646c,
    (int16_t)0x4fad,(int16_t)0x642e,
    (int16_t)0x4ffb,(int16_t)0x63ef,
    (int16_t)0x504a,(int16_t)0x63b0,
    (int16_t)0x5098,(int16_t)0x6371,
    (int16_t)0x50e6,(int16_t)0x6332,
    (int16_t)0x5134,(int16_t)0x62f2,
    (int16_t)0x5181,(int16_t)0x62b2,
    (int16_t)0x51cf,(int16_t)0x6272,
    (int16_t)0x521c,(int16_t)0x6232,
    (int16_t)0x5269,(int16_t)0x61f1,
    (int16_t)0x52b6,(int16_t)0x61b0,
    (int16_t)0x5303,(int16_t)0x616f,
    (int16_t)0x534f,(int16_t)0x612e,
    (int16_t)0x539b,(int16_t)0x60ec,
    (int16_t)0x53e7,(int16_t)0x60aa,
    (int16_t)0x5433,(int16_t)0x6068,
    (int16_t)0x547f,(int16_t)0x6026,
    (int16_t)0x54ca,(int16_t)0x5fe4,
    (int16_t)0x5515,(int16_t)0x5fa1,
    (int16_t)0x5560,(int16_t)0x5f5e,
    (int16_t)0x55ab,(int16_t)0x5f1b,
    (int16_t)0x55f6,(int16_t)0x5ed7,
    (int16_t)0x5640,(int16_t)0x5e94,
    (int16_t)0x568a,(int16_t)0x5e50,
    (int16_t)0x56d4,(int16_t)0x5e0c,
    (int16_t)0x571e,(int16_t)0x5dc8,
    (int16_t)0x5767,(int16_t)0x5d83,
    (int16_t)0x57b1,(int16_t)0x5d3e,
    (int16_t)0x57fa,(int16_t)0x5cf9,
    (int16_t)0x5843,(int16_t)0x5cb4,
    (int16_t)0x588c,(int16_t)0x5c6f,
    (int16_t)0x58d4,(int16_t)0x5c29,
    (int16_t)0x591c,(int16_t)0x5be3,
    (int16_t)0x5964,(int16_t)0x5b9d,
    (int16_t)0x59ac,(int16_t)0x5b57,
    (int16_t)0x59f4,(int16_t)0x5b10,
    (int16_t)0x5a3b,(int16_t)0x5ac9,
    (int16_t)0x5a82,(int16_t)0x5a82,
    (int16_t)0x5ac9,(int16_t)0x5a3b,
    (int16_t)0x5b10,(int16_t)0x59f4,
    (int16_t)0x5b57,(int16_t)0x59ac,
    (int16_t)0x5b9d,(int16_t)0x5964,
    (int16_t)0x5be3,(int16_t)0x591c,
    (int16_t)0x5c29,(int16_t)0x58d4,
    (int16_t)0x5c6f,(int16_t)0x588c,
    (int16_t)0x5cb4,(int16_t)0x5843,
    (int16_t)0x5cf9,(int16_t)0x57fa,
    (int16_t)0x5d3e,(int16_t)0x57b1,
    (int16_t)0x5d83,(int16_t)0x5767,
    (int16_t)0x5dc8,(int16_t)0x571e,
    (int16_t)0x5e0c,(int16_t)0x56d4,
    (int16_t)0x5e50,(int16_t)0x568a,
    (int16_t)0x5e94,(int16_t)0x5640,
    (int16_t)0x5ed7,(int16_t)0x55f6,
    (int16_t)0x5f1b,(int16_t)0x55ab,
    (int16_t)0x5f5e,(int16_t)0x5560,
    (int16_t)0x5fa1,(int16_t)0x5515,
    (int16_t)0x5fe4,(int16_t)0x54ca,
    (int16_t)0x6026,(int16_t)0x547f,
    (int16_t)0x6068,(int16_t)0x5433,
    (int16_t)0x60aa,(int16_t)0x53e7,
    (int16_t)0x60ec,(int16_t)0x539b,
    (int16_t)0x612e,(int16_t)0x534f,
    (int16_t)0x616f,(int16_t)0x5303,
    (int16_t)0x61b0,(int16_t)0x52b6,
    (int16_t)0x61f1,(int16_t)0x5269,
    (int16_t)0x6232,(int16_t)0x521c,
    (int16_t)0x6272,(int16_t)0x51cf,
    (int16_t)0x62b2,(int16_t)0x5181,
    (int16_t)0x62f2,(int16_t)0x5134,
    (int16_t)0x6332,(int16_t)0x50e6,
    (int16_t)0x6371,(int16_t)0x5098,
    (int16_t)0x63b0,(int16_t)0x504a,
    (int16_t)0x63ef,(int16_t)0x4ffb,
    (int16_t)0x642e,(int16_t)0x4fad,
    (int16_t)0x646c,(int16_t)0x4f5e,
    (int16_t)0x64ab,(int16_t)0x4f0f,
    (int16_t)0x64e9,(int16_t)0x4ec0,
    (int16_t)0x6526,(int16_t)0x4e71,
    (int16_t)0x6564,(int16_t)0x4e21,
    (int16_t)0x65a1,(int16_t)0x4dd1,
    (int16_t)0x65de,(int16_t)0x4d81,
    (int16_t)0x661b,(int16_t)0x4d31,
    (int16_t)0x6657,(int16_t)0x4ce1,
    (int16_t)0x6693,(int16_t)0x4c91,
    (int16_t)0x66d0,(int16_t)0x4c40,
    (int16_t)0x670b,(int16_t)0x4bef,
    (int16_t)0x6747,(int16_t)0x4b9e,
    (int16_t)0x6782,(int16_t)0x4b4d,
    (int16_t)0x67bd,(int16_t)0x4afb,
    (int16_t)0x67f8,(int16_t)0x4aaa,
    (int16_t)0x6832,(int16_t)0x4a58,
    (int16_t)0x686d,(int16_t)0x4a06,
    (int16_t)0x68a7,(int16_t)0x49b4,
    (int16_t)0x68e0,(int16_t)0x4962,
    (int16_t)0x691a,(int16_t)0x490f,
    (int16_t)0x6953,(int16_t)0x48bd,
    (int16_t)0x698c,(int16_t)0x486a,
    (int16_t)0x69c5,(int16_t)0x4817,
    (int16_t)0x69fd,(int16_t)0x47c4,
    (int16_t)0x6a36,(int16_t)0x4770,
    (int16_t)0x6a6e,(int16_t)0x471d,
    (int16_t)0x6aa5,(int16_t)0x46c9,
    (int16_t)0x6add,(int16_t)0x4675,
    (int16_t)0x6b14,(int16_t)0x4621,
    (int16_t)0x6b4b,(int16_t)0x45cd,
    (int16_t)0x6b82,(int16_t)0x4579,
    (int16_t)0x6bb8,(int16_t)0x4524,
    (int16_t)0x6bee,(int16_t)0x44d0,
    (int16_t)0x6c24,(int16_t)0x447b,
    (int16_t)0x6c5a,(int16_t)0x4426,
    (int16_t)0x6c8f,(int16_t)0x43d1,
    (int16_t)0x6cc4,(int16_t)0x437b,
    (int16_t)0x6cf9,(int16_t)0x4326,
    (int16_t)0x6d2e,(int16_t)0x42d0,
    (int16_t)0x6d62,(int16_t)0x427a,
    (int16_t)0x6d96,(int16_t)0x4224,
    (int16_t)0x6dca,(int16_t)0x41ce,
    (int16_t)0x6dfe,(int16_t)0x4178,
    (int16_t)0x6e31,(int16_t)0x4121,
    (int16_t)0x6e64,(int16_t)0x40cb,
    (int16_t)0x6e97,(int16_t)0x4074,
    (int16_t)0x6ec9,(int16_t)0x401d,
    (int16_t)0x6efb,(int16_t)0x3fc6,
    (int16_t)0x6f2d,(int16_t)0x3f6f,
    (int16_t)0x6f5f,(int16_t)0x3f17,
    (int16_t)0x6f90,(int16_t)0x3ec0,
    (int16_t)0x6fc2,(int16_t)0x3e68,
    (int16_t)0x6ff2,(int16_t)0x3e10,
    (int16_t)0x7023,(int16_t)0x3db8,
    (int16_t)0x7053,(int16_t)0x3d60,
    (int16_t)0x7083,(int16_t)0x3d08,
    (int16_t)0x70b3,(int16_t)0x3caf,
    (int16_t)0x70e3,(int16_t)0x3c57,
    (int16_t)0x7112,(int16_t)0x3bfe,
    (int16_t)0x7141,(int16_t)0x3ba5,
    (int16_t)0x7170,(int16_t)0x3b4c,
    (int16_t)0x719e,(int16_t)0x3af3,
    (int16_t)0x71cc,(int16_t)0x3a9a,
    (int16_t)0x71fa,(int16_t)0x3a40,
    (int16_t)0x7228,(int16_t)0x39e7,
    (int16_t)0x7255,(int16_t)0x398d,
    (int16_t)0x7282,(int16_t)0x3933,
    (int16_t)0x72af,(int16_t)0x38d9,
    (int16_t)0x72dc,(int16_t)0x387f,
    (int16_t)0x7308,(int16_t)0x3825,
    (int16_t)0x7334,(int16_t)0x37ca,
    (int16_t)0x735f,(int16_t)0x3770,
    (int16_t)0x738b,(int16_t)0x3715,
    (int16_t)0x73b6,(int16_t)0x36ba,
    (int16_t)0x73e1,(int16_t)0x365f,
    (int16_t)0x740b,(int16_t)0x3604,
    (int16_t)0x7436,(int16_t)0x35a9,
    (int16_t)0x7460,(int16_t)0x354e,
    (int16_t)0x7489,(int16_t)0x34f2,
    (int16_t)0x74b3,(int16_t)0x3497,
    (int16_t)0x74dc,(int16_t)0x343b,
    (int16_t)0x7505,(int16_t)0x33df,
    (int16_t)0x752d,(int16_t)0x3383,
    (int16_t)0x7556,(int16_t)0x3327,
    (int16_t)0x757e,(int16_t)0x32cb,
    (int16_t)0x75a6,(int16_t)0x326e,
    (int16_t)0x75cd,(int16_t)0x3212,
    (int16_t)0x75f4,(int16_t)0x31b5,
    (int16_t)0x761b,(int16_t)0x3159,
    (int16_t)0x7642,(int16_t)0x30fc,
    (int16_t)0x7668,(int16_t)0x309f,
    (int16_t)0x768e,(int16_t)0x3042,
    (int16_t)0x76b4,(int16_t)0x2fe5,
    (int16_t)0x76d9,(int16_t)0x2f87,
    (int16_t)0x76fe,(int16_t)0x2f2a,
    (int16_t)0x7723,(int16_t)0x2ecc,
    (int16_t)0x7748,(int16_t)0x2e6f,
    (int16_t)0x776c,(int16_t)0x2e11,
    (int16_t)0x7790,(int16_t)0x2db3,
    (int16_t)0x77b4,(int16_t)0x2d55,
    (int16_t)0x77d8,(int16_t)0x2cf7,
    (int16_t)0x77fb,(int16_t)0x2c99,
    (int16_t)0x781e,(int16_t)0x2c3b,
    (int16_t)0x7840,(int16_t)0x2bdc,
    (int16_t)0x7863,(int16_t)0x2b7e,
    (int16_t)0x7885,(int16_t)0x2b1f,
    (int16_t)0x78a6,(int16_t)0x2ac1,
    (int16_t)0x78c8,(int16_t)0x2a62,
    (int16_t)0x78e9,(int16_t)0x2a03,
    (int16_t)0x790a,(int16_t)0x29a4,
    (int16_t)0x792a,(int16_t)0x2945,
    (int16_t)0x794a,(int16_t)0x28e5,
    (int16_t)0x796a,(int16_t)0x2886,
    (int16_t)0x798a,(int16_t)0x2827,
    (int16_t)0x79aa,(int16_t)0x27c7,
    (int16_t)0x79c9,(int16_t)0x2768,
    (int16_t)0x79e7,(int16_t)0x2708,
    (int16_t)0x7a06,(int16_t)0x26a8,
    (int16_t)0x7a24,(int16_t)0x2648,
    (int16_t)0x7a42,(int16_t)0x25e8,
    (int16_t)0x7a60,(int16_t)0x2588,
    (int16_t)0x7a7d,(int16_t)0x2528,
    (int16_t)0x7a9a,(int16_t)0x24c8,
    (int16_t)0x7ab7,(int16_t)0x2467,
    (int16_t)0x7ad3,(int16_t)0x2407,
    (int16_t)0x7aef,(int16_t)0x23a7,
    (int16_t)0x7b0b,(int16_t)0x2346,
    (int16_t)0x7b27,(int16_t)0x22e5,
    (int16_t)0x7b42,(int16_t)0x2284,
    (int16_t)0x7b5d,(int16_t)0x2224,
    (int16_t)0x7b78,(int16_t)0x21c3,
    (int16_t)0x7b92,(int16_t)0x2162,
    (int16_t)0x7bac,(int16_t)0x2101,
    (int16_t)0x7bc6,(int16_t)0x209f,
    (int16_t)0x7bdf,(int16_t)0x203e,
    (int16_t)0x7bf9,(int16_t)0x1fdd,
    (int16_t)0x7c11,(int16_t)0x1f7b,
    (int16_t)0x7c2a,(int16_t)0x1f1a,
    (int16_t)0x7c42,(int16_t)0x1eb8,
    (int16_t)0x7c5a,(int16_t)0x1e57,
    (int16_t)0x7c72,(int16_t)0x1df5,
    (int16_t)0x7c89,(int16_t)0x1d93,
    (int16_t)0x7ca0,(int16_t)0x1d31,
    (int16_t)0x7cb7,(int16_t)0x1cd0,
    (int16_t)0x7cce,(int16_t)0x1c6e,
    (int16_t)0x7ce4,(int16_t)0x1c0c,
    (int16_t)0x7cfa,(int16_t)0x1ba9,
    (int16_t)0x7d0f,(int16_t)0x1b47,
    (int16_t)0x7d25,(int16_t)0x1ae5,
    (int16_t)0x7d3a,(int16_t)0x1a83,
    (int16_t)0x7d4e,(int16_t)0x1a20,
    (int16_t)0x7d63,(int16_t)0x19be,
    (int16_t)0x7d77,(int16_t)0x195b,
    (int16_t)0x7d8a,(int16_t)0x18f9,
    (int16_t)0x7d9e,(int16_t)0x1896,
    (int16_t)0x7db1,(int16_t)0x1833,
    (int16_t)0x7dc4,(int16_t)0x17d1,
    (int16_t)0x7dd6,(int16_t)0x176e,
    (int16_t)0x7de9,(int16_t)0x170b,
    (int16_t)0x7dfb,(int16_t)0x16a8,
    (int16_t)0x7e0c,(int16_t)0x1645,
    (int16_t)0x7e1e,(int16_t)0x15e2,
    (int16_t)0x7e2f,(int16_t)0x157f,
    (int16_t)0x7e3f,(int16_t)0x151c,
    (int16_t)0x7e50,(int16_t)0x14b9,
    (int16_t)0x7e60,(int16_t)0x1455,
    (int16_t)0x7e70,(int16_t)0x13f2,
    (int16_t)0x7e7f,(int16_t)0x138f,
    (int16_t)0x7e8e,(int16_t)0x132b,
    (int16_t)0x7e9d,(int16_t)0x12c8,
    (int16_t)0x7eac,(int16_t)0x1265,
    (int16_t)0x7eba,(int16_t)0x1201,
    (int16_t)0x7ec8,(int16_t)0x119e,
    (int16_t)0x7ed6,(int16_t)0x113a,
    (int16_t)0x7ee3,(int16_t)0x10d6,
    (int16_t)0x7ef0,(int16_t)0x1073,
    (int16_t)0x7efd,(int16_t)0x100f,
    (int16_t)0x7f0a,(int16_t)0x0fab,
    (int16_t)0x7f16,(int16_t)0x0f47,
    (int16_t)0x7f22,(int16_t)0x0ee4,
    (int16_t)0x7f2d,(int16_t)0x0e80,
    (int16_t)0x7f38,(int16_t)0x0e1c,
    (int16_t)0x7f43,(int16_t)0x0db8,
    (int16_t)0x7f4e,(int16_t)0x0d54,
    (int16_t)0x7f58,(int16_t)0x0cf0,
    (int16_t)0x7f62,(int16_t)0x0c8c,
    (int16_t)0x7f6c,(int16_t)0x0c28,
    (int16_t)0x7f75,(int16_t)0x0bc4,
    (int16_t)0x7f7e,(int16_t)0x0b60,
    (int16_t)0x7f87,(int16_t)0x0afb,
    (int16_t)0x7f90,(int16_t)0x0a97,
    (int16_t)0x7f98,(int16_t)0x0a33,
    (int16_t)0x7fa0,(int16_t)0x09cf,
    (int16_t)0x7fa7,(int16_t)0x096b,
    (int16_t)0x7fae,(int16_t)0x0906,
    (int16_t)0x7fb5,(int16_t)0x08a2,
    (int16_t)0x7fbc,(int16_t)0x083e,
    (int16_t)0x7fc2,(int16_t)0x07d9,
    (int16_t)0x7fc8,(int16_t)0x0775,
    (int16_t)0x7fce,(int16_t)0x0711,
    (int16_t)0x7fd3,(int16_t)0x06ac,
    (int16_t)0x7fd9,(int16_t)0x0648,
    (int16_t)0x7fdd,(int16_t)0x05e3,
    (int16_t)0x7fe2,(int16_t)0x057f,
    (int16_t)0x7fe6,(int16_t)0x051b,
    (int16_t)0x7fea,(int16_t)0x04b6,
    (int16_t)0x7fed,(int16_t)0x0452,
    (int16_t)0x7ff1,(int16_t)0x03ed,
    (int16_t)0x7ff4,(int16_t)0x0389,
    (int16_t)0x7ff6,(int16_t)0x0324,
    (int16_t)0x7ff8,(int16_t)0x02c0,
    (int16_t)0x7ffa,(int16_t)0x025b,
    (int16_t)0x7ffc,(int16_t)0x01f7,
    (int16_t)0x7ffe,(int16_t)0x0192,
    (int16_t)0x7fff,(int16_t)0x012e,
    (int16_t)0x7fff,(int16_t)0x00c9,
    (int16_t)0x7fff,(int16_t)0x0065
};

static const fft_real_x16_descr_t __rfft_descr =
{
    &__cfft_x16_descr1024,
    __fft_real16_tw
};
static const fft_real_x16_descr_t __rifft_descr =
{
    &__cifft_x16_descr1024,
    __fft_real16_tw
};
const fft_handle_t rfft16_2048 =  (const fft_handle_t)&__rfft_descr;
const fft_handle_t rifft16_2048 = (const fft_handle_t)&__rifft_descr;