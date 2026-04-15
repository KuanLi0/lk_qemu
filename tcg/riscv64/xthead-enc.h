#ifndef TCG_RISCV64_XTHEAD_ENC_H
#define TCG_RISCV64_XTHEAD_ENC_H

#include "qemu/osdep.h"

enum {
    XTHEAD_VLMUL_M1 = 0,
    XTHEAD_VSEW_8 = 0,
    XTHEAD_VSEW_16 = 1,
    XTHEAD_VSEW_32 = 2,
    XTHEAD_VSEW_64 = 3,
};

uint32_t xthead_encode_vtypei(bool vta, bool vma, unsigned vsew,
                              unsigned vlmul);
uint32_t xthead_encode_vsetvli(unsigned rd, unsigned rs1, uint32_t vtypei);
uint32_t xthead_encode_vv(uint32_t opc_fixed, unsigned vd, unsigned vs1,
                          unsigned vs2, bool vm);
uint32_t xthead_encode_vx(uint32_t opc_fixed, unsigned vd, unsigned rs1,
                          unsigned vs2, bool vm);
uint32_t xthead_encode_vi(uint32_t opc_fixed, unsigned vd, int32_t imm,
                          unsigned vs2, bool vm);
uint32_t xthead_encode_vle(uint32_t opc_fixed, unsigned vd, unsigned base,
                           int32_t imm, bool vm);
uint32_t xthead_encode_vse(uint32_t opc_fixed, unsigned vs3, unsigned base,
                           int32_t imm, bool vm);

uint32_t xthead_encode_vmv_v_v(unsigned vd, unsigned vs1, bool vm);
uint32_t xthead_encode_vmv_v_x(unsigned vd, unsigned rs1, bool vm);
uint32_t xthead_encode_vmv_v_i(unsigned vd, int32_t imm, bool vm);
uint32_t xthead_encode_vadd_vv(unsigned vd, unsigned vs1, unsigned vs2,
                               bool vm);
uint32_t xthead_encode_vadd_vi(unsigned vd, int32_t imm, unsigned vs2,
                               bool vm);
uint32_t xthead_encode_vsub_vv(unsigned vd, unsigned vs1, unsigned vs2,
                               bool vm);
uint32_t xthead_encode_vrsub_vi(unsigned vd, int32_t imm, unsigned vs2,
                                bool vm);
uint32_t xthead_encode_vand_vv(unsigned vd, unsigned vs1, unsigned vs2,
                               bool vm);
uint32_t xthead_encode_vand_vi(unsigned vd, int32_t imm, unsigned vs2,
                               bool vm);
uint32_t xthead_encode_vor_vv(unsigned vd, unsigned vs1, unsigned vs2,
                              bool vm);
uint32_t xthead_encode_vor_vi(unsigned vd, int32_t imm, unsigned vs2,
                              bool vm);
uint32_t xthead_encode_vxor_vv(unsigned vd, unsigned vs1, unsigned vs2,
                               bool vm);
uint32_t xthead_encode_vxor_vi(unsigned vd, int32_t imm, unsigned vs2,
                               bool vm);
uint32_t xthead_encode_vfadd_vv(unsigned vd, unsigned vs1, unsigned vs2,
                                bool vm);
uint32_t xthead_encode_vfsub_vv(unsigned vd, unsigned vs1, unsigned vs2,
                                bool vm);
uint32_t xthead_encode_vmseq_vv(unsigned vd, unsigned vs1, unsigned vs2,
                                bool vm);
uint32_t xthead_encode_vmseq_vi(unsigned vd, int32_t imm, unsigned vs2,
                                bool vm);
uint32_t xthead_encode_vmsne_vv(unsigned vd, unsigned vs1, unsigned vs2,
                                bool vm);
uint32_t xthead_encode_vmsne_vi(unsigned vd, int32_t imm, unsigned vs2,
                                bool vm);
uint32_t xthead_encode_vmslt_vv(unsigned vd, unsigned vs1, unsigned vs2,
                                bool vm);
uint32_t xthead_encode_vmsltu_vv(unsigned vd, unsigned vs1, unsigned vs2,
                                 bool vm);
uint32_t xthead_encode_vmsle_vv(unsigned vd, unsigned vs1, unsigned vs2,
                                bool vm);
uint32_t xthead_encode_vmsle_vi(unsigned vd, int32_t imm, unsigned vs2,
                                bool vm);
uint32_t xthead_encode_vmsleu_vv(unsigned vd, unsigned vs1, unsigned vs2,
                                 bool vm);
uint32_t xthead_encode_vmsleu_vi(unsigned vd, int32_t imm, unsigned vs2,
                                 bool vm);
uint32_t xthead_encode_vmsgt_vi(unsigned vd, int32_t imm, unsigned vs2,
                                bool vm);
uint32_t xthead_encode_vmsgtu_vi(unsigned vd, int32_t imm, unsigned vs2,
                                 bool vm);
uint32_t xthead_encode_vmerge_vim(unsigned vd, int32_t imm, unsigned vs2,
                                  bool vm);
uint32_t xthead_encode_vsll_vi(unsigned vd, int32_t imm, unsigned vs2,
                               bool vm);
uint32_t xthead_encode_vsll_vx(unsigned vd, unsigned rs1, unsigned vs2,
                               bool vm);
uint32_t xthead_encode_vsrl_vi(unsigned vd, int32_t imm, unsigned vs2,
                               bool vm);
uint32_t xthead_encode_vsrl_vx(unsigned vd, unsigned rs1, unsigned vs2,
                               bool vm);
uint32_t xthead_encode_vsra_vi(unsigned vd, int32_t imm, unsigned vs2,
                               bool vm);
uint32_t xthead_encode_vsra_vx(unsigned vd, unsigned rs1, unsigned vs2,
                               bool vm);
uint32_t xthead_encode_vlbu_v(unsigned vd, unsigned base, int32_t imm, bool vm);
uint32_t xthead_encode_vsb_v(unsigned vs3, unsigned base, int32_t imm, bool vm);
uint32_t xthead_encode_vle64(unsigned vd, unsigned base, int32_t imm, bool vm);
uint32_t xthead_encode_vse64(unsigned vs3, unsigned base, int32_t imm, bool vm);

#endif
