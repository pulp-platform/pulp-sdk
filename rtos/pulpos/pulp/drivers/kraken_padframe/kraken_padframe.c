#include "archi/chips/kraken/kraken_padframe.h"
#define  KRAKEN_PADFRAME_AON_CONFIG0_BASE_ADDR KRAKEN_PADFRAME_BASE_ADDRESS
#include "archi/chips/kraken/kraken_padframe_aon_regs.h"

#define REG_WRITE32(addr, value) *((volatile uint32_t*) addr) = (uint32_t) value;
#define REG_READ32(addr) *((volatile uint32_t*) addr)
#define REG_SET(FIELD, v) ((v & FIELD##_MASK) << FIELD##_LSB)
#define REG_CLR(FIELD)    ((FIELD##_MASK) << FIELD##_LSB)
#define REG_GET(FIELD, v) ((v >> FIELD##_LSB) & FIELD##_MASK)


void kraken_padframe_aon_pad_gpioa_cfg_chip2pad_set(uint32_t idx, uint8_t value) {
  uint32_t address = KRAKEN_PADFRAME_AON_CONFIG_PAD_GPIOA_CFG_0(0) + 4*idx;
  uint32_t v = REG_READ32(address);
  v &= ~REG_CLR(KRAKEN_PADFRAME_AON_CONFIG_PAD_GPIOA_CFG_0_CHIP2PAD_0);
  v |= REG_SET(KRAKEN_PADFRAME_AON_CONFIG_PAD_GPIOA_CFG_0_CHIP2PAD_0, value);
  REG_WRITE32(address, v);
}

uint8_t kraken_padframe_aon_pad_gpioa_cfg_chip2pad_get(uint32_t idx) {
  uint32_t address = KRAKEN_PADFRAME_AON_CONFIG_PAD_GPIOA_CFG_0(0) + 4*idx;
  return REG_GET(KRAKEN_PADFRAME_AON_CONFIG_PAD_GPIOA_CFG_0_CHIP2PAD_0, REG_READ32(address));
}

void kraken_padframe_aon_pad_gpioa_cfg_rxe_set(uint32_t idx, uint8_t value) {
  uint32_t address = KRAKEN_PADFRAME_AON_CONFIG_PAD_GPIOA_CFG_0(0) + 4*idx;
  uint32_t v = REG_READ32(address);
  v &= ~REG_CLR(KRAKEN_PADFRAME_AON_CONFIG_PAD_GPIOA_CFG_0_RXE_0);
  v |= REG_SET(KRAKEN_PADFRAME_AON_CONFIG_PAD_GPIOA_CFG_0_RXE_0, value);
  REG_WRITE32(address, v);
}

uint8_t kraken_padframe_aon_pad_gpioa_cfg_rxe_get(uint32_t idx) {
  uint32_t address = KRAKEN_PADFRAME_AON_CONFIG_PAD_GPIOA_CFG_0(0) + 4*idx;
  return REG_GET(KRAKEN_PADFRAME_AON_CONFIG_PAD_GPIOA_CFG_0_RXE_0, REG_READ32(address));
}

void kraken_padframe_aon_pad_gpioa_cfg_trie_set(uint32_t idx, uint8_t value) {
  uint32_t address = KRAKEN_PADFRAME_AON_CONFIG_PAD_GPIOA_CFG_0(0) + 4*idx;
  uint32_t v = REG_READ32(address);
  v &= ~REG_CLR(KRAKEN_PADFRAME_AON_CONFIG_PAD_GPIOA_CFG_0_TRIE_0);
  v |= REG_SET(KRAKEN_PADFRAME_AON_CONFIG_PAD_GPIOA_CFG_0_TRIE_0, value);
  REG_WRITE32(address, v);
}

uint8_t kraken_padframe_aon_pad_gpioa_cfg_trie_get(uint32_t idx) {
  uint32_t address = KRAKEN_PADFRAME_AON_CONFIG_PAD_GPIOA_CFG_0(0) + 4*idx;
  return REG_GET(KRAKEN_PADFRAME_AON_CONFIG_PAD_GPIOA_CFG_0_TRIE_0, REG_READ32(address));
}

void kraken_padframe_aon_pad_gpioa_cfg_pue_set(uint32_t idx, uint8_t value) {
  uint32_t address = KRAKEN_PADFRAME_AON_CONFIG_PAD_GPIOA_CFG_0(0) + 4*idx;
  uint32_t v = REG_READ32(address);
  v &= ~REG_CLR(KRAKEN_PADFRAME_AON_CONFIG_PAD_GPIOA_CFG_0_PUE_0);
  v |= REG_SET(KRAKEN_PADFRAME_AON_CONFIG_PAD_GPIOA_CFG_0_PUE_0, value);
  REG_WRITE32(address, v);
}

uint8_t kraken_padframe_aon_pad_gpioa_cfg_pue_get(uint32_t idx) {
  uint32_t address = KRAKEN_PADFRAME_AON_CONFIG_PAD_GPIOA_CFG_0(0) + 4*idx;
  return REG_GET(KRAKEN_PADFRAME_AON_CONFIG_PAD_GPIOA_CFG_0_PUE_0, REG_READ32(address));
}

void kraken_padframe_aon_pad_gpioa_cfg_pde_set(uint32_t idx, uint8_t value) {
  uint32_t address = KRAKEN_PADFRAME_AON_CONFIG_PAD_GPIOA_CFG_0(0) + 4*idx;
  uint32_t v = REG_READ32(address);
  v &= ~REG_CLR(KRAKEN_PADFRAME_AON_CONFIG_PAD_GPIOA_CFG_0_PDE_0);
  v |= REG_SET(KRAKEN_PADFRAME_AON_CONFIG_PAD_GPIOA_CFG_0_PDE_0, value);
  REG_WRITE32(address, v);
}

uint8_t kraken_padframe_aon_pad_gpioa_cfg_pde_get(uint32_t idx) {
  uint32_t address = KRAKEN_PADFRAME_AON_CONFIG_PAD_GPIOA_CFG_0(0) + 4*idx;
  return REG_GET(KRAKEN_PADFRAME_AON_CONFIG_PAD_GPIOA_CFG_0_PDE_0, REG_READ32(address));
}

void kraken_padframe_aon_pad_gpioa_cfg_drv_set(uint32_t idx, uint8_t value) {
  uint32_t address = KRAKEN_PADFRAME_AON_CONFIG_PAD_GPIOA_CFG_0(0) + 4*idx;
  uint32_t v = REG_READ32(address);
  v &= ~REG_CLR(KRAKEN_PADFRAME_AON_CONFIG_PAD_GPIOA_CFG_0_DRV_0);
  v |= REG_SET(KRAKEN_PADFRAME_AON_CONFIG_PAD_GPIOA_CFG_0_DRV_0, value);
  REG_WRITE32(address, v);
}

uint8_t kraken_padframe_aon_pad_gpioa_cfg_drv_get(uint32_t idx) {
  uint32_t address = KRAKEN_PADFRAME_AON_CONFIG_PAD_GPIOA_CFG_0(0) + 4*idx;
  return REG_GET(KRAKEN_PADFRAME_AON_CONFIG_PAD_GPIOA_CFG_0_DRV_0, REG_READ32(address));
}

void kraken_padframe_aon_pad_gpioa_cfg_slw_set(uint32_t idx, uint8_t value) {
  uint32_t address = KRAKEN_PADFRAME_AON_CONFIG_PAD_GPIOA_CFG_0(0) + 4*idx;
  uint32_t v = REG_READ32(address);
  v &= ~REG_CLR(KRAKEN_PADFRAME_AON_CONFIG_PAD_GPIOA_CFG_0_SLW_0);
  v |= REG_SET(KRAKEN_PADFRAME_AON_CONFIG_PAD_GPIOA_CFG_0_SLW_0, value);
  REG_WRITE32(address, v);
}

uint8_t kraken_padframe_aon_pad_gpioa_cfg_slw_get(uint32_t idx) {
  uint32_t address = KRAKEN_PADFRAME_AON_CONFIG_PAD_GPIOA_CFG_0(0) + 4*idx;
  return REG_GET(KRAKEN_PADFRAME_AON_CONFIG_PAD_GPIOA_CFG_0_SLW_0, REG_READ32(address));
}

void kraken_padframe_aon_pad_gpioa_cfg_smt_set(uint32_t idx, uint8_t value) {
  uint32_t address = KRAKEN_PADFRAME_AON_CONFIG_PAD_GPIOA_CFG_0(0) + 4*idx;
  uint32_t v = REG_READ32(address);
  v &= ~REG_CLR(KRAKEN_PADFRAME_AON_CONFIG_PAD_GPIOA_CFG_0_SMT_0);
  v |= REG_SET(KRAKEN_PADFRAME_AON_CONFIG_PAD_GPIOA_CFG_0_SMT_0, value);
  REG_WRITE32(address, v);
}

uint8_t kraken_padframe_aon_pad_gpioa_cfg_smt_get(uint32_t idx) {
  uint32_t address = KRAKEN_PADFRAME_AON_CONFIG_PAD_GPIOA_CFG_0(0) + 4*idx;
  return REG_GET(KRAKEN_PADFRAME_AON_CONFIG_PAD_GPIOA_CFG_0_SMT_0, REG_READ32(address));
}

void kraken_padframe_aon_pad_gpioa_mux_set(uint32_t idx, kraken_padframe_aon_pad_gpioa_mux_sel_t mux_sel) {
  const uint32_t pads_per_register = 4;
  const uint32_t address = KRAKEN_PADFRAME_AON_CONFIG_PAD_GPIOA_MUX_SEL_0(0)+idx/pads_per_register*4;
  const uint32_t sel_size = 7;
  uint32_t field_offset = idx % pads_per_register * sel_size;
  uint32_t field_mask = ((1 << (7)) - 1);
  uint32_t v = REG_READ32(address);
  v &= ~(field_mask << field_offset);
  v |= (mux_sel & field_mask) << field_offset;
  REG_WRITE32(address, v);
}

kraken_padframe_aon_pad_gpioa_mux_sel_t kraken_padframe_aon_pad_gpioa_mux_get(uint32_t idx) {
  const uint32_t pads_per_register = 4;
  const uint32_t address = KRAKEN_PADFRAME_AON_CONFIG_PAD_GPIOA_MUX_SEL_0(0)+idx/pads_per_register*4;
  const uint32_t sel_size = 7;
  uint32_t field_offset = idx % pads_per_register * sel_size;
  uint32_t field_mask = ((1 << 7) - 1) <<sel_size-1;
  return (REG_READ32(address) >> field_offset) & field_mask;
}

void kraken_padframe_aon_pad_gpiob_cfg_chip2pad_set(uint32_t idx, uint8_t value) {
  uint32_t address = KRAKEN_PADFRAME_AON_CONFIG_PAD_GPIOB_CFG_0(0) + 4*idx;
  uint32_t v = REG_READ32(address);
  v &= ~REG_CLR(KRAKEN_PADFRAME_AON_CONFIG_PAD_GPIOB_CFG_0_CHIP2PAD_0);
  v |= REG_SET(KRAKEN_PADFRAME_AON_CONFIG_PAD_GPIOB_CFG_0_CHIP2PAD_0, value);
  REG_WRITE32(address, v);
}

uint8_t kraken_padframe_aon_pad_gpiob_cfg_chip2pad_get(uint32_t idx) {
  uint32_t address = KRAKEN_PADFRAME_AON_CONFIG_PAD_GPIOB_CFG_0(0) + 4*idx;
  return REG_GET(KRAKEN_PADFRAME_AON_CONFIG_PAD_GPIOB_CFG_0_CHIP2PAD_0, REG_READ32(address));
}

void kraken_padframe_aon_pad_gpiob_cfg_rxe_set(uint32_t idx, uint8_t value) {
  uint32_t address = KRAKEN_PADFRAME_AON_CONFIG_PAD_GPIOB_CFG_0(0) + 4*idx;
  uint32_t v = REG_READ32(address);
  v &= ~REG_CLR(KRAKEN_PADFRAME_AON_CONFIG_PAD_GPIOB_CFG_0_RXE_0);
  v |= REG_SET(KRAKEN_PADFRAME_AON_CONFIG_PAD_GPIOB_CFG_0_RXE_0, value);
  REG_WRITE32(address, v);
}

uint8_t kraken_padframe_aon_pad_gpiob_cfg_rxe_get(uint32_t idx) {
  uint32_t address = KRAKEN_PADFRAME_AON_CONFIG_PAD_GPIOB_CFG_0(0) + 4*idx;
  return REG_GET(KRAKEN_PADFRAME_AON_CONFIG_PAD_GPIOB_CFG_0_RXE_0, REG_READ32(address));
}

void kraken_padframe_aon_pad_gpiob_cfg_trie_set(uint32_t idx, uint8_t value) {
  uint32_t address = KRAKEN_PADFRAME_AON_CONFIG_PAD_GPIOB_CFG_0(0) + 4*idx;
  uint32_t v = REG_READ32(address);
  v &= ~REG_CLR(KRAKEN_PADFRAME_AON_CONFIG_PAD_GPIOB_CFG_0_TRIE_0);
  v |= REG_SET(KRAKEN_PADFRAME_AON_CONFIG_PAD_GPIOB_CFG_0_TRIE_0, value);
  REG_WRITE32(address, v);
}

uint8_t kraken_padframe_aon_pad_gpiob_cfg_trie_get(uint32_t idx) {
  uint32_t address = KRAKEN_PADFRAME_AON_CONFIG_PAD_GPIOB_CFG_0(0) + 4*idx;
  return REG_GET(KRAKEN_PADFRAME_AON_CONFIG_PAD_GPIOB_CFG_0_TRIE_0, REG_READ32(address));
}

void kraken_padframe_aon_pad_gpiob_cfg_pue_set(uint32_t idx, uint8_t value) {
  uint32_t address = KRAKEN_PADFRAME_AON_CONFIG_PAD_GPIOB_CFG_0(0) + 4*idx;
  uint32_t v = REG_READ32(address);
  v &= ~REG_CLR(KRAKEN_PADFRAME_AON_CONFIG_PAD_GPIOB_CFG_0_PUE_0);
  v |= REG_SET(KRAKEN_PADFRAME_AON_CONFIG_PAD_GPIOB_CFG_0_PUE_0, value);
  REG_WRITE32(address, v);
}

uint8_t kraken_padframe_aon_pad_gpiob_cfg_pue_get(uint32_t idx) {
  uint32_t address = KRAKEN_PADFRAME_AON_CONFIG_PAD_GPIOB_CFG_0(0) + 4*idx;
  return REG_GET(KRAKEN_PADFRAME_AON_CONFIG_PAD_GPIOB_CFG_0_PUE_0, REG_READ32(address));
}

void kraken_padframe_aon_pad_gpiob_cfg_pde_set(uint32_t idx, uint8_t value) {
  uint32_t address = KRAKEN_PADFRAME_AON_CONFIG_PAD_GPIOB_CFG_0(0) + 4*idx;
  uint32_t v = REG_READ32(address);
  v &= ~REG_CLR(KRAKEN_PADFRAME_AON_CONFIG_PAD_GPIOB_CFG_0_PDE_0);
  v |= REG_SET(KRAKEN_PADFRAME_AON_CONFIG_PAD_GPIOB_CFG_0_PDE_0, value);
  REG_WRITE32(address, v);
}

uint8_t kraken_padframe_aon_pad_gpiob_cfg_pde_get(uint32_t idx) {
  uint32_t address = KRAKEN_PADFRAME_AON_CONFIG_PAD_GPIOB_CFG_0(0) + 4*idx;
  return REG_GET(KRAKEN_PADFRAME_AON_CONFIG_PAD_GPIOB_CFG_0_PDE_0, REG_READ32(address));
}

void kraken_padframe_aon_pad_gpiob_cfg_drv_set(uint32_t idx, uint8_t value) {
  uint32_t address = KRAKEN_PADFRAME_AON_CONFIG_PAD_GPIOB_CFG_0(0) + 4*idx;
  uint32_t v = REG_READ32(address);
  v &= ~REG_CLR(KRAKEN_PADFRAME_AON_CONFIG_PAD_GPIOB_CFG_0_DRV_0);
  v |= REG_SET(KRAKEN_PADFRAME_AON_CONFIG_PAD_GPIOB_CFG_0_DRV_0, value);
  REG_WRITE32(address, v);
}

uint8_t kraken_padframe_aon_pad_gpiob_cfg_drv_get(uint32_t idx) {
  uint32_t address = KRAKEN_PADFRAME_AON_CONFIG_PAD_GPIOB_CFG_0(0) + 4*idx;
  return REG_GET(KRAKEN_PADFRAME_AON_CONFIG_PAD_GPIOB_CFG_0_DRV_0, REG_READ32(address));
}

void kraken_padframe_aon_pad_gpiob_cfg_slw_set(uint32_t idx, uint8_t value) {
  uint32_t address = KRAKEN_PADFRAME_AON_CONFIG_PAD_GPIOB_CFG_0(0) + 4*idx;
  uint32_t v = REG_READ32(address);
  v &= ~REG_CLR(KRAKEN_PADFRAME_AON_CONFIG_PAD_GPIOB_CFG_0_SLW_0);
  v |= REG_SET(KRAKEN_PADFRAME_AON_CONFIG_PAD_GPIOB_CFG_0_SLW_0, value);
  REG_WRITE32(address, v);
}

uint8_t kraken_padframe_aon_pad_gpiob_cfg_slw_get(uint32_t idx) {
  uint32_t address = KRAKEN_PADFRAME_AON_CONFIG_PAD_GPIOB_CFG_0(0) + 4*idx;
  return REG_GET(KRAKEN_PADFRAME_AON_CONFIG_PAD_GPIOB_CFG_0_SLW_0, REG_READ32(address));
}

void kraken_padframe_aon_pad_gpiob_cfg_smt_set(uint32_t idx, uint8_t value) {
  uint32_t address = KRAKEN_PADFRAME_AON_CONFIG_PAD_GPIOB_CFG_0(0) + 4*idx;
  uint32_t v = REG_READ32(address);
  v &= ~REG_CLR(KRAKEN_PADFRAME_AON_CONFIG_PAD_GPIOB_CFG_0_SMT_0);
  v |= REG_SET(KRAKEN_PADFRAME_AON_CONFIG_PAD_GPIOB_CFG_0_SMT_0, value);
  REG_WRITE32(address, v);
}

uint8_t kraken_padframe_aon_pad_gpiob_cfg_smt_get(uint32_t idx) {
  uint32_t address = KRAKEN_PADFRAME_AON_CONFIG_PAD_GPIOB_CFG_0(0) + 4*idx;
  return REG_GET(KRAKEN_PADFRAME_AON_CONFIG_PAD_GPIOB_CFG_0_SMT_0, REG_READ32(address));
}

void kraken_padframe_aon_pad_gpiob_mux_set(uint32_t idx, kraken_padframe_aon_pad_gpiob_mux_sel_t mux_sel) {
  const uint32_t pads_per_register = 5;
  const uint32_t address = KRAKEN_PADFRAME_AON_CONFIG_PAD_GPIOB_MUX_SEL_0(0)+idx/pads_per_register*4;
  const uint32_t sel_size = 6;
  uint32_t field_offset = idx % pads_per_register * sel_size;
  uint32_t field_mask = ((1 << (6)) - 1);
  uint32_t v = REG_READ32(address);
  v &= ~(field_mask << field_offset);
  v |= (mux_sel & field_mask) << field_offset;
  REG_WRITE32(address, v);
}

kraken_padframe_aon_pad_gpiob_mux_sel_t kraken_padframe_aon_pad_gpiob_mux_get(uint32_t idx) {
  const uint32_t pads_per_register = 5;
  const uint32_t address = KRAKEN_PADFRAME_AON_CONFIG_PAD_GPIOB_MUX_SEL_0(0)+idx/pads_per_register*4;
  const uint32_t sel_size = 6;
  uint32_t field_offset = idx % pads_per_register * sel_size;
  uint32_t field_mask = ((1 << 6) - 1) <<sel_size-1;
  return (REG_READ32(address) >> field_offset) & field_mask;
}
