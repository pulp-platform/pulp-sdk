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
 * The legacy __builtin_pulp_* names, for third-party code that calls them
 * directly (pulp-nn-mixed, Deeploy).
 *
 * Pure alias layer: everything forwards to the __XXX names in
 * builtins_corev_v2.h, so the ISA semantics live in exactly one place. Include
 * after it. Where pulp's signature differs, only the shape is adapted here:
 * accumulator-first argument order, and the RN forms' explicit rounding
 * constant, which CoreV implies.
 */

#ifndef __ARCHI_RISCV_BUILTINS_PULP_COMPAT_H__
#define __ARCHI_RISCV_BUILTINS_PULP_COMPAT_H__

#ifdef ARCHI_CORE_HAS_COREV_V2

/* ---- SIMD 16-bit ------------------------------------------------------- */

#define __builtin_pulp_pack2(x, y)  __PACK2((x), (y))
#define __builtin_pulp_add2(x, y)   __ADD2((x), (y))
#define __builtin_pulp_sub2(x, y)   __SUB2((x), (y))
#define __builtin_pulp_avg2(x, y)   __AVG2((x), (y))
#define __builtin_pulp_avgu2(x, y)  __AVGU2((x), (y))
#define __builtin_pulp_max2(x, y)   __MAX2((x), (y))
#define __builtin_pulp_maxu2(x, y)  __MAXU2((x), (y))
#define __builtin_pulp_min2(x, y)   __MIN2((x), (y))
#define __builtin_pulp_minu2(x, y)  __MINU2((x), (y))
#define __builtin_pulp_abs2(x)      __ABS2((x))
#define __builtin_pulp_neg2(x)      __NEG2((x))
#define __builtin_pulp_and2(x, y)   __AND2((x), (y))
#define __builtin_pulp_or2(x, y)    __OR2((x), (y))
#define __builtin_pulp_exor2(x, y)  __EXOR2((x), (y))
#define __builtin_pulp_srl2(x, y)   __SRL2((x), (y))
#define __builtin_pulp_sra2(x, y)   __SRA2((x), (y))
#define __builtin_pulp_sll2(x, y)   __SLL2((x), (y))

/* ---- SIMD 8-bit ------------------------------------------------------- */

#define __builtin_pulp_pack4(x, y, z, t) __PACK4((x), (y), (z), (t))
#define __builtin_pulp_add4(x, y)   __ADD4((x), (y))
#define __builtin_pulp_sub4(x, y)   __SUB4((x), (y))
#define __builtin_pulp_avg4(x, y)   __AVG4((x), (y))
#define __builtin_pulp_avgu4(x, y)  __AVGU4((x), (y))
#define __builtin_pulp_max4(x, y)   __MAX4((x), (y))
#define __builtin_pulp_maxu4(x, y)  __MAXU4((x), (y))
#define __builtin_pulp_min4(x, y)   __MIN4((x), (y))
#define __builtin_pulp_minu4(x, y)  __MINU4((x), (y))
#define __builtin_pulp_abs4(x)      __ABS4((x))
#define __builtin_pulp_neg4(x)      __NEG4((x))
#define __builtin_pulp_and4(x, y)   __AND4((x), (y))
#define __builtin_pulp_or4(x, y)    __OR4((x), (y))
#define __builtin_pulp_exor4(x, y)  __EXOR4((x), (y))
#define __builtin_pulp_srl4(x, y)   __SRL4((x), (y))
#define __builtin_pulp_sra4(x, y)   __SRA4((x), (y))
#define __builtin_pulp_sll4(x, y)   __SLL4((x), (y))

/* ---- Dot products ----------------------------------------------------- */

#define __builtin_pulp_dotsp2(x, y)     __DOTP2((x), (y))
#define __builtin_pulp_dotup2(x, y)     __DOTPU2((x), (y))
#define __builtin_pulp_dotusp2(x, y)    __DOTPUS2((x), (y))
#define __builtin_pulp_dotspsc2(x, y)   __DOTPSC2((x), (y))
#define __builtin_pulp_dotupsc2(x, y)   __DOTPUSC2((x), (y))
#define __builtin_pulp_dotuspsc2(x, y)  __DOTPUSSC2((x), (y))

#define __builtin_pulp_dotsp4(x, y)     __DOTP4((x), (y))
#define __builtin_pulp_dotup4(x, y)     __DOTPU4((x), (y))
#define __builtin_pulp_dotusp4(x, y)    __DOTPUS4((x), (y))
#define __builtin_pulp_dotspsc4(x, y)   __DOTPSC4((x), (y))
#define __builtin_pulp_dotupsc4(x, y)   __DOTPUSC4((x), (y))
#define __builtin_pulp_dotuspsc4(x, y)  __DOTPUSSC4((x), (y))

#define __builtin_pulp_sdotsp2(x, y, z)    __SUMDOTP2((x), (y), (z))
#define __builtin_pulp_sdotup2(x, y, z)    __SUMDOTPU2((x), (y), (z))
#define __builtin_pulp_sdotusp2(x, y, z)   __SUMDOTPUS2((x), (y), (z))
#define __builtin_pulp_sdotspsc2(x, y, z)  __SUMDOTPSC2((x), (y), (z))
#define __builtin_pulp_sdotupsc2(x, y, z)  __SUMDOTPUSC2((x), (y), (z))
#define __builtin_pulp_sdotuspsc2(x, y, z) __SUMDOTPUSSC2((x), (y), (z))

#define __builtin_pulp_sdotsp4(x, y, z)    __SUMDOTP4((x), (y), (z))
#define __builtin_pulp_sdotup4(x, y, z)    __SUMDOTPU4((x), (y), (z))
#define __builtin_pulp_sdotusp4(x, y, z)   __SUMDOTPUS4((x), (y), (z))
#define __builtin_pulp_sdotspsc4(x, y, z)  __SUMDOTPSC4((x), (y), (z))
#define __builtin_pulp_sdotupsc4(x, y, z)  __SUMDOTPUSC4((x), (y), (z))
#define __builtin_pulp_sdotuspsc4(x, y, z) __SUMDOTPUSSC4((x), (y), (z))

/* ---- Fixed-point multiply / MAC --------------------------------------- */

/* pulp puts the accumulator last, the __XXX layer puts it first. */
#define __builtin_pulp_mac(x, y, Acc)  __MAC((Acc), (x), (y))
#define __builtin_pulp_msu(x, y, Acc)  __MSU((Acc), (x), (y))

#define __builtin_pulp_macs(x, y, Acc)    __MACS((Acc), (x), (y))
#define __builtin_pulp_macu(x, y, Acc)    __MACU((Acc), (x), (y))
#define __builtin_pulp_machhs(x, y, Acc)  __MACHHS((Acc), (x), (y))
#define __builtin_pulp_machhu(x, y, Acc)  __MACHHU((Acc), (x), (y))

#define __builtin_pulp_macsN(x, y, Acc, n)    __MACSN((Acc), (x), (y), (n))
#define __builtin_pulp_macuN(x, y, Acc, n)    __MACUN((Acc), (x), (y), (n))
#define __builtin_pulp_machhsN(x, y, Acc, n)  __MACHHSN((Acc), (x), (y), (n))
#define __builtin_pulp_machhuN(x, y, Acc, n)  __MACHHUN((Acc), (x), (y), (n))

/* The RN forms take an explicit rounding constant that CoreV implies. */
#define __builtin_pulp_macsRN(x, y, Acc, n, rnd)   __MACSRN((Acc), (x), (y), (n))
#define __builtin_pulp_macuRN(x, y, Acc, n, rnd)   __MACURN((Acc), (x), (y), (n))

#define __builtin_pulp_muls(x, y)    __MULS((x), (y))
#define __builtin_pulp_mulu(x, y)    __MULU((x), (y))
#define __builtin_pulp_mulhhs(x, y)  __MULHHS((x), (y))
#define __builtin_pulp_mulhhu(x, y)  __MULHHU((x), (y))

#define __builtin_pulp_mulsN(x, y, n)    __MULSN((x), (y), (n))
#define __builtin_pulp_muluN(x, y, n)    __MULUN((x), (y), (n))
#define __builtin_pulp_mulhhsN(x, y, n)  __MULHHSN((x), (y), (n))
#define __builtin_pulp_mulhhuN(x, y, n)  __MULHHUN((x), (y), (n))

#define __builtin_pulp_mulsRN(x, y, n, rnd)    __MULSRN((x), (y), (n))
#define __builtin_pulp_muluRN(x, y, n, rnd)    __MULURN((x), (y), (n))
#define __builtin_pulp_mulhhsRN(x, y, n, rnd)  __MULHHSRN((x), (y), (n))
#define __builtin_pulp_mulhhuRN(x, y, n, rnd)  __MULHHURN((x), (y), (n))

/* ---- Add/sub with normalisation --------------------------------------- */

#define __builtin_pulp_addN(x, y, s)       __ADDNORM((x), (y), (s))
#define __builtin_pulp_addN_r(x, y, s)     __ADDNORM_REG((x), (y), (s))
#define __builtin_pulp_adduN(x, y, s)      __ADDNORMU((x), (y), (s))
#define __builtin_pulp_adduN_r(x, y, s)    __ADDNORMU_REG((x), (y), (s))
#define __builtin_pulp_addRN(x, y, s, rnd) __ADDROUNDNORM((x), (y), (s))
#define __builtin_pulp_addRN_r(x, y, s)    __ADDROUNDNORM_REG((x), (y), (s))
#define __builtin_pulp_adduRN(x, y, s, rnd) __ADDROUNDNORMU((x), (y), (s))
#define __builtin_pulp_adduRN_r(x, y, s)   __ADDROUNDNORMU_REG((x), (y), (s))

#define __builtin_pulp_subN(x, y, s)       __SUBNORM((x), (y), (s))
#define __builtin_pulp_subN_r(x, y, s)     __SUBNORM_REG((x), (y), (s))
#define __builtin_pulp_subuN(x, y, s)      __SUBNORMU((x), (y), (s))
#define __builtin_pulp_subuN_r(x, y, s)    __SUBNORMU_REG((x), (y), (s))
#define __builtin_pulp_subRN(x, y, s, rnd) __SUBROUNDNORM((x), (y), (s))
#define __builtin_pulp_subRN_r(x, y, s)    __SUBROUNDNORM_REG((x), (y), (s))
#define __builtin_pulp_subuRN(x, y, s, rnd) __SUBROUNDNORMU((x), (y), (s))
#define __builtin_pulp_subuRN_r(x, y, s)   __SUBROUNDNORMU_REG((x), (y), (s))

/* ---- Clip ------------------------------------------------------------- */

/* pulp passes both bounds; only the upper one is encoded, and lo = -(hi+1). */
#define __builtin_pulp_clip(x, lo, hi)   __CLIP_R((x), (hi))
#define __builtin_pulp_clipu(x, lo, hi)  __CLIPU_R((x), (hi))
#define __builtin_pulp_clip_r(x, bound)  __CLIP_R((x), (bound))
#define __builtin_pulp_clipu_r(x, bound) __CLIPU_R((x), (bound))

/* ---- Scalar min/max --------------------------------------------------- */

/* Only __MAX has an __XXX name; the other three go straight to the intrinsic. */
#define __builtin_pulp_maxsi(a, b)  __MAX((a), (b))
#define __builtin_pulp_minsi(a, b)  __builtin_riscv_cv_alu_min ((int)(a), (int)(b))
#define __builtin_pulp_maxusi(a, b) __builtin_riscv_cv_alu_maxu((unsigned int)(a), (unsigned int)(b))
#define __builtin_pulp_minusi(a, b) __builtin_riscv_cv_alu_minu((unsigned int)(a), (unsigned int)(b))

/* ---- Bit manipulation ------------------------------------------------- */

#define __builtin_pulp_ff1(x)  __FF1((x))
#define __builtin_pulp_fl1(x)  __FL1((x))
#define __builtin_pulp_clb(x)  __CLB((x))
#define __builtin_pulp_cnt(x)  __CNT((x))
/* pulp's rotr takes the amount; __ROTR fixes it at 1. */
#define __builtin_pulp_rotr(x, n) __builtin_riscv_cv_bitmanip_ror((unsigned int)(x), (n))

#define __builtin_pulp_bextract(x, size, off)  __BITEXTRACT((x), (size), (off))
#define __builtin_pulp_bextractu(x, size, off) __BITEXTRACTU((x), (size), (off))

/* The _r / mask forms carry a pre-built field or mask instead of size+offset,
 * so they cannot go through the size/offset macros. */
#define __builtin_pulp_bextract_r(x, field)  __builtin_riscv_cv_bitmanip_extract ((unsigned int)(x), (field))
#define __builtin_pulp_bextractu_r(x, field) __builtin_riscv_cv_bitmanip_extractu((unsigned int)(x), (field))
#define __builtin_pulp_bset_r(x, field)      __builtin_riscv_cv_bitmanip_bset    ((unsigned int)(x), (field))
#define __builtin_pulp_bclr_r(x, field)      __builtin_riscv_cv_bitmanip_bclr    ((unsigned int)(x), (field))
#define __builtin_pulp_binsert_r(dst, src, field) \
  __builtin_riscv_cv_bitmanip_insert((unsigned int)(dst), (field), (unsigned int)(src))

#define __builtin_pulp_bset(x, mask)    (((unsigned int)(x)) |  ((unsigned int)(mask)))
#define __builtin_pulp_bclr(x, notmask) (((unsigned int)(x)) &  ((unsigned int)(notmask)))
#define __builtin_pulp_binsert(dst, not_mask_imm, src, mask_imm, off) \
  ((((unsigned int)(dst)) & ((unsigned int)(not_mask_imm))) | \
   ((((unsigned int)(src)) << (off)) & ((unsigned int)(mask_imm))))

/* ---- Core / SPR / memory --------------------------------------------- */

#define __builtin_pulp_CoreId()     __COREID()
#define __builtin_pulp_ClusterId()  __CLUSTERID()
#define __builtin_pulp_IsFc()       __ISFC()

#define __builtin_pulp_spr_read(spr)      __SPRREAD(spr)
#define __builtin_pulp_spr_read_vol(spr)  __SPRREAD_VOL(spr)
#define __builtin_pulp_spr_write(spr, x)  __SPRWRITE((spr), (x))
#define __builtin_pulp_spr_bit_set(spr, off) __SPRBITSET((spr), (off))
#define __builtin_pulp_spr_bit_clr(spr, off) __SPRBITCLR((spr), (off))
#define __builtin_pulp_read_then_spr_write(spr, x)     __SPRREADTHENWRITE((spr), (x))
#define __builtin_pulp_read_then_spr_bit_set(spr, off) __SPRREADTHENBITSET((spr), (off))
#define __builtin_pulp_read_then_spr_bit_clr(spr, off) __SPRREADTHENBITCLR((spr), (off))

#define __builtin_pulp_read_base_off(base, off)       __READ_BASE_OFF((base), (off))
#define __builtin_pulp_write_base_off(base, off, val) __WRITE_BASE_OFF((base), (off), (val))
#define __builtin_pulp_OffsetedRead(base, off)        __READ_BASE_OFF_VOL((base), (off))
#define __builtin_pulp_OffsetedReadHalf(base, off)    __READ_BASE_OFF_HALF_VOL((base), (off))
#define __builtin_pulp_OffsetedReadByte(base, off)    __READ_BASE_OFF_BYTE_VOL((base), (off))
#define __builtin_pulp_OffsetedWrite(x, base, off)      __WRITE_BASE_OFF_VOL((x), (base), (off))
#define __builtin_pulp_OffsetedWriteHalf(x, base, off)  __WRITE_BASE_OFF_HALF_VOL((x), (base), (off))
#define __builtin_pulp_OffsetedWriteByte(x, base, off)  __WRITE_BASE_OFF_BYTE_VOL((x), (base), (off))

/* ---- Complex --------------------------------------------------------- */

#ifdef ARCHI_CORE_HAS_CPLX
#define __builtin_pulp_cplxmuls(x, y)      __CPLXMULS((x), (y))
#define __builtin_pulp_cplxmulsdiv2(x, y)  __CPLXMULSDIV2((x), (y))
#define __builtin_pulp_cplxmulsdiv4(x, y)  __CPLXMULSDIV4((x), (y))
#define __builtin_pulp_cplx_conj(x)        __CPLXCONJ((x))
#define __builtin_pulp_sub2rotmj(x, y)     __SUB2ROTMJ((x), (y))
#define __builtin_pulp_add2div2(x, y)      __ADD2DIV2((x), (y))
#define __builtin_pulp_add2div4(x, y)      __ADD2DIV4((x), (y))
#define __builtin_pulp_sub2div2(x, y)      __SUB2DIV2((x), (y))
#define __builtin_pulp_sub2div4(x, y)      __SUB2DIV4((x), (y))
#define __builtin_pulp_add4div2(x, y)      __ADD4DIV2((x), (y))
#define __builtin_pulp_add4div4(x, y)      __ADD4DIV4((x), (y))
#define __builtin_pulp_sub4div2(x, y)      __SUB4DIV2((x), (y))
#define __builtin_pulp_sub4div4(x, y)      __SUB4DIV4((x), (y))
#endif

/*
 * Deliberately NOT defined, so a build that needs them fails loudly:
 *
 * - vitmax2 / vitsel2 -- Viterbi is xpulpv3, absent from CV32E40P.
 * - The 4-bit and 2-bit packed SIMD forms (sdotsp8/16, max8/16, min8/16,
 *   avgu8/16, ...) and the MacLoad family (mlinitspr_v3, mlsdotsp*_v3, ...).
 *   Those are XpulpNN; CV32E40P implements only 8- and 16-bit lanes and has no
 *   MacLoad SPRs.
 *
 * The XpulpNN names are referenced only by macro bodies in pulp-nn-mixed's
 * pulp_nn_utils.h that the XpulpV2/32bit sources never expand, and an
 * unexpanded macro body naming an undefined identifier is harmless.
 */

#endif /* ARCHI_CORE_HAS_COREV_V2 */

#endif
