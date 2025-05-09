/*
 * This file is extracted from the implementation of the FFT on Arm64/Neon
 * available in https://github.com/cothan/Falcon-Arm (neon/macrof.h).
 * =============================================================================
 * Copyright (c) 2022 by Cryptographic Engineering Research Group (CERG)
 * ECE Department, George Mason University
 * Fairfax, VA, U.S.A.
 * @author: Duc Tri Nguyen dnguye69@gmu.edu, cothannguyen@gmail.com
 * Licensed under the Apache License, Version 2.0 (the "License");
 * =============================================================================
 *
 * This 64-bit Floating point NEON macro x1 has not been modified and is provided as is.
 */

#ifndef MACROF_H
#define MACROF_H

#include <arm_neon.h>

// c <= addr x1
#define vload(c, addr) c = vld1q_f64(addr);
// c <= addr interleave 2
#define vload2(c, addr) c = vld2q_f64(addr);
// c <= addr interleave 4
#define vload4(c, addr) c = vld4q_f64(addr);

#define vstore(addr, c) vst1q_f64(addr, c);
// addr <= c
#define vstore2(addr, c) vst2q_f64(addr, c);
// addr <= c
#define vstore4(addr, c) vst4q_f64(addr, c);

// c <= addr x2
#define vloadx2(c, addr) c = vld1q_f64_x2(addr);
// c <= addr x3
#define vloadx3(c, addr) c = vld1q_f64_x3(addr);

// addr <= c
#define vstorex2(addr, c) vst1q_f64_x2(addr, c);

// c = a - b
#define vfsub(c, a, b) c = vsubq_f64(a, b);

// c = a + b
#define vfadd(c, a, b) c = vaddq_f64(a, b);

// c = a * b
#define vfmul(c, a, b) c = vmulq_f64(a, b);

// c = a * n (n is constant)
#define vfmuln(c, a, n) c = vmulq_n_f64(a, n);

// Swap from a|b to b|a
#define vswap(c, a) c = vextq_f64(a, a, 1);

// c = a * b[i]
#define vfmul_lane(c, a, b, i) c = vmulq_laneq_f64(a, b, i);

// c = 1/a
#define vfinv(c, a) c = vdivq_f64(vdupq_n_f64(1.0), a);

// c = -a
#define vfneg(c, a) c = vnegq_f64(a);

#define transpose_f64(a, b, t, ia, ib, it)      \
  t.val[it] = a.val[ia];                        \
  a.val[ia] = vzip1q_f64(a.val[ia], b.val[ib]); \
  b.val[ib] = vzip2q_f64(t.val[it], b.val[ib]);

/*
 * c = a + jb
 * c[0] = a[0] - b[1]
 * c[1] = a[1] + b[0]
 */
#define vfcaddj(c, a, b) c = vcaddq_rot90_f64(a, b);

/*
 * c = a - jb
 * c[0] = a[0] + b[1]
 * c[1] = a[1] - b[0]
 */
#define vfcsubj(c, a, b) c = vcaddq_rot270_f64(a, b);

// c[0] = c[0] + b[0]*a[0], c[1] = c[1] + b[1]*a[0]
#define vfcmla(c, a, b) c = vcmlaq_f64(c, a, b);

// c[0] = c[0] - b[1]*a[1], c[1] = c[1] + b[0]*a[1]
#define vfcmla_90(c, a, b) c = vcmlaq_rot90_f64(c, a, b);

// c[0] = c[0] - b[0]*a[0], c[1] = c[1] - b[1]*a[0]
#define vfcmla_180(c, a, b) c = vcmlaq_rot180_f64(c, a, b);

// c[0] = c[0] + b[1]*a[1], c[1] = c[1] - b[0]*a[1]
#define vfcmla_270(c, a, b) c = vcmlaq_rot270_f64(c, a, b);

/*
 * Complex MUL: c = a*b
 * c[0] = a[0]*b[0] - a[1]*b[1]
 * c[1] = a[0]*b[1] + a[1]*b[0]
 */
#define FPC_CMUL(c, a, b)       \
  c = vmulq_laneq_f64(b, a, 0); \
  c = vcmlaq_rot90_f64(c, a, b);

/*
 * Complex MUL: c = a * conjugate(b) = a * (b[0], -b[1])
 * c[0] =   b[0]*a[0] + b[1]*a[1]
 * c[1] = + b[0]*a[1] - b[1]*a[0]
 */
#define FPC_CMUL_CONJ(c, a, b)  \
  c = vmulq_laneq_f64(a, b, 0); \
  c = vcmlaq_rot270_f64(c, b, a);

#if FMA == 1
// d = c + a *b
#define vfmla(d, c, a, b) d = vfmaq_f64(c, a, b);
// d = c - a * b
#define vfmls(d, c, a, b) d = vfmsq_f64(c, a, b);
// d = c + a * b[i]
#define vfmla_lane(d, c, a, b, i) d = vfmaq_laneq_f64(c, a, b, i);
// d = c - a * b[i]
#define vfmls_lane(d, c, a, b, i) d = vfmsq_laneq_f64(c, a, b, i);

#else
// d = c + a *b
#define vfmla(d, c, a, b) d = vaddq_f64(c, vmulq_f64(a, b));
// d = c - a *b
#define vfmls(d, c, a, b) d = vsubq_f64(c, vmulq_f64(a, b));
// d = c + a * b[i]
#define vfmla_lane(d, c, a, b, i) d = vaddq_f64(c, vmulq_laneq_f64(a, b, i));

#define vfmls_lane(d, c, a, b, i) d = vsubq_f64(c, vmulq_laneq_f64(a, b, i));

#endif

#endif
