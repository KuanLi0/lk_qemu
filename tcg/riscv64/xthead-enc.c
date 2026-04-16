#include "qemu/osdep.h"
#include "tcg/riscv64/xthead-enc.h"

#define XTHEAD_V_OPCFG        (0x7u << 12)
#define XTHEAD_V_OPIVV        (0x0u << 12)
#define XTHEAD_V_OPMVV        (0x2u << 12)
#define XTHEAD_V_OPIVI        (0x3u << 12)
#define XTHEAD_V_OPIVX        (0x4u << 12)
#define XTHEAD_V_OPFVV        (0x1u << 12)

#define XTHEAD_OPC_TH_VSETVLI   (0x57u | XTHEAD_V_OPCFG)
#define XTHEAD_OPC_TH_VLE64_V   (0x7007u)
#define XTHEAD_OPC_TH_VSE64_V   (0x7027u)
#define XTHEAD_OPC_TH_VMV_V_V   (0x5e000057u | XTHEAD_V_OPIVV)
#define XTHEAD_OPC_TH_VMV_V_I   (0x5e000057u | XTHEAD_V_OPIVI)
#define XTHEAD_OPC_TH_VMV_V_X   (0x5e000057u | XTHEAD_V_OPIVX)
/*
 * XTheadVector scalar/lane move opcodes do not use the standard RVV encodings
 * used by vmv.v.{v,x,i}. Keep this family separate from the generic RVV bases.
 */
#define XTHEAD_OPC_TH_VMV_X_S   (0x32002057u)
#define XTHEAD_OPC_TH_VFMV_F_S  (0x32001057u)
#define XTHEAD_OPC_TH_VMV_S_X   (0x36006057u)
#define XTHEAD_OPC_TH_VFMV_S_F  (0x36005057u)
#define XTHEAD_OPC_TH_VSLIDEUP_VI  (0x38003057u)
#define XTHEAD_OPC_TH_VSLIDE1UP_VX (0x38006057u)
#define XTHEAD_OPC_TH_VMERGE_VIM (0x5c000057u | XTHEAD_V_OPIVI)
#define XTHEAD_OPC_TH_VADD_VV   (0x00000057u | XTHEAD_V_OPIVV)
#define XTHEAD_OPC_TH_VADD_VI   (0x00000057u | XTHEAD_V_OPIVI)
#define XTHEAD_OPC_TH_VSUB_VV   (0x08000057u | XTHEAD_V_OPIVV)
#define XTHEAD_OPC_TH_VRSUB_VI  (0x0c000057u | XTHEAD_V_OPIVI)
#define XTHEAD_OPC_TH_VMUL_VV   (0x94000057u | XTHEAD_V_OPMVV)
#define XTHEAD_OPC_TH_VSADDU_VV (0x80000057u | XTHEAD_V_OPIVV)
#define XTHEAD_OPC_TH_VSADDU_VI (0x80000057u | XTHEAD_V_OPIVI)
#define XTHEAD_OPC_TH_VSSUBU_VV (0x88000057u | XTHEAD_V_OPIVV)
#define XTHEAD_OPC_TH_VSSUBU_VI (0x88000057u | XTHEAD_V_OPIVI)
#define XTHEAD_OPC_TH_VSADD_VV  (0x84000057u | XTHEAD_V_OPIVV)
#define XTHEAD_OPC_TH_VSADD_VI  (0x84000057u | XTHEAD_V_OPIVI)
#define XTHEAD_OPC_TH_VSSUB_VV  (0x8c000057u | XTHEAD_V_OPIVV)
#define XTHEAD_OPC_TH_VSSUB_VI  (0x8c000057u | XTHEAD_V_OPIVI)
#define XTHEAD_OPC_TH_VMINU_VV  (0x10000057u | XTHEAD_V_OPIVV)
#define XTHEAD_OPC_TH_VMINU_VI  (0x10000057u | XTHEAD_V_OPIVI)
#define XTHEAD_OPC_TH_VMIN_VV   (0x14000057u | XTHEAD_V_OPIVV)
#define XTHEAD_OPC_TH_VMIN_VI   (0x14000057u | XTHEAD_V_OPIVI)
#define XTHEAD_OPC_TH_VMAXU_VV  (0x18000057u | XTHEAD_V_OPIVV)
#define XTHEAD_OPC_TH_VMAXU_VI  (0x18000057u | XTHEAD_V_OPIVI)
#define XTHEAD_OPC_TH_VMAX_VV   (0x1c000057u | XTHEAD_V_OPIVV)
#define XTHEAD_OPC_TH_VMAX_VI   (0x1c000057u | XTHEAD_V_OPIVI)
#define XTHEAD_OPC_TH_VAND_VV   (0x24000057u | XTHEAD_V_OPIVV)
#define XTHEAD_OPC_TH_VAND_VI   (0x24000057u | XTHEAD_V_OPIVI)
#define XTHEAD_OPC_TH_VOR_VV    (0x28000057u | XTHEAD_V_OPIVV)
#define XTHEAD_OPC_TH_VOR_VI    (0x28000057u | XTHEAD_V_OPIVI)
#define XTHEAD_OPC_TH_VXOR_VV   (0x2c000057u | XTHEAD_V_OPIVV)
#define XTHEAD_OPC_TH_VXOR_VI   (0x2c000057u | XTHEAD_V_OPIVI)
#define XTHEAD_OPC_TH_VMSEQ_VV  (0x60000057u | XTHEAD_V_OPIVV)
#define XTHEAD_OPC_TH_VMSEQ_VI  (0x60000057u | XTHEAD_V_OPIVI)
#define XTHEAD_OPC_TH_VMSNE_VV  (0x64000057u | XTHEAD_V_OPIVV)
#define XTHEAD_OPC_TH_VMSNE_VI  (0x64000057u | XTHEAD_V_OPIVI)
#define XTHEAD_OPC_TH_VMSLTU_VV (0x68000057u | XTHEAD_V_OPIVV)
#define XTHEAD_OPC_TH_VMSLT_VV  (0x6c000057u | XTHEAD_V_OPIVV)
#define XTHEAD_OPC_TH_VMSLEU_VV (0x70000057u | XTHEAD_V_OPIVV)
#define XTHEAD_OPC_TH_VMSLEU_VI (0x70000057u | XTHEAD_V_OPIVI)
#define XTHEAD_OPC_TH_VMSLE_VV  (0x74000057u | XTHEAD_V_OPIVV)
#define XTHEAD_OPC_TH_VMSLE_VI  (0x74000057u | XTHEAD_V_OPIVI)
#define XTHEAD_OPC_TH_VMSGTU_VI (0x78000057u | XTHEAD_V_OPIVI)
#define XTHEAD_OPC_TH_VMSGT_VI  (0x7c000057u | XTHEAD_V_OPIVI)
#define XTHEAD_OPC_TH_VSLL_VI   (0x94000057u | XTHEAD_V_OPIVI)
#define XTHEAD_OPC_TH_VSLL_VX   (0x94000057u | XTHEAD_V_OPIVX)
#define XTHEAD_OPC_TH_VSRL_VI   (0xa0000057u | XTHEAD_V_OPIVI)
#define XTHEAD_OPC_TH_VSRL_VX   (0xa0000057u | XTHEAD_V_OPIVX)
#define XTHEAD_OPC_TH_VSRA_VI   (0xa4000057u | XTHEAD_V_OPIVI)
#define XTHEAD_OPC_TH_VSRA_VX   (0xa4000057u | XTHEAD_V_OPIVX)
#define XTHEAD_OPC_TH_VFADD_VV  (0x00001057u)
#define XTHEAD_OPC_TH_VFSUB_VV  (0x08001057u)
#define XTHEAD_OPC_TH_VLBU_V    (0x00000007u)
#define XTHEAD_OPC_TH_VSB_V     (0x00000027u)

static inline uint32_t xthead_encode_reg5(unsigned reg, unsigned shift)
{
    return ((uint32_t)reg & 0x1fu) << shift;
}

uint32_t xthead_encode_vtypei(bool vta, bool vma, unsigned vsew,
                              unsigned vlmul)
{
    (void)vta;
    (void)vma;
    /*
     * XTheadVector follows the RVV 0.7.1 vtype layout:
     *   [6:5] vediv, [4:2] vsew, [1:0] vlmul.
     * Tail/mask policy are fixed to undisturbed, so there are no vta/vma bits.
     */
    return ((uint32_t)vsew << 2) | ((uint32_t)vlmul & 0x3u);
}

uint32_t xthead_encode_vsetvli(unsigned rd, unsigned rs1, uint32_t vtypei)
{
    return XTHEAD_OPC_TH_VSETVLI |
           xthead_encode_reg5(rd, 7) |
           xthead_encode_reg5(rs1, 15) |
           (((uint32_t)vtypei & 0x7ffu) << 20);
}

uint32_t xthead_encode_vv(uint32_t opc_fixed, unsigned vd, unsigned vs1,
                          unsigned vs2, bool vm)
{
    return opc_fixed |
           xthead_encode_reg5(vd, 7) |
           xthead_encode_reg5(vs1, 15) |
           xthead_encode_reg5(vs2, 20) |
           ((uint32_t)vm << 25);
}

uint32_t xthead_encode_vx(uint32_t opc_fixed, unsigned vd, unsigned rs1,
                          unsigned vs2, bool vm)
{
    return opc_fixed |
           xthead_encode_reg5(vd, 7) |
           xthead_encode_reg5(rs1, 15) |
           xthead_encode_reg5(vs2, 20) |
           ((uint32_t)vm << 25);
}

uint32_t xthead_encode_vi(uint32_t opc_fixed, unsigned vd, int32_t imm,
                          unsigned vs2, bool vm)
{
    return opc_fixed |
           xthead_encode_reg5(vd, 7) |
           xthead_encode_reg5((unsigned)imm, 15) |
           xthead_encode_reg5(vs2, 20) |
           ((uint32_t)vm << 25);
}

uint32_t xthead_encode_vle(uint32_t opc_fixed, unsigned vd, unsigned base,
                           int32_t imm, bool vm)
{
    g_assert(imm == 0);
    return opc_fixed |
           xthead_encode_reg5(vd, 7) |
           xthead_encode_reg5(base, 15) |
           ((uint32_t)vm << 25);
}

uint32_t xthead_encode_vse(uint32_t opc_fixed, unsigned vs3, unsigned base,
                           int32_t imm, bool vm)
{
    g_assert(imm == 0);
    return opc_fixed |
           xthead_encode_reg5(vs3, 7) |
           xthead_encode_reg5(base, 15) |
           ((uint32_t)vm << 25);
}

uint32_t xthead_encode_vmv_v_v(unsigned vd, unsigned vs1, bool vm)
{
    return xthead_encode_vv(XTHEAD_OPC_TH_VMV_V_V, vd, vs1, 0, vm);
}

uint32_t xthead_encode_vmv_v_x(unsigned vd, unsigned rs1, bool vm)
{
    return xthead_encode_vx(XTHEAD_OPC_TH_VMV_V_X, vd, rs1, 0, vm);
}

uint32_t xthead_encode_vmv_v_i(unsigned vd, int32_t imm, bool vm)
{
    return xthead_encode_vi(XTHEAD_OPC_TH_VMV_V_I, vd, imm, 0, vm);
}

uint32_t xthead_encode_vmv_s_x(unsigned vd, unsigned rs1)
{
    return XTHEAD_OPC_TH_VMV_S_X |
           xthead_encode_reg5(vd, 7) |
           xthead_encode_reg5(rs1, 15);
}

uint32_t xthead_encode_vmv_x_s(unsigned rd, unsigned vs2)
{
    return XTHEAD_OPC_TH_VMV_X_S |
           xthead_encode_reg5(rd, 7) |
           xthead_encode_reg5(vs2, 20);
}

uint32_t xthead_encode_vfmv_s_f(unsigned vd, unsigned rs1)
{
    return XTHEAD_OPC_TH_VFMV_S_F |
           xthead_encode_reg5(vd, 7) |
           xthead_encode_reg5(rs1, 15);
}

uint32_t xthead_encode_vfmv_f_s(unsigned rd, unsigned vs2)
{
    return XTHEAD_OPC_TH_VFMV_F_S |
           xthead_encode_reg5(rd, 7) |
           xthead_encode_reg5(vs2, 20);
}

uint32_t xthead_encode_vslideup_vi(unsigned vd, int32_t imm, unsigned vs2,
                                   bool vm)
{
    return xthead_encode_vi(XTHEAD_OPC_TH_VSLIDEUP_VI, vd, imm, vs2, vm);
}

uint32_t xthead_encode_vslide1up_vx(unsigned vd, unsigned rs1, unsigned vs2,
                                    bool vm)
{
    return xthead_encode_vx(XTHEAD_OPC_TH_VSLIDE1UP_VX, vd, rs1, vs2, vm);
}

uint32_t xthead_encode_vadd_vv(unsigned vd, unsigned vs1, unsigned vs2, bool vm)
{
    return xthead_encode_vv(XTHEAD_OPC_TH_VADD_VV, vd, vs1, vs2, vm);
}

uint32_t xthead_encode_vadd_vi(unsigned vd, int32_t imm, unsigned vs2, bool vm)
{
    return xthead_encode_vi(XTHEAD_OPC_TH_VADD_VI, vd, imm, vs2, vm);
}

uint32_t xthead_encode_vsub_vv(unsigned vd, unsigned vs1, unsigned vs2, bool vm)
{
    return xthead_encode_vv(XTHEAD_OPC_TH_VSUB_VV, vd, vs1, vs2, vm);
}

uint32_t xthead_encode_vrsub_vi(unsigned vd, int32_t imm, unsigned vs2, bool vm)
{
    return xthead_encode_vi(XTHEAD_OPC_TH_VRSUB_VI, vd, imm, vs2, vm);
}

uint32_t xthead_encode_vmul_vv(unsigned vd, unsigned vs1, unsigned vs2, bool vm)
{
    return xthead_encode_vv(XTHEAD_OPC_TH_VMUL_VV, vd, vs1, vs2, vm);
}

uint32_t xthead_encode_vsadd_vv(unsigned vd, unsigned vs1, unsigned vs2,
                                bool vm)
{
    return xthead_encode_vv(XTHEAD_OPC_TH_VSADD_VV, vd, vs1, vs2, vm);
}

uint32_t xthead_encode_vsadd_vi(unsigned vd, int32_t imm, unsigned vs2, bool vm)
{
    return xthead_encode_vi(XTHEAD_OPC_TH_VSADD_VI, vd, imm, vs2, vm);
}

uint32_t xthead_encode_vssub_vv(unsigned vd, unsigned vs1, unsigned vs2,
                                bool vm)
{
    return xthead_encode_vv(XTHEAD_OPC_TH_VSSUB_VV, vd, vs1, vs2, vm);
}

uint32_t xthead_encode_vssub_vi(unsigned vd, int32_t imm, unsigned vs2, bool vm)
{
    return xthead_encode_vi(XTHEAD_OPC_TH_VSSUB_VI, vd, imm, vs2, vm);
}

uint32_t xthead_encode_vsaddu_vv(unsigned vd, unsigned vs1, unsigned vs2,
                                 bool vm)
{
    return xthead_encode_vv(XTHEAD_OPC_TH_VSADDU_VV, vd, vs1, vs2, vm);
}

uint32_t xthead_encode_vsaddu_vi(unsigned vd, int32_t imm, unsigned vs2,
                                 bool vm)
{
    return xthead_encode_vi(XTHEAD_OPC_TH_VSADDU_VI, vd, imm, vs2, vm);
}

uint32_t xthead_encode_vssubu_vv(unsigned vd, unsigned vs1, unsigned vs2,
                                 bool vm)
{
    return xthead_encode_vv(XTHEAD_OPC_TH_VSSUBU_VV, vd, vs1, vs2, vm);
}

uint32_t xthead_encode_vssubu_vi(unsigned vd, int32_t imm, unsigned vs2,
                                 bool vm)
{
    return xthead_encode_vi(XTHEAD_OPC_TH_VSSUBU_VI, vd, imm, vs2, vm);
}

uint32_t xthead_encode_vmax_vv(unsigned vd, unsigned vs1, unsigned vs2, bool vm)
{
    return xthead_encode_vv(XTHEAD_OPC_TH_VMAX_VV, vd, vs1, vs2, vm);
}

uint32_t xthead_encode_vmax_vi(unsigned vd, int32_t imm, unsigned vs2, bool vm)
{
    return xthead_encode_vi(XTHEAD_OPC_TH_VMAX_VI, vd, imm, vs2, vm);
}

uint32_t xthead_encode_vmaxu_vv(unsigned vd, unsigned vs1, unsigned vs2,
                                bool vm)
{
    return xthead_encode_vv(XTHEAD_OPC_TH_VMAXU_VV, vd, vs1, vs2, vm);
}

uint32_t xthead_encode_vmaxu_vi(unsigned vd, int32_t imm, unsigned vs2,
                                bool vm)
{
    return xthead_encode_vi(XTHEAD_OPC_TH_VMAXU_VI, vd, imm, vs2, vm);
}

uint32_t xthead_encode_vmin_vv(unsigned vd, unsigned vs1, unsigned vs2, bool vm)
{
    return xthead_encode_vv(XTHEAD_OPC_TH_VMIN_VV, vd, vs1, vs2, vm);
}

uint32_t xthead_encode_vmin_vi(unsigned vd, int32_t imm, unsigned vs2, bool vm)
{
    return xthead_encode_vi(XTHEAD_OPC_TH_VMIN_VI, vd, imm, vs2, vm);
}

uint32_t xthead_encode_vminu_vv(unsigned vd, unsigned vs1, unsigned vs2,
                                bool vm)
{
    return xthead_encode_vv(XTHEAD_OPC_TH_VMINU_VV, vd, vs1, vs2, vm);
}

uint32_t xthead_encode_vminu_vi(unsigned vd, int32_t imm, unsigned vs2,
                                bool vm)
{
    return xthead_encode_vi(XTHEAD_OPC_TH_VMINU_VI, vd, imm, vs2, vm);
}

uint32_t xthead_encode_vand_vv(unsigned vd, unsigned vs1, unsigned vs2, bool vm)
{
    return xthead_encode_vv(XTHEAD_OPC_TH_VAND_VV, vd, vs1, vs2, vm);
}

uint32_t xthead_encode_vand_vi(unsigned vd, int32_t imm, unsigned vs2, bool vm)
{
    return xthead_encode_vi(XTHEAD_OPC_TH_VAND_VI, vd, imm, vs2, vm);
}

uint32_t xthead_encode_vor_vv(unsigned vd, unsigned vs1, unsigned vs2, bool vm)
{
    return xthead_encode_vv(XTHEAD_OPC_TH_VOR_VV, vd, vs1, vs2, vm);
}

uint32_t xthead_encode_vor_vi(unsigned vd, int32_t imm, unsigned vs2, bool vm)
{
    return xthead_encode_vi(XTHEAD_OPC_TH_VOR_VI, vd, imm, vs2, vm);
}

uint32_t xthead_encode_vxor_vv(unsigned vd, unsigned vs1, unsigned vs2, bool vm)
{
    return xthead_encode_vv(XTHEAD_OPC_TH_VXOR_VV, vd, vs1, vs2, vm);
}

uint32_t xthead_encode_vxor_vi(unsigned vd, int32_t imm, unsigned vs2, bool vm)
{
    return xthead_encode_vi(XTHEAD_OPC_TH_VXOR_VI, vd, imm, vs2, vm);
}

uint32_t xthead_encode_vfadd_vv(unsigned vd, unsigned vs1, unsigned vs2,
                                bool vm)
{
    return xthead_encode_vv(XTHEAD_OPC_TH_VFADD_VV, vd, vs1, vs2, vm);
}

uint32_t xthead_encode_vfsub_vv(unsigned vd, unsigned vs1, unsigned vs2,
                                bool vm)
{
    return xthead_encode_vv(XTHEAD_OPC_TH_VFSUB_VV, vd, vs1, vs2, vm);
}

uint32_t xthead_encode_vmseq_vv(unsigned vd, unsigned vs1, unsigned vs2,
                                bool vm)
{
    return xthead_encode_vv(XTHEAD_OPC_TH_VMSEQ_VV, vd, vs1, vs2, vm);
}

uint32_t xthead_encode_vmseq_vi(unsigned vd, int32_t imm, unsigned vs2,
                                bool vm)
{
    return xthead_encode_vi(XTHEAD_OPC_TH_VMSEQ_VI, vd, imm, vs2, vm);
}

uint32_t xthead_encode_vmsne_vv(unsigned vd, unsigned vs1, unsigned vs2,
                                bool vm)
{
    return xthead_encode_vv(XTHEAD_OPC_TH_VMSNE_VV, vd, vs1, vs2, vm);
}

uint32_t xthead_encode_vmsne_vi(unsigned vd, int32_t imm, unsigned vs2,
                                bool vm)
{
    return xthead_encode_vi(XTHEAD_OPC_TH_VMSNE_VI, vd, imm, vs2, vm);
}

uint32_t xthead_encode_vmslt_vv(unsigned vd, unsigned vs1, unsigned vs2,
                                bool vm)
{
    return xthead_encode_vv(XTHEAD_OPC_TH_VMSLT_VV, vd, vs1, vs2, vm);
}

uint32_t xthead_encode_vmsltu_vv(unsigned vd, unsigned vs1, unsigned vs2,
                                 bool vm)
{
    return xthead_encode_vv(XTHEAD_OPC_TH_VMSLTU_VV, vd, vs1, vs2, vm);
}

uint32_t xthead_encode_vmsle_vv(unsigned vd, unsigned vs1, unsigned vs2,
                                bool vm)
{
    return xthead_encode_vv(XTHEAD_OPC_TH_VMSLE_VV, vd, vs1, vs2, vm);
}

uint32_t xthead_encode_vmsle_vi(unsigned vd, int32_t imm, unsigned vs2,
                                bool vm)
{
    return xthead_encode_vi(XTHEAD_OPC_TH_VMSLE_VI, vd, imm, vs2, vm);
}

uint32_t xthead_encode_vmsleu_vv(unsigned vd, unsigned vs1, unsigned vs2,
                                 bool vm)
{
    return xthead_encode_vv(XTHEAD_OPC_TH_VMSLEU_VV, vd, vs1, vs2, vm);
}

uint32_t xthead_encode_vmsleu_vi(unsigned vd, int32_t imm, unsigned vs2,
                                 bool vm)
{
    return xthead_encode_vi(XTHEAD_OPC_TH_VMSLEU_VI, vd, imm, vs2, vm);
}

uint32_t xthead_encode_vmsgt_vi(unsigned vd, int32_t imm, unsigned vs2,
                                bool vm)
{
    return xthead_encode_vi(XTHEAD_OPC_TH_VMSGT_VI, vd, imm, vs2, vm);
}

uint32_t xthead_encode_vmsgtu_vi(unsigned vd, int32_t imm, unsigned vs2,
                                 bool vm)
{
    return xthead_encode_vi(XTHEAD_OPC_TH_VMSGTU_VI, vd, imm, vs2, vm);
}

uint32_t xthead_encode_vmerge_vim(unsigned vd, int32_t imm, unsigned vs2,
                                  bool vm)
{
    return xthead_encode_vi(XTHEAD_OPC_TH_VMERGE_VIM, vd, imm, vs2, vm);
}

uint32_t xthead_encode_vsll_vi(unsigned vd, int32_t imm, unsigned vs2, bool vm)
{
    return xthead_encode_vi(XTHEAD_OPC_TH_VSLL_VI, vd, imm, vs2, vm);
}

uint32_t xthead_encode_vsll_vx(unsigned vd, unsigned rs1, unsigned vs2, bool vm)
{
    return xthead_encode_vx(XTHEAD_OPC_TH_VSLL_VX, vd, rs1, vs2, vm);
}

uint32_t xthead_encode_vsrl_vi(unsigned vd, int32_t imm, unsigned vs2, bool vm)
{
    return xthead_encode_vi(XTHEAD_OPC_TH_VSRL_VI, vd, imm, vs2, vm);
}

uint32_t xthead_encode_vsrl_vx(unsigned vd, unsigned rs1, unsigned vs2, bool vm)
{
    return xthead_encode_vx(XTHEAD_OPC_TH_VSRL_VX, vd, rs1, vs2, vm);
}

uint32_t xthead_encode_vsra_vi(unsigned vd, int32_t imm, unsigned vs2, bool vm)
{
    return xthead_encode_vi(XTHEAD_OPC_TH_VSRA_VI, vd, imm, vs2, vm);
}

uint32_t xthead_encode_vsra_vx(unsigned vd, unsigned rs1, unsigned vs2, bool vm)
{
    return xthead_encode_vx(XTHEAD_OPC_TH_VSRA_VX, vd, rs1, vs2, vm);
}

uint32_t xthead_encode_vlbu_v(unsigned vd, unsigned base, int32_t imm, bool vm)
{
    return xthead_encode_vle(XTHEAD_OPC_TH_VLBU_V, vd, base, imm, vm);
}

uint32_t xthead_encode_vsb_v(unsigned vs3, unsigned base, int32_t imm, bool vm)
{
    return xthead_encode_vse(XTHEAD_OPC_TH_VSB_V, vs3, base, imm, vm);
}

uint32_t xthead_encode_vle64(unsigned vd, unsigned base, int32_t imm, bool vm)
{
    return xthead_encode_vle(XTHEAD_OPC_TH_VLE64_V, vd, base, imm, vm);
}

uint32_t xthead_encode_vse64(unsigned vs3, unsigned base, int32_t imm, bool vm)
{
    return xthead_encode_vse(XTHEAD_OPC_TH_VSE64_V, vs3, base, imm, vm);
}
