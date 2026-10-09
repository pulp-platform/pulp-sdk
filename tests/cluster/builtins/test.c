/*
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
 * Equivalence test for the xpulp/CoreV builtin macros.
 *
 * Every macro that archi/riscv/builtins_corev_v2.h maps onto a native CoreV
 * intrinsic is computed twice: once through the macro (which on the CV32E40P
 * target is the hardware instruction) and once through an independent pure-C
 * reference written out longhand below. Any mismatch is an error, so a wrong
 * intrinsic mapping -- wrong signedness, wrong immediate encoding, wrong
 * element width -- cannot pass silently.
 *
 * Builds and passes on every target: on RI5CY the macro side is the pulp-gcc
 * builtin, on CV32E40P it is the CoreV intrinsic, and the reference side is the
 * same C in both cases.
 */

#include "pmsis.h"
#include "stdio.h"

static int errors;

#define CHECK(what, got, exp)                                                  \
  do {                                                                         \
    int _g = (int)(got), _e = (int)(exp);                                       \
    if (_g != _e) {                                                            \
      errors++;                                                                \
      printf("FAIL %-14s got %11d  expected %11d\n", (what), _g, _e);          \
    }                                                                          \
  } while (0)

#define CHECK_V2(what, got, e0, e1)                                            \
  do {                                                                        \
    v2s _g = (got);                                                            \
    short _e0 = (short)(e0), _e1 = (short)(e1);                                \
    if (_g[0] != _e0 || _g[1] != _e1) {                                        \
      errors++;                                                               \
      printf("FAIL %-14s got {%d,%d}  expected {%d,%d}\n", (what),             \
             _g[0], _g[1], _e0, _e1);                                          \
    }                                                                         \
  } while (0)

#define CHECK_V4(what, got, e0, e1, e2, e3)                                    \
  do {                                                                        \
    v4s _g = (got);                                                            \
    signed char _e0=(signed char)(e0), _e1=(signed char)(e1);                  \
    signed char _e2=(signed char)(e2), _e3=(signed char)(e3);                  \
    if (_g[0]!=_e0 || _g[1]!=_e1 || _g[2]!=_e2 || _g[3]!=_e3) {                \
      errors++;                                                               \
      printf("FAIL %-14s got {%d,%d,%d,%d} expected {%d,%d,%d,%d}\n", (what),  \
             _g[0],_g[1],_g[2],_g[3], _e0,_e1,_e2,_e3);                        \
    }                                                                         \
  } while (0)

/* ---- independent pure-C references -----------------------------------
 * noinline keeps GCC from fusing these into the call site and pattern-matching
 * them into the very instruction under test, which would make the comparison
 * hardware-against-hardware and hide a bad mapping. */
#define REF __attribute__((noinline))

static REF int ref_dotp2(v2s a, v2s b)  { return a[0]*b[0] + a[1]*b[1]; }
static REF unsigned ref_dotpu2(v2u a, v2u b) { return (unsigned)a[0]*b[0] + (unsigned)a[1]*b[1]; }
static REF int ref_dotpus2(v2u a, v2s b){ return (int)a[0]*b[0] + (int)a[1]*b[1]; }

static REF int ref_dotp4(v4s a, v4s b)
{ return a[0]*b[0] + a[1]*b[1] + a[2]*b[2] + a[3]*b[3]; }
static REF unsigned ref_dotpu4(v4u a, v4u b)
{ return (unsigned)a[0]*b[0] + (unsigned)a[1]*b[1] + (unsigned)a[2]*b[2] + (unsigned)a[3]*b[3]; }
static REF int ref_dotpus4(v4u a, v4s b)
{ return (int)a[0]*b[0] + (int)a[1]*b[1] + (int)a[2]*b[2] + (int)a[3]*b[3]; }

static REF short ref_max(short p, short q) { return p > q ? p : q; }
static REF short ref_min(short p, short q) { return p < q ? p : q; }
static REF unsigned short ref_maxu(unsigned short p, unsigned short q) { return p > q ? p : q; }
static REF unsigned short ref_minu(unsigned short p, unsigned short q) { return p < q ? p : q; }

static REF int ref_clip(int x, int precision)
{
  int lo = -(1 << precision), hi = (1 << precision) - 1;
  return x < lo ? lo : (x > hi ? hi : x);
}
static REF unsigned ref_clipu(unsigned x, int precision)
{
  unsigned hi = (1u << precision) - 1u;
  /* cv.clipu's operand is signed: the ALU forces 0 whenever bit 31 is set
   * (cv32e40p_alu.sv, ALU_CLIPU: operand_a_i[31] -> '0), which is also what the
   * emulated __builtin_clipu does. Not an unsigned compare against hi. */
  if (x & 0x80000000u) return 0u;
  return x > hi ? hi : x;
}

/* position of the most significant set bit */
static REF int ref_fl1(unsigned x)
{
  int i;
  for (i = 31; i >= 0; i--) if (x & (1u << i)) return i;
  return 0;
}
/* number of redundant sign bits (pulp cv.clb semantics: leading-bit count - 1) */
static REF int ref_clb(unsigned x)
{
  int i, n = 0;
  unsigned top = x >> 31;
  if (x == 0) return 0;
  for (i = 30; i >= 0; i--) { if (((x >> i) & 1u) == top) n++; else break; }
  return n;
}
static REF int ref_ff1(unsigned x)
{
  int i;
  for (i = 0; i < 32; i++) if (x & (1u << i)) return i;
  return 32;
}
static REF unsigned ref_rotr(unsigned x) { return (x >> 1) | (x << 31); }

/* ---- test vectors: zero, ±1, saturating and boundary values ---------- */

static const short  h[] = { 0, 1, -1, 2, -2, 127, -128, 255, 32767, -32768, 12345, -12345 };
static const signed char b_[] = { 0, 1, -1, 2, -2, 63, -64, 127, -128, 100, -100, 7 };
static const int    w[] = { 0, 1, -1, 7, 8, -8, -9, 127, -128, 32767, -32768, 0x7fffffff, (int)0x80000000 };
static const unsigned u[] = { 0u, 1u, 2u, 0x80u, 0xffu, 0x8000u, 0xffffu, 0x80000000u, 0xffffffffu, 0x5a5a5a5au, 7u, 0x1000u };

#define NH (int)(sizeof(h)/sizeof(h[0]))
#define NB (int)(sizeof(b_)/sizeof(b_[0]))
#define NW (int)(sizeof(w)/sizeof(w[0]))
#define NU (int)(sizeof(u)/sizeof(u[0]))

static void test_simd16(void)
{
  for (int i = 0; i < NH; i++)
    for (int j = 0; j < NH; j++)
    {
      v2s a = (v2s){ h[i], h[j] };
      v2s c = (v2s){ h[j], h[i] };
      v2u au = (v2u){ (unsigned short)h[i], (unsigned short)h[j] };
      v2u cu = (v2u){ (unsigned short)h[j], (unsigned short)h[i] };

      CHECK("dotp2",     __builtin_dotp2(a, c),        ref_dotp2(a, c));
      CHECK("dotpu2",    __builtin_dotpu2(au, cu),     ref_dotpu2(au, cu));
      CHECK("dotpus2",   __builtin_dotpus2(au, c),     ref_dotpus2(au, c));
      CHECK("sumdotp2",  __builtin_sumdotp2(a, c, 7),  ref_dotp2(a, c) + 7);
      CHECK("sumdotpu2", __builtin_sumdotpu2(au, cu, 7), ref_dotpu2(au, cu) + 7);
      CHECK("sumdotpus2",__builtin_sumdotpus2(au, c, 7), ref_dotpus2(au, c) + 7);

      CHECK_V2("max2", __builtin_max2(a, c),
               ref_max(a[0], c[0]), ref_max(a[1], c[1]));
      CHECK_V2("min2", __builtin_min2(a, c),
               ref_min(a[0], c[0]), ref_min(a[1], c[1]));
      /* unsigned forms take the other comparison path in the ALU (cmp_signed),
       * so they need their own vectors -- h[] spans values whose sign bit is
       * set once reinterpreted as unsigned. */
      CHECK_V2("maxu2", (v2s)__builtin_maxu2(au, cu),
               (short)ref_maxu(au[0], cu[0]), (short)ref_maxu(au[1], cu[1]));
      CHECK_V2("minu2", (v2s)__builtin_minu2(au, cu),
               (short)ref_minu(au[0], cu[0]), (short)ref_minu(au[1], cu[1]));
      CHECK_V2("abs2", __builtin_abs2(a),
               a[0] < 0 ? -a[0] : a[0], a[1] < 0 ? -a[1] : a[1]);
      /* cv.pack puts rs1 in the HIGH half, so element order is the thing to
       * catch here: pack2(p,q) must give {p,q}, not {q,p}. */
      CHECK_V2("pack2",  __builtin_pack2(h[i], h[j]),  h[i], h[j]);
      CHECK_V2("packu2", (v2s)__builtin_packu2((unsigned short)h[i], (unsigned short)h[j]),
               h[i], h[j]);
    }
}

static void test_simd8(void)
{
  for (int i = 0; i < NB; i++)
    for (int j = 0; j < NB; j++)
    {
      v4s a = (v4s){ b_[i], b_[j], b_[(i+1)%NB], b_[(j+2)%NB] };
      v4s c = (v4s){ b_[j], b_[i], b_[(j+3)%NB], b_[(i+1)%NB] };
      v4u au = (v4u){ (unsigned char)a[0], (unsigned char)a[1],
                      (unsigned char)a[2], (unsigned char)a[3] };
      v4u cu = (v4u){ (unsigned char)c[0], (unsigned char)c[1],
                      (unsigned char)c[2], (unsigned char)c[3] };

      CHECK("dotp4",     __builtin_dotp4(a, c),          ref_dotp4(a, c));
      CHECK("dotpu4",    __builtin_dotpu4(au, cu),       ref_dotpu4(au, cu));
      CHECK("dotpus4",   __builtin_dotpus4(au, c),       ref_dotpus4(au, c));
      CHECK("sumdotp4",  __builtin_sumdotp4(a, c, -3),   ref_dotp4(a, c) - 3);
      CHECK("sumdotpu4", __builtin_sumdotpu4(au, cu, 5), ref_dotpu4(au, cu) + 5);
      CHECK("sumdotpus4",__builtin_sumdotpus4(au, c, 5), ref_dotpus4(au, c) + 5);

      CHECK_V4("max4", __builtin_max4(a, c),
               a[0]>c[0]?a[0]:c[0], a[1]>c[1]?a[1]:c[1],
               a[2]>c[2]?a[2]:c[2], a[3]>c[3]?a[3]:c[3]);
      CHECK_V4("min4", __builtin_min4(a, c),
               a[0]<c[0]?a[0]:c[0], a[1]<c[1]?a[1]:c[1],
               a[2]<c[2]?a[2]:c[2], a[3]<c[3]?a[3]:c[3]);
      CHECK_V4("maxu4", (v4s)__builtin_maxu4(au, cu),
               au[0]>cu[0]?au[0]:cu[0], au[1]>cu[1]?au[1]:cu[1],
               au[2]>cu[2]?au[2]:cu[2], au[3]>cu[3]?au[3]:cu[3]);
      CHECK_V4("minu4", (v4s)__builtin_minu4(au, cu),
               au[0]<cu[0]?au[0]:cu[0], au[1]<cu[1]?au[1]:cu[1],
               au[2]<cu[2]?au[2]:cu[2], au[3]<cu[3]?au[3]:cu[3]);
      CHECK_V4("abs4", __builtin_abs4(a),
               a[0]<0?-a[0]:a[0], a[1]<0?-a[1]:a[1],
               a[2]<0?-a[2]:a[2], a[3]<0?-a[3]:a[3]);
      CHECK_V4("pack4", __builtin_pack4(a[0], a[1], a[2], a[3]),
               a[0], a[1], a[2], a[3]);
      CHECK_V4("packu4", (v4s)__builtin_packu4((unsigned char)a[0], (unsigned char)a[1],
                                               (unsigned char)a[2], (unsigned char)a[3]),
               a[0], a[1], a[2], a[3]);
    }
}

static void test_scalar(void)
{
  /* clip: the immediate encoding is the easiest thing to get wrong, so sweep
   * every precision against the longhand bound. */
  for (int i = 0; i < NW; i++)
  {
    CHECK("clip p3",  __builtin_clip(w[i], 3),  ref_clip(w[i], 3));
    CHECK("clip p7",  __builtin_clip(w[i], 7),  ref_clip(w[i], 7));
    CHECK("clip p15", __builtin_clip(w[i], 15), ref_clip(w[i], 15));
  }
  for (int i = 0; i < NU; i++)
  {
    CHECK("clipu p3",  __builtin_clipu(u[i], 3),  ref_clipu(u[i], 3));
    CHECK("clipu p7",  __builtin_clipu(u[i], 7),  ref_clipu(u[i], 7));
    CHECK("clipu p15", __builtin_clipu(u[i], 15), ref_clipu(u[i], 15));
  }

  for (int i = 0; i < NU; i++)
  {
    if (u[i]) CHECK("fl1", __builtin_fl1(u[i]), ref_fl1(u[i]));
    CHECK("rotr", __builtin_rotr(u[i]), ref_rotr(u[i]));
    CHECK("__FF1", __FF1(u[i]), ref_ff1(u[i]));
    if (u[i]) CHECK("clb", __builtin_clb(u[i]), ref_clb(u[i]));
  }

  for (int i = 0; i < NW; i++)
    for (int j = 0; j < NW; j++)
    {
      CHECK("__MAX", __MAX(w[i], w[j]), w[i] > w[j] ? w[i] : w[j]);
    }
}

static void cluster_entry(void *arg)
{
  test_simd16();
  test_simd8();
  test_scalar();
}

int test_entry()
{
  struct pi_device cluster_dev;
  struct pi_cluster_conf conf;
  struct pi_cluster_task task;

  printf("Starting builtin equivalence test\n");

  pi_cluster_conf_init(&conf);
  conf.id = 0;
  pi_open_from_conf(&cluster_dev, &conf);
  if (pi_cluster_open(&cluster_dev))
    return -1;

  pi_cluster_send_task_to_cl(&cluster_dev,
                             pi_cluster_task(&task, cluster_entry, NULL));
  pi_cluster_close(&cluster_dev);

  if (errors)
    printf("Test failure (%d mismatches)\n", errors);
  else
    printf("Test success\n");

  return errors;
}

void test_kickoff(void *arg)
{
  pmsis_exit(test_entry());
}

int main()
{
  return pmsis_kickoff((void *)test_kickoff);
}
