// Generated register defines for i3c_config

#ifndef _I3C_CONFIG_REG_DEFS_
#define _I3C_CONFIG_REG_DEFS_

#ifdef __cplusplus
extern "C" {
#endif
// Register width
#define I3C_CONFIG_PARAM_REG_WIDTH 32

// Perform software reset of the whole IP
#define I3C_CONFIG_SWRST_REG_OFFSET 0x0
#define I3C_CONFIG_SWRST_GLOBRST_BIT 0
#define I3C_CONFIG_SWRST_LOCRST_BIT 1

// Configure static device configuration. Any change to this register (except
// for RSTCYCLES) requires reseting the I3C peripheral
#define I3C_CONFIG_CFG1_REG_OFFSET 0x4
#define I3C_CONFIG_CFG1_RSTCYCLES_MASK 0xff
#define I3C_CONFIG_CFG1_RSTCYCLES_OFFSET 0
#define I3C_CONFIG_CFG1_RSTCYCLES_FIELD \
  ((bitfield_field32_t) { .mask = I3C_CONFIG_CFG1_RSTCYCLES_MASK, .index = I3C_CONFIG_CFG1_RSTCYCLES_OFFSET })
#define I3C_CONFIG_CFG1_DEVICE_ROLE_BIT 8
#define I3C_CONFIG_CFG1_PID_MFR_MASK 0x7fff
#define I3C_CONFIG_CFG1_PID_MFR_OFFSET 9
#define I3C_CONFIG_CFG1_PID_MFR_FIELD \
  ((bitfield_field32_t) { .mask = I3C_CONFIG_CFG1_PID_MFR_MASK, .index = I3C_CONFIG_CFG1_PID_MFR_OFFSET })
#define I3C_CONFIG_CFG1_PID_INST_ID_MASK 0xf
#define I3C_CONFIG_CFG1_PID_INST_ID_OFFSET 24
#define I3C_CONFIG_CFG1_PID_INST_ID_FIELD \
  ((bitfield_field32_t) { .mask = I3C_CONFIG_CFG1_PID_INST_ID_MASK, .index = I3C_CONFIG_CFG1_PID_INST_ID_OFFSET })

// Configure static device configuration. Any change to this register (except
// for RSTCYCLES) requires reseting the I3C peripheral
#define I3C_CONFIG_CFG2_REG_OFFSET 0x8
#define I3C_CONFIG_CFG2_BUS_AVAIL_TIMER_MASK 0xff
#define I3C_CONFIG_CFG2_BUS_AVAIL_TIMER_OFFSET 0
#define I3C_CONFIG_CFG2_BUS_AVAIL_TIMER_FIELD \
  ((bitfield_field32_t) { .mask = I3C_CONFIG_CFG2_BUS_AVAIL_TIMER_MASK, .index = I3C_CONFIG_CFG2_BUS_AVAIL_TIMER_OFFSET })
#define I3C_CONFIG_CFG2_BUS_IDLE_TIMER_MASK 0x3ffff
#define I3C_CONFIG_CFG2_BUS_IDLE_TIMER_OFFSET 8
#define I3C_CONFIG_CFG2_BUS_IDLE_TIMER_FIELD \
  ((bitfield_field32_t) { .mask = I3C_CONFIG_CFG2_BUS_IDLE_TIMER_MASK, .index = I3C_CONFIG_CFG2_BUS_IDLE_TIMER_OFFSET })

// Active-safety feature configuration
#define I3C_CONFIG_ASF_CFG_REG_OFFSET 0xc
#define I3C_CONFIG_ASF_CFG_ASF_PASSTHROUGH_EN_BIT 0

#ifdef __cplusplus
}  // extern "C"
#endif
#endif  // _I3C_CONFIG_REG_DEFS_
// End generated register defines for i3c_config