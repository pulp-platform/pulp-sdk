/*
 * Copyright (C) 2018 ETH Zurich and University of Bologna
 * Copyright (C) 2026 Fondazione Chips-IT
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/*
 * xpulpv2 provider for corev-gcc
 */

#ifndef __ARCHI_RISCV_BUILTINS_COREV_V2_H__
#define __ARCHI_RISCV_BUILTINS_COREV_V2_H__

#ifdef ARCHI_CORE_HAS_COREV_V2

/* v2s/v2u/v4s/v4u are all 4 bytes: reinterpret, not convert. Costs nothing. */
#define __CV_PUN(from_t, to_t, name) \
  static inline to_t name(from_t x) { union { from_t f; to_t t; } c; c.f = x; return c.t; }

__CV_PUN(v2s, unsigned int, __cv_u_of_v2s)
__CV_PUN(v2u, unsigned int, __cv_u_of_v2u)
__CV_PUN(v4s, unsigned int, __cv_u_of_v4s)
__CV_PUN(v4u, unsigned int, __cv_u_of_v4u)
__CV_PUN(unsigned int, v2s, __cv_v2s_of_u)
__CV_PUN(unsigned int, v2u, __cv_v2u_of_u)
__CV_PUN(unsigned int, v4s, __cv_v4s_of_u)
__CV_PUN(unsigned int, v4u, __cv_v4u_of_u)

#undef __CV_PUN

/* Packed 10-bit (size-1)<<5|offset field used by extract/insert/bset/bclr. */
#ifndef __ExtInsMaskFast
static inline unsigned int __attribute__ ((always_inline)) __ExtInsMaskFast(unsigned int Size, unsigned int Offset) { return ((((Size-1))<<5)|(Offset)); }
static inline unsigned int __attribute__ ((always_inline)) __ExtInsMaskSafe(unsigned int Size, unsigned int Offset) { return ((((Size-1)&0x1F)<<5)|(Offset&0x1F)); }
#endif


/* ==== SIMD 16-bit (cv.*.h) ============================================= */

/* add/sub take a div factor: 0 = none, 1/2/3 = div2/div4/div8. */
#define __ADD2(x, y)  __cv_v2s_of_u(__builtin_riscv_cv_simd_add_h(__cv_u_of_v2s(x), __cv_u_of_v2s(y), 0))
#define __SUB2(x, y)  __cv_v2s_of_u(__builtin_riscv_cv_simd_sub_h(__cv_u_of_v2s(x), __cv_u_of_v2s(y), 0))
#define __AVG2(x, y)  __cv_v2s_of_u(__builtin_riscv_cv_simd_avg_h (__cv_u_of_v2s(x), __cv_u_of_v2s(y)))
#define __AVGU2(x, y) __cv_v2u_of_u(__builtin_riscv_cv_simd_avgu_h(__cv_u_of_v2u(x), __cv_u_of_v2u(y)))

#define __MAX2(x, y)  __cv_v2s_of_u(__builtin_riscv_cv_simd_max_h (__cv_u_of_v2s(x), __cv_u_of_v2s(y)))
#define __MAXU2(x, y) __cv_v2u_of_u(__builtin_riscv_cv_simd_maxu_h(__cv_u_of_v2u(x), __cv_u_of_v2u(y)))
#define __MIN2(x, y)  __cv_v2s_of_u(__builtin_riscv_cv_simd_min_h (__cv_u_of_v2s(x), __cv_u_of_v2s(y)))
#define __MINU2(x, y) __cv_v2u_of_u(__builtin_riscv_cv_simd_minu_h(__cv_u_of_v2u(x), __cv_u_of_v2u(y)))

#define __ABS2(x) __cv_v2s_of_u(__builtin_riscv_cv_simd_abs_h(__cv_u_of_v2s(x)))
#define __NEG2(x) __cv_v2s_of_u(__builtin_riscv_cv_simd_sub_h(0u, __cv_u_of_v2s(x), 0))

#define __AND2(x, y)  __cv_v2s_of_u(__builtin_riscv_cv_simd_and_h(__cv_u_of_v2s(x), __cv_u_of_v2s(y)))
#define __OR2(x, y)   __cv_v2s_of_u(__builtin_riscv_cv_simd_or_h (__cv_u_of_v2s(x), __cv_u_of_v2s(y)))
#define __EXOR2(x, y) __cv_v2s_of_u(__builtin_riscv_cv_simd_xor_h(__cv_u_of_v2s(x), __cv_u_of_v2s(y)))
#define __SRL2(x, y)  __cv_v2s_of_u(__builtin_riscv_cv_simd_srl_h(__cv_u_of_v2s(x), __cv_u_of_v2s(y)))
#define __SRA2(x, y)  __cv_v2s_of_u(__builtin_riscv_cv_simd_sra_h(__cv_u_of_v2s(x), __cv_u_of_v2s(y)))
#define __SLL2(x, y)  __cv_v2s_of_u(__builtin_riscv_cv_simd_sll_h(__cv_u_of_v2s(x), __cv_u_of_v2s(y)))

/* ==== SIMD 8-bit (cv.*.b) ============================================== */

#define __ADD4(x, y)  __cv_v4s_of_u(__builtin_riscv_cv_simd_add_b (__cv_u_of_v4s(x), __cv_u_of_v4s(y)))
#define __SUB4(x, y)  __cv_v4s_of_u(__builtin_riscv_cv_simd_sub_b (__cv_u_of_v4s(x), __cv_u_of_v4s(y)))
#define __AVG4(x, y)  __cv_v4s_of_u(__builtin_riscv_cv_simd_avg_b (__cv_u_of_v4s(x), __cv_u_of_v4s(y)))
#define __AVGU4(x, y) __cv_v4u_of_u(__builtin_riscv_cv_simd_avgu_b(__cv_u_of_v4u(x), __cv_u_of_v4u(y)))

#define __MAX4(x, y)  __cv_v4s_of_u(__builtin_riscv_cv_simd_max_b (__cv_u_of_v4s(x), __cv_u_of_v4s(y)))
#define __MAXU4(x, y) __cv_v4u_of_u(__builtin_riscv_cv_simd_maxu_b(__cv_u_of_v4u(x), __cv_u_of_v4u(y)))
#define __MIN4(x, y)  __cv_v4s_of_u(__builtin_riscv_cv_simd_min_b (__cv_u_of_v4s(x), __cv_u_of_v4s(y)))
#define __MINU4(x, y) __cv_v4u_of_u(__builtin_riscv_cv_simd_minu_b(__cv_u_of_v4u(x), __cv_u_of_v4u(y)))

#define __ABS4(x) __cv_v4s_of_u(__builtin_riscv_cv_simd_abs_b(__cv_u_of_v4s(x)))
#define __NEG4(x) __cv_v4s_of_u(__builtin_riscv_cv_simd_sub_b(0u, __cv_u_of_v4s(x)))

#define __AND4(x, y)  __cv_v4s_of_u(__builtin_riscv_cv_simd_and_b(__cv_u_of_v4s(x), __cv_u_of_v4s(y)))
#define __OR4(x, y)   __cv_v4s_of_u(__builtin_riscv_cv_simd_or_b (__cv_u_of_v4s(x), __cv_u_of_v4s(y)))
#define __EXOR4(x, y) __cv_v4s_of_u(__builtin_riscv_cv_simd_xor_b(__cv_u_of_v4s(x), __cv_u_of_v4s(y)))
#define __SRL4(x, y)  __cv_v4s_of_u(__builtin_riscv_cv_simd_srl_b(__cv_u_of_v4s(x), __cv_u_of_v4s(y)))
#define __SRA4(x, y)  __cv_v4s_of_u(__builtin_riscv_cv_simd_sra_b(__cv_u_of_v4s(x), __cv_u_of_v4s(y)))
#define __SLL4(x, y)  __cv_v4s_of_u(__builtin_riscv_cv_simd_sll_b(__cv_u_of_v4s(x), __cv_u_of_v4s(y)))

/* ==== SIMD pack ======================================================== */

/* Misleading names: packlo_h emits cv.pack, packhi_h emits cv.pack.h.
 * cv.pack puts rs1 in the HIGH half, so the arguments swap vs pack2(x,y).
 * The .b forms write half of rD and read the rest via a third operand. */
#define __PACK2(x, y)  __cv_v2s_of_u(__builtin_riscv_cv_simd_packlo_h((unsigned int)(y), (unsigned int)(x)))
#define __PACKU2(x, y) __cv_v2u_of_u(__builtin_riscv_cv_simd_packlo_h((unsigned int)(y), (unsigned int)(x)))

#define __PACK4(x, y, z, t) \
  __cv_v4s_of_u(__builtin_riscv_cv_simd_packhi_b((unsigned int)(t), (unsigned int)(z), \
                  __builtin_riscv_cv_simd_packlo_b((unsigned int)(y), (unsigned int)(x), 0u)))
#define __PACKU4(x, y, z, t) \
  __cv_v4u_of_u(__builtin_riscv_cv_simd_packhi_b((unsigned int)(t), (unsigned int)(z), \
                  __builtin_riscv_cv_simd_packlo_b((unsigned int)(y), (unsigned int)(x), 0u)))

/* ==== SIMD dot products ================================================ */

#define __DOTP2(x, y)   ((int)         __builtin_riscv_cv_simd_dotsp_h (__cv_u_of_v2s(x), __cv_u_of_v2s(y)))
#define __DOTPU2(x, y)  ((unsigned int)__builtin_riscv_cv_simd_dotup_h (__cv_u_of_v2u(x), __cv_u_of_v2u(y)))
#define __DOTPUS2(x, y) ((int)         __builtin_riscv_cv_simd_dotusp_h(__cv_u_of_v2u(x), __cv_u_of_v2s(y)))

#define __DOTP4(x, y)   ((int)         __builtin_riscv_cv_simd_dotsp_b (__cv_u_of_v4s(x), __cv_u_of_v4s(y)))
#define __DOTPU4(x, y)  ((unsigned int)__builtin_riscv_cv_simd_dotup_b (__cv_u_of_v4u(x), __cv_u_of_v4u(y)))
#define __DOTPUS4(x, y) ((int)         __builtin_riscv_cv_simd_dotusp_b(__cv_u_of_v4u(x), __cv_u_of_v4s(y)))

/* _SC_ forms: second operand is a scalar replicated across the lanes. */
#define __DOTPSC2(x, y)    ((int)         __builtin_riscv_cv_simd_dotsp_sc_h (__cv_u_of_v2s(x), (short)(y)))
#define __DOTPUSC2(x, y)   ((unsigned int)__builtin_riscv_cv_simd_dotup_sc_h (__cv_u_of_v2u(x), (unsigned short)(y)))
#define __DOTPUSSC2(x, y)  ((int)         __builtin_riscv_cv_simd_dotusp_sc_h(__cv_u_of_v2u(x), (short)(y)))

#define __DOTPSC4(x, y)    ((int)         __builtin_riscv_cv_simd_dotsp_sc_b (__cv_u_of_v4s(x), (signed char)(y)))
#define __DOTPUSC4(x, y)   ((unsigned int)__builtin_riscv_cv_simd_dotup_sc_b (__cv_u_of_v4u(x), (unsigned char)(y)))
#define __DOTPUSSC4(x, y)  ((int)         __builtin_riscv_cv_simd_dotusp_sc_b(__cv_u_of_v4u(x), (signed char)(y)))

#define __SUMDOTP2(x, y, z)   ((int)         __builtin_riscv_cv_simd_sdotsp_h (__cv_u_of_v2s(x), __cv_u_of_v2s(y), (unsigned int)(z)))
#define __SUMDOTPU2(x, y, z)  ((unsigned int)__builtin_riscv_cv_simd_sdotup_h (__cv_u_of_v2u(x), __cv_u_of_v2u(y), (unsigned int)(z)))
#define __SUMDOTPUS2(x, y, z) ((int)         __builtin_riscv_cv_simd_sdotusp_h(__cv_u_of_v2u(x), __cv_u_of_v2s(y), (unsigned int)(z)))

#define __SUMDOTP4(x, y, z)   ((int)         __builtin_riscv_cv_simd_sdotsp_b (__cv_u_of_v4s(x), __cv_u_of_v4s(y), (unsigned int)(z)))
#define __SUMDOTPU4(x, y, z)  ((unsigned int)__builtin_riscv_cv_simd_sdotup_b (__cv_u_of_v4u(x), __cv_u_of_v4u(y), (unsigned int)(z)))
#define __SUMDOTPUS4(x, y, z) ((int)         __builtin_riscv_cv_simd_sdotusp_b(__cv_u_of_v4u(x), __cv_u_of_v4s(y), (unsigned int)(z)))

#define __SUMDOTPSC2(x, y, z)   ((int)         __builtin_riscv_cv_simd_sdotsp_sc_h (__cv_u_of_v2s(x), (short)(y),          (unsigned int)(z)))
#define __SUMDOTPUSC2(x, y, z)  ((unsigned int)__builtin_riscv_cv_simd_sdotup_sc_h (__cv_u_of_v2u(x), (unsigned short)(y), (unsigned int)(z)))
#define __SUMDOTPUSSC2(x, y, z) ((int)         __builtin_riscv_cv_simd_sdotusp_sc_h(__cv_u_of_v2u(x), (short)(y),          (unsigned int)(z)))

#define __SUMDOTPSC4(x, y, z)   ((int)         __builtin_riscv_cv_simd_sdotsp_sc_b (__cv_u_of_v4s(x), (signed char)(y),   (unsigned int)(z)))
#define __SUMDOTPUSC4(x, y, z)  ((unsigned int)__builtin_riscv_cv_simd_sdotup_sc_b (__cv_u_of_v4u(x), (unsigned char)(y), (unsigned int)(z)))
#define __SUMDOTPUSSC4(x, y, z) ((int)         __builtin_riscv_cv_simd_sdotusp_sc_b(__cv_u_of_v4u(x), (signed char)(y),   (unsigned int)(z)))

/* ==== Fixed-point multiply / MAC (cv.mac) ============================== */

/* mulsN uses the low halfwords, mulhhsN the high ones; N is the right shift,
 * so the plain forms are N = 0. The RN forms round; unlike pulp's builtins the
 * rounding constant is implied, so there is no extra argument. */
#define __MULS(x, y)   __builtin_riscv_cv_mac_mulsN  ((int)(x), (int)(y), 0)
#define __MULU(x, y)   __builtin_riscv_cv_mac_muluN  ((unsigned int)(x), (unsigned int)(y), 0)
#define __MULHHS(x, y) __builtin_riscv_cv_mac_mulhhsN((int)(x), (int)(y), 0)
#define __MULHHU(x, y) __builtin_riscv_cv_mac_mulhhuN((unsigned int)(x), (unsigned int)(y), 0)

#define __MULSN(x, y, n)    __builtin_riscv_cv_mac_mulsN   ((int)(x), (int)(y), (n))
#define __MULUN(x, y, n)    __builtin_riscv_cv_mac_muluN   ((unsigned int)(x), (unsigned int)(y), (n))
#define __MULHHSN(x, y, n)  __builtin_riscv_cv_mac_mulhhsN ((int)(x), (int)(y), (n))
#define __MULHHUN(x, y, n)  __builtin_riscv_cv_mac_mulhhuN ((unsigned int)(x), (unsigned int)(y), (n))
#define __MULSRN(x, y, n)   __builtin_riscv_cv_mac_mulsRN  ((int)(x), (int)(y), (n))
#define __MULURN(x, y, n)   __builtin_riscv_cv_mac_muluRN  ((unsigned int)(x), (unsigned int)(y), (n))
#define __MULHHSRN(x, y, n) __builtin_riscv_cv_mac_mulhhsRN((int)(x), (int)(y), (n))
#define __MULHHURN(x, y, n) __builtin_riscv_cv_mac_mulhhuRN((unsigned int)(x), (unsigned int)(y), (n))

/* 32x32 mac/msu; the accumulator is tied to the result. */
#define __MAC(Acc, x, y) __builtin_riscv_cv_mac_mac((int)(x), (int)(y), (int)(Acc))
#define __MSU(Acc, x, y) __builtin_riscv_cv_mac_msu((int)(x), (int)(y), (int)(Acc))

#define __MACS(Acc, x, y)   __builtin_riscv_cv_mac_macsN  ((int)(x), (int)(y), (int)(Acc), 0)
#define __MACU(Acc, x, y)   __builtin_riscv_cv_mac_macuN  ((unsigned int)(x), (unsigned int)(y), (unsigned int)(Acc), 0)
#define __MACHHS(Acc, x, y) __builtin_riscv_cv_mac_machhsN((int)(x), (int)(y), (int)(Acc), 0)
#define __MACHHU(Acc, x, y) __builtin_riscv_cv_mac_machhuN((unsigned int)(x), (unsigned int)(y), (unsigned int)(Acc), 0)

#define __MACSN(Acc, x, y, n)    __builtin_riscv_cv_mac_macsN   ((int)(x), (int)(y), (int)(Acc), (n))
#define __MACUN(Acc, x, y, n)    __builtin_riscv_cv_mac_macuN   ((unsigned int)(x), (unsigned int)(y), (unsigned int)(Acc), (n))
#define __MACHHSN(Acc, x, y, n)  __builtin_riscv_cv_mac_machhsN ((int)(x), (int)(y), (int)(Acc), (n))
#define __MACHHUN(Acc, x, y, n)  __builtin_riscv_cv_mac_machhuN ((unsigned int)(x), (unsigned int)(y), (unsigned int)(Acc), (n))
#define __MACSRN(Acc, x, y, n)   __builtin_riscv_cv_mac_macsRN  ((int)(x), (int)(y), (int)(Acc), (n))
#define __MACURN(Acc, x, y, n)   __builtin_riscv_cv_mac_macuRN  ((unsigned int)(x), (unsigned int)(y), (unsigned int)(Acc), (n))
#define __MACHHSRN(Acc, x, y, n) __builtin_riscv_cv_mac_machhsRN((int)(x), (int)(y), (int)(Acc), (n))
#define __MACHHURN(Acc, x, y, n) __builtin_riscv_cv_mac_machhuRN((unsigned int)(x), (unsigned int)(y), (unsigned int)(Acc), (n))

/* ==== Scalar ALU (cv.clip, cv.addN, cv.max) ============================ */

/* clip's immediate is the bound, not the bit count: CV_alu_pow2 wants ival+1 a
 * power of two. A non-constant bound selects cv.clipr.
 * clipu's operand is signed: bit 31 set yields 0, not the bound. */
#define __CLIP(x, precision)  __builtin_riscv_cv_alu_clip ((int)(x),          (1  << (precision)) - 1)
#define __CLIPU(x, precision) __builtin_riscv_cv_alu_clipu((unsigned int)(x), (1u << (precision)) - 1u)
#define __CLIP_R(x, bound)    __builtin_riscv_cv_alu_clip ((int)(x),          (bound))
#define __CLIPU_R(x, bound)   __builtin_riscv_cv_alu_clipu((unsigned int)(x), (bound))

/* addN/subN carry both an immediate and a register alternative, so the _REG
 * forms are the same intrinsic with a non-constant scale. */
#define __ADDNORM(x, y, scale)          __builtin_riscv_cv_alu_addN  ((int)(x), (int)(y), (scale))
#define __ADDNORM_REG(x, y, scale)      __builtin_riscv_cv_alu_addN  ((int)(x), (int)(y), (scale))
#define __ADDNORMU(x, y, scale)         __builtin_riscv_cv_alu_adduN ((unsigned int)(x), (unsigned int)(y), (scale))
#define __ADDNORMU_REG(x, y, scale)     __builtin_riscv_cv_alu_adduN ((unsigned int)(x), (unsigned int)(y), (scale))
#define __ADDROUNDNORM(x, y, scale)     __builtin_riscv_cv_alu_addRN ((int)(x), (int)(y), (scale))
#define __ADDROUNDNORM_REG(x, y, scale) __builtin_riscv_cv_alu_addRN ((int)(x), (int)(y), (scale))
#define __ADDROUNDNORMU(x, y, scale)    __builtin_riscv_cv_alu_adduRN((unsigned int)(x), (unsigned int)(y), (scale))
#define __ADDROUNDNORMU_REG(x, y, scale)__builtin_riscv_cv_alu_adduRN((unsigned int)(x), (unsigned int)(y), (scale))

#define __SUBNORM(x, y, scale)          __builtin_riscv_cv_alu_subN  ((int)(x), (int)(y), (scale))
#define __SUBNORM_REG(x, y, scale)      __builtin_riscv_cv_alu_subN  ((int)(x), (int)(y), (scale))
#define __SUBNORMU(x, y, scale)         __builtin_riscv_cv_alu_subuN ((unsigned int)(x), (unsigned int)(y), (scale))
#define __SUBNORMU_REG(x, y, scale)     __builtin_riscv_cv_alu_subuN ((unsigned int)(x), (unsigned int)(y), (scale))
#define __SUBROUNDNORM(x, y, scale)     __builtin_riscv_cv_alu_subRN ((int)(x), (int)(y), (scale))
#define __SUBROUNDNORM_REG(x, y, scale) __builtin_riscv_cv_alu_subRN ((int)(x), (int)(y), (scale))
#define __SUBROUNDNORMU(x, y, scale)    __builtin_riscv_cv_alu_subuRN((unsigned int)(x), (unsigned int)(y), (scale))
#define __SUBROUNDNORMU_REG(x, y, scale)__builtin_riscv_cv_alu_subuRN((unsigned int)(x), (unsigned int)(y), (scale))

/* Normalise-and-round is add-with-zero, as in builtins_v2.h. */
#define __ROUNDNORM(x, scale)      __builtin_riscv_cv_alu_addRN ((int)(x), 0, (scale))
#define __ROUNDNORM_REG(x, scale)  __builtin_riscv_cv_alu_addRN ((int)(x), 0, (scale))
#define __ROUNDNORMU(x, scale)     __builtin_riscv_cv_alu_adduRN((unsigned int)(x), 0u, (scale))
#define __ROUNDNORMU_REG(x, scale) __builtin_riscv_cv_alu_adduRN((unsigned int)(x), 0u, (scale))

#define __MAX(a, b) (((int)(a) > (int)(b)) ? ((int)a) : ((int)b))

/* ==== Bit manipulation (cv.bset/bclr/extract/insert/ff1/fl1/clb/ror) ==== */

/* All take the packed 10-bit field and have an immediate and a register
 * alternative, so _R / _R_SAFE differ only in how the field is built. */
#define __BITSET(x, size, off)        __builtin_riscv_cv_bitmanip_bset((unsigned int)(x), __ExtInsMaskFast((size), (off)))
#define __BITSET_R(x, size, off)      __builtin_riscv_cv_bitmanip_bset((unsigned int)(x), __ExtInsMaskFast((size), (off)))
#define __BITSET_R_SAFE(x, size, off) __builtin_riscv_cv_bitmanip_bset((unsigned int)(x), __ExtInsMaskSafe((size), (off)))

#define __BITCLR(x, size, off)        __builtin_riscv_cv_bitmanip_bclr((unsigned int)(x), __ExtInsMaskFast((size), (off)))
#define __BITCLR_R(x, size, off)      __builtin_riscv_cv_bitmanip_bclr((unsigned int)(x), __ExtInsMaskFast((size), (off)))
#define __BITCLR_R_SAFE(x, size, off) __builtin_riscv_cv_bitmanip_bclr((unsigned int)(x), __ExtInsMaskSafe((size), (off)))

#define __BITEXTRACT(x, size, off)        __builtin_riscv_cv_bitmanip_extract((unsigned int)(x), __ExtInsMaskFast((size), (off)))
#define __BITEXTRACT_R(x, size, off)      __builtin_riscv_cv_bitmanip_extract((unsigned int)(x), __ExtInsMaskFast((size), (off)))
#define __BITEXTRACT_R_SAFE(x, size, off) __builtin_riscv_cv_bitmanip_extract((unsigned int)(x), __ExtInsMaskSafe((size), (off)))

#define __BITEXTRACTU(x, size, off)        __builtin_riscv_cv_bitmanip_extractu((unsigned int)(x), __ExtInsMaskFast((size), (off)))
#define __BITEXTRACTU_R(x, size, off)      __builtin_riscv_cv_bitmanip_extractu((unsigned int)(x), __ExtInsMaskFast((size), (off)))
#define __BITEXTRACTU_R_SAFE(x, size, off) __builtin_riscv_cv_bitmanip_extractu((unsigned int)(x), __ExtInsMaskSafe((size), (off)))

#define __BITINSERT(dst, src, size, off)        __builtin_riscv_cv_bitmanip_insert((unsigned int)(dst), __ExtInsMaskFast((size), (off)), (unsigned int)(src))
#define __BITINSERT_R(dst, src, size, off)      __builtin_riscv_cv_bitmanip_insert((unsigned int)(dst), __ExtInsMaskFast((size), (off)), (unsigned int)(src))
#define __BITINSERT_R_SAFE(dst, src, size, off) __builtin_riscv_cv_bitmanip_insert((unsigned int)(dst), __ExtInsMaskSafe((size), (off)), (unsigned int)(src))

#define __FF1(x) ((unsigned int)__builtin_ctz((unsigned int)(x)))
static inline unsigned int __cv_fl1(unsigned int x)
{
    unsigned int result;

    __asm__("cv.fl1 %0, %1"
            : "=r"(result)
            : "r"(x));

    return result;
}
#define __FL1(x) __cv_fl1((unsigned int)(x))
#define __CLB(x) ((unsigned int)__builtin_riscv_cv_bitmanip_clb((unsigned int)(x)))
#define __CNT(x) ((unsigned int)__builtin_popcount((unsigned int)(x)))
#define __ROTR(x) ((unsigned int)__builtin_rotateright32((unsigned int)(x), 1u))

/* ==== Core / SPR access ================================================ */

/* Not ISA extensions: plain CSRs. The hart id is {22'b0, cluster, core}. */
#define __COREID()    ({ unsigned int __h; asm ("csrr %0, 0xF14" : "=r"(__h)); __h & 0x1f; })
#define __CLUSTERID() ({ unsigned int __h; asm ("csrr %0, 0xF14" : "=r"(__h)); (__h >> 5) & 0x3f; })
#define __NCORE()     (ARCHI_CLUSTER_NB_PE)
#ifdef ARCHI_HAS_FC
#define __ISFC()      (__CLUSTERID() == ARCHI_FC_CID)
#else
#define __ISFC()      0
#endif

#define __SPRREAD(spr)     ({ unsigned int __v; asm ("csrr %0, %1" : "=r"(__v) : "i"(spr)); __v; })
#define __SPRREAD_VOL(spr) ({ unsigned int __v; asm volatile ("csrr %0, %1" : "=r"(__v) : "i"(spr)); __v; })
#define __SPRWRITE(spr, x) asm volatile ("csrw %0, %1" :: "i"(spr), "r"(x))

#define __SPRBITSET(spr, off) asm volatile ("csrs %0, %1" :: "i"(spr), "r"(1u << (off)))
#define __SPRBITCLR(spr, off) asm volatile ("csrc %0, %1" :: "i"(spr), "r"(1u << (off)))

#define __SPRREADTHENWRITE(spr, x)    ({ unsigned int __v; asm volatile ("csrrw %0, %1, %2" : "=r"(__v) : "i"(spr), "r"(x)); __v; })
#define __SPRREADTHENBITSET(spr, off) ({ unsigned int __v; asm volatile ("csrrs %0, %1, %2" : "=r"(__v) : "i"(spr), "r"(1u << (off))); __v; })
#define __SPRREADTHENBITCLR(spr, off) ({ unsigned int __v; asm volatile ("csrrc %0, %1, %2" : "=r"(__v) : "i"(spr), "r"(1u << (off))); __v; })

/* ==== Base + offset memory access ====================================== */

/* Xcvmem has no intrinsics; GCC emits cv.lw/cv.sw from ordinary pointer code. */
#define __READ_BASE_OFF(base, off)             (*(unsigned int *)((char *)(base) + (off)))
#define __WRITE_BASE_OFF(base, off, val)       (*(unsigned int *)((char *)(base) + (off)) = (val))

#define __READ_BASE_OFF_VOL(base, off)         (*(volatile unsigned int   *)((char *)(base) + (off)))
#define __READ_BASE_OFF_HALF_VOL(base, off)    (*(volatile unsigned short *)((char *)(base) + (off)))
#define __READ_BASE_OFF_BYTE_VOL(base, off)    (*(volatile unsigned char  *)((char *)(base) + (off)))

#define __WRITE_BASE_OFF_VOL(x, base, off)      (*(volatile unsigned int   *)((char *)(base) + (off)) = (x))
#define __WRITE_BASE_OFF_HALF_VOL(x, base, off) (*(volatile unsigned short *)((char *)(base) + (off)) = (x))
#define __WRITE_BASE_OFF_BYTE_VOL(x, base, off) (*(volatile unsigned char  *)((char *)(base) + (off)) = (x))

/* ==== Complex / Viterbi ================================================ */

#ifdef ARCHI_CORE_HAS_CPLX
/* cplxmul's third operand is tied to the result; pulp's form returns a fresh
 * value, so pass 0. Fourth operand is the div factor. */
#define __CPLXMULS(x, y)     __cv_v2s_of_u(__builtin_riscv_cv_simd_cplxmul_r(__cv_u_of_v2s(x), __cv_u_of_v2s(y), 0u, 0))
#define __CPLXMULSDIV2(x, y) __cv_v2s_of_u(__builtin_riscv_cv_simd_cplxmul_r(__cv_u_of_v2s(x), __cv_u_of_v2s(y), 0u, 1))
#define __CPLXMULSDIV4(x, y) __cv_v2s_of_u(__builtin_riscv_cv_simd_cplxmul_r(__cv_u_of_v2s(x), __cv_u_of_v2s(y), 0u, 2))
#define __CPLXCONJ(x)        __cv_v2s_of_u(__builtin_riscv_cv_simd_cplxconj(__cv_u_of_v2s(x)))
#define __SUB2ROTMJ(x, y)    __cv_v2s_of_u(__builtin_riscv_cv_simd_subrotmj(__cv_u_of_v2s(x), __cv_u_of_v2s(y), 0))

#define __ADD2DIV2(x, y) __cv_v2s_of_u(__builtin_riscv_cv_simd_add_h(__cv_u_of_v2s(x), __cv_u_of_v2s(y), 1))
#define __ADD2DIV4(x, y) __cv_v2s_of_u(__builtin_riscv_cv_simd_add_h(__cv_u_of_v2s(x), __cv_u_of_v2s(y), 2))
#define __SUB2DIV2(x, y) __cv_v2s_of_u(__builtin_riscv_cv_simd_sub_h(__cv_u_of_v2s(x), __cv_u_of_v2s(y), 1))
#define __SUB2DIV4(x, y) __cv_v2s_of_u(__builtin_riscv_cv_simd_sub_h(__cv_u_of_v2s(x), __cv_u_of_v2s(y), 2))

/* No byte div variant in hardware: cv.add.div2/4/8 are halfword only. */
#define __ADD4DIV2(x, y) __SRA4(__ADD4((x), (y)), (v4s){1,1,1,1})
#define __ADD4DIV4(x, y) __SRA4(__ADD4((x), (y)), (v4s){2,2,2,2})
#define __SUB4DIV2(x, y) __SRA4(__SUB4((x), (y)), (v4s){1,1,1,1})
#define __SUB4DIV4(x, y) __SRA4(__SUB4((x), (y)), (v4s){2,2,2,2})

/* __VITMAX / __VITSEL are xpulpv3; CV32E40P has no Viterbi unit. Left
 * undefined so a build that needs them fails instead of degrading. */
#endif

/* ==== The __builtin_* layer ============================================ */

#define __builtin_pack2(x, y)        __PACK2((x), (y))
#define __builtin_packu2(x, y)       __PACKU2((x), (y))
#define __builtin_pack4(x, y, z, t)  __PACK4((x), (y), (z), (t))
#define __builtin_packu4(x, y, z, t) __PACKU4((x), (y), (z), (t))

#define __builtin_max2(x, y)  __MAX2((x), (y))
#define __builtin_max4(x, y)  __MAX4((x), (y))
#define __builtin_maxu2(x, y) __MAXU2((x), (y))
#define __builtin_maxu4(x, y) __MAXU4((x), (y))
#define __builtin_min2(x, y)  __MIN2((x), (y))
#define __builtin_min4(x, y)  __MIN4((x), (y))
#define __builtin_minu2(x, y) __MINU2((x), (y))
#define __builtin_minu4(x, y) __MINU4((x), (y))
#define __builtin_abs2(x)     __ABS2((x))
#define __builtin_abs4(x)     __ABS4((x))

#define __builtin_dotp2(x, y)   __DOTP2((x), (y))
#define __builtin_dotpu2(x, y)  __DOTPU2((x), (y))
#define __builtin_dotpus2(x, y) __DOTPUS2((x), (y))
#define __builtin_dotp4(x, y)   __DOTP4((x), (y))
#define __builtin_dotpu4(x, y)  __DOTPU4((x), (y))
#define __builtin_dotpus4(x, y) __DOTPUS4((x), (y))

#define __builtin_sumdotp2(x, y, z)   __SUMDOTP2((x), (y), (z))
#define __builtin_sumdotpu2(x, y, z)  __SUMDOTPU2((x), (y), (z))
#define __builtin_sumdotpus2(x, y, z) __SUMDOTPUS2((x), (y), (z))
#define __builtin_sumdotp4(x, y, z)   __SUMDOTP4((x), (y), (z))
#define __builtin_sumdotpu4(x, y, z)  __SUMDOTPU4((x), (y), (z))
#define __builtin_sumdotpus4(x, y, z) __SUMDOTPUS4((x), (y), (z))

#define __builtin_clip(x, precision)  __CLIP((x), (precision))
#define __builtin_clipu(x, precision) __CLIPU((x), (precision))

#define __builtin_mulsN(x, y, n)    __MULSN((x), (y), (n))
#define __builtin_mulsRN(x, y, n)   __MULSRN((x), (y), (n))
#define __builtin_muluN(x, y, n)    __MULUN((x), (y), (n))
#define __builtin_muluRN(x, y, n)   __MULURN((x), (y), (n))
#define __builtin_mulhhsN(x, y, n)  __MULHHSN((x), (y), (n))
#define __builtin_mulhhsRN(x, y, n) __MULHHSRN((x), (y), (n))
#define __builtin_mulhhuN(x, y, n)  __MULHHUN((x), (y), (n))
#define __builtin_mulhhuRN(x, y, n) __MULHHURN((x), (y), (n))

#define __builtin_macs(Acc, x, y)   __MACS((Acc), (x), (y))
#define __builtin_macu(Acc, x, y)   __MACU((Acc), (x), (y))
#define __builtin_machhs(Acc, x, y) __MACHHS((Acc), (x), (y))
#define __builtin_machhu(Acc, x, y) __MACHHU((Acc), (x), (y))

#define __builtin_macsN(Acc, x, y, n)    __MACSN((Acc), (x), (y), (n))
#define __builtin_macuN(Acc, x, y, n)    __MACUN((Acc), (x), (y), (n))
#define __builtin_macsRN(Acc, x, y, n)   __MACSRN((Acc), (x), (y), (n))
#define __builtin_macuRN(Acc, x, y, n)   __MACURN((Acc), (x), (y), (n))
#define __builtin_machhsN(Acc, x, y, n)  __MACHHSN((Acc), (x), (y), (n))
#define __builtin_machhuN(Acc, x, y, n)  __MACHHUN((Acc), (x), (y), (n))
#define __builtin_machhsRN(Acc, x, y, n) __MACHHSRN((Acc), (x), (y), (n))
#define __builtin_machhuRN(Acc, x, y, n) __MACHHURN((Acc), (x), (y), (n))

#define __builtin_bitextract(x, size, off)           __BITEXTRACT((x), (size), (off))
#define __builtin_bitextractu(x, size, off)          __BITEXTRACTU((x), (size), (off))
#define __builtin_bitinsert(dst, src, size, off)     __BITINSERT((dst), (src), (size), (off))
#define __builtin_bitinsert_r(dst, src, size, off)   __BITINSERT_R((dst), (src), (size), (off))

#define __builtin_addroundnorm(x, y, scale)  __ADDROUNDNORM((x), (y), (scale))
#define __builtin_addroundnormu(x, y, scale) __ADDROUNDNORMU((x), (y), (scale))
#define __builtin_roundnorm(x, scale)        __ROUNDNORM((x), (scale))
#define __builtin_roundnormu(x, scale)       __ROUNDNORMU((x), (scale))

#define __builtin_fl1(x)  __FL1((x))
#define __builtin_clb(x)  __CLB((x))
#define __builtin_rotr(x) __ROTR((x))

#endif /* ARCHI_CORE_HAS_COREV_V2 */

#endif
