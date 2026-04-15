#include "qemu/osdep.h"
#include "tcg/riscv64/xthead-enc.h"

#define XTHEAD_V_OPCFG        (0x7u << 12)
#define XTHEAD_V_OPMVV        (0x2u << 12)
#define XTHEAD_V_OPIVI        (0x3u << 12)
#define XTHEAD_V_OPIVX        (0x4u << 12)
#define XTHEAD_V_OPFVV        (0x1u << 12)

#define XTHEAD_OPC_TH_VSETVLI   (0x57u | XTHEAD_V_OPCFG)
#define XTHEAD_OPC_TH_VLE64_V   (0x7007u)
#define XTHEAD_OPC_TH_VSE64_V   (0x7027u)
#define XTHEAD_OPC_TH_VMV_V_V   (0x5e000057u | XTHEAD_V_OPMVV)
#define XTHEAD_OPC_TH_VMV_V_I   (0x5e000057u | XTHEAD_V_OPIVI)
#define XTHEAD_OPC_TH_VMV_V_X   (0x5e000057u | XTHEAD_V_OPIVX)
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
