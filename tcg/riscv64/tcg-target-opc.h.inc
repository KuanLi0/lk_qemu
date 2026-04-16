/*
 * Copyright (c) C-SKY Microsystems Co., Ltd.
 *
 * This work is licensed under the terms of the GNU GPL, version 2 or
 * (at your option) any later version.
 *
 * See the COPYING file in the top-level directory for details.
 *
 * Target-specific opcodes for host vector expansion.  These will be
 * emitted by tcg_expand_vec_op.  For those familiar with GCC internals,
 * consider these to be UNSPEC with names.
 */
DEF(xthead_vfaddsd_vec, 1, 2, 0, TCG_OPF_VECTOR)
DEF(xthead_vfsubsd_vec, 1, 2, 0, TCG_OPF_VECTOR)
DEF(xthead_cvtps2pd_vec, 1, 1, 0, TCG_OPF_VECTOR)
DEF(xthead_cvtpd2ps_vec, 1, 2, 0, TCG_OPF_VECTOR)
DEF(xthead_cvtss2sd_vec, 1, 2, 0, TCG_OPF_VECTOR)
DEF(xthead_cvtsd2ss_vec, 1, 3, 0, TCG_OPF_VECTOR)
DEF(xthead_cvtsi2sd_vec, 1, 3, 1, TCG_OPF_VECTOR)
DEF(xthead_roundss_vec, 1, 3, 0, TCG_OPF_VECTOR)
