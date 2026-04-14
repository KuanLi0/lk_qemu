#include "qemu/osdep.h"
#include "tcg/riscv64/xthead-enc.h"

#define XTHEAD_V_OPCFG        (0x7u << 12)
#define XTHEAD_V_OPFVV        (0x1u << 12)
#define XTHEAD_V_UNIT_STRIDE  (0x0u << 20)

#define XTHEAD_OPC_TH_VSETVLI   (0x57u | XTHEAD_V_OPCFG)
#define XTHEAD_OPC_TH_VLE64_V   (0x7007u | XTHEAD_V_UNIT_STRIDE)
#define XTHEAD_OPC_TH_VSE64_V   (0x7027u | XTHEAD_V_UNIT_STRIDE)
#define XTHEAD_OPC_TH_VFADD_VV  (0x00001057u)
#define XTHEAD_OPC_TH_VFSUB_VV  (0x08001057u)

static inline uint32_t xthead_encode_imm12(int32_t imm)
{
    return ((uint32_t)imm & 0xfffu) << 20;
}

static inline uint32_t xthead_encode_reg5(unsigned reg, unsigned shift)
{
    return ((uint32_t)reg & 0x1fu) << shift;
}

static inline uint32_t xthead_encode_simm12(int32_t imm)
{
    uint32_t uimm = (uint32_t)imm & 0xfffu;

    return ((uimm & 0xfe0u) << 20) | ((uimm & 0x1fu) << 7);
}

uint32_t xthead_encode_vtypei(bool vta, bool vma, unsigned vsew,
                              unsigned vlmul)
{
    return ((uint32_t)vma << 7) | ((uint32_t)vta << 6) |
           ((uint32_t)vsew << 3) | ((uint32_t)vlmul & 0x7u);
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

uint32_t xthead_encode_vle(uint32_t opc_fixed, unsigned vd, unsigned base,
                           int32_t imm, bool vm)
{
    return opc_fixed |
           xthead_encode_reg5(vd, 7) |
           xthead_encode_reg5(base, 15) |
           xthead_encode_imm12(imm) |
           ((uint32_t)vm << 25);
}

uint32_t xthead_encode_vse(uint32_t opc_fixed, unsigned vs3, unsigned base,
                           int32_t imm, bool vm)
{
    return opc_fixed |
           xthead_encode_reg5(base, 15) |
           xthead_encode_reg5(vs3, 20) |
           xthead_encode_simm12(imm) |
           ((uint32_t)vm << 25);
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

uint32_t xthead_encode_vle64(unsigned vd, unsigned base, int32_t imm, bool vm)
{
    return xthead_encode_vle(XTHEAD_OPC_TH_VLE64_V, vd, base, imm, vm);
}

uint32_t xthead_encode_vse64(unsigned vs3, unsigned base, int32_t imm, bool vm)
{
    return xthead_encode_vse(XTHEAD_OPC_TH_VSE64_V, vs3, base, imm, vm);
}
