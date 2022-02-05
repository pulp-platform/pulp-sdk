// Generated register defines for wide_alu

#ifndef _WIDE_ALU_REG_DEFS_
#define _WIDE_ALU_REG_DEFS_

#ifdef __cplusplus
extern "C" {
#endif
// Register width
#define WIDE_ALU_PARAM_REG_WIDTH 32

// Subword of Operand A. (common parameters)
#define WIDE_ALU_OP_A_OP_A_FIELD_WIDTH 32
#define WIDE_ALU_OP_A_OP_A_FIELDS_PER_REG 1
#define WIDE_ALU_OP_A_MULTIREG_COUNT 8

#define WIDE_ALU_OP_A_REG_OFFSET 0x0
#define WIDE_ALU_OP_A(id) (ARCHI_WIDE_ALU_ADDR + WIDE_ALU_OP_A_REG_OFFSET + (0x4 * (id)))

// Subword of Operand A.
#define WIDE_ALU_OP_A_0_REG_OFFSET 0x0

// Subword of Operand A.
#define WIDE_ALU_OP_A_1_REG_OFFSET 0x4

// Subword of Operand A.
#define WIDE_ALU_OP_A_2_REG_OFFSET 0x8

// Subword of Operand A.
#define WIDE_ALU_OP_A_3_REG_OFFSET 0xc

// Subword of Operand A.
#define WIDE_ALU_OP_A_4_REG_OFFSET 0x10

// Subword of Operand A.
#define WIDE_ALU_OP_A_5_REG_OFFSET 0x14

// Subword of Operand A.
#define WIDE_ALU_OP_A_6_REG_OFFSET 0x18

// Subword of Operand A.
#define WIDE_ALU_OP_A_7_REG_OFFSET 0x1c

// Subword of Operand B. (common parameters)
#define WIDE_ALU_OP_B_OP_B_FIELD_WIDTH 32
#define WIDE_ALU_OP_B_OP_B_FIELDS_PER_REG 1
#define WIDE_ALU_OP_B_MULTIREG_COUNT 8

#define WIDE_ALU_OP_B_REG_OFFSET 0x20
#define WIDE_ALU_OP_B(id) (ARCHI_WIDE_ALU_ADDR + WIDE_ALU_OP_B_REG_OFFSET + (0x4 * (id)))

// Subword of Operand B.
#define WIDE_ALU_OP_B_0_REG_OFFSET 0x20

// Subword of Operand B.
#define WIDE_ALU_OP_B_1_REG_OFFSET 0x24

// Subword of Operand B.
#define WIDE_ALU_OP_B_2_REG_OFFSET 0x28

// Subword of Operand B.
#define WIDE_ALU_OP_B_3_REG_OFFSET 0x2c

// Subword of Operand B.
#define WIDE_ALU_OP_B_4_REG_OFFSET 0x30

// Subword of Operand B.
#define WIDE_ALU_OP_B_5_REG_OFFSET 0x34

// Subword of Operand B.
#define WIDE_ALU_OP_B_6_REG_OFFSET 0x38

// Subword of Operand B.
#define WIDE_ALU_OP_B_7_REG_OFFSET 0x3c

// Subword of results. (common parameters)
#define WIDE_ALU_RESULT_RESULT_FIELD_WIDTH 32
#define WIDE_ALU_RESULT_RESULT_FIELDS_PER_REG 1
#define WIDE_ALU_RESULT_MULTIREG_COUNT 16

#define WIDE_ALU_RESULT_REG_OFFSET 0x40
#define WIDE_ALU_RESULT(id) (ARCHI_WIDE_ALU_ADDR + WIDE_ALU_RESULT_REG_OFFSET + (0x4 * (id)))

// Subword of results.
#define WIDE_ALU_RESULT_0_REG_OFFSET 0x40

// Subword of results.
#define WIDE_ALU_RESULT_1_REG_OFFSET 0x44

// Subword of results.
#define WIDE_ALU_RESULT_2_REG_OFFSET 0x48

// Subword of results.
#define WIDE_ALU_RESULT_3_REG_OFFSET 0x4c

// Subword of results.
#define WIDE_ALU_RESULT_4_REG_OFFSET 0x50

// Subword of results.
#define WIDE_ALU_RESULT_5_REG_OFFSET 0x54

// Subword of results.
#define WIDE_ALU_RESULT_6_REG_OFFSET 0x58

// Subword of results.
#define WIDE_ALU_RESULT_7_REG_OFFSET 0x5c

// Subword of results.
#define WIDE_ALU_RESULT_8_REG_OFFSET 0x60

// Subword of results.
#define WIDE_ALU_RESULT_9_REG_OFFSET 0x64

// Subword of results.
#define WIDE_ALU_RESULT_10_REG_OFFSET 0x68

// Subword of results.
#define WIDE_ALU_RESULT_11_REG_OFFSET 0x6c

// Subword of results.
#define WIDE_ALU_RESULT_12_REG_OFFSET 0x70

// Subword of results.
#define WIDE_ALU_RESULT_13_REG_OFFSET 0x74

// Subword of results.
#define WIDE_ALU_RESULT_14_REG_OFFSET 0x78

// Subword of results.
#define WIDE_ALU_RESULT_15_REG_OFFSET 0x7c

// Controls clear and trigger signal of the deaccelerator.
#define WIDE_ALU_CTRL1_REG_OFFSET 0x80
#define WIDE_ALU_CTRL1(id) (ARCHI_WIDE_ALU_ADDR + WIDE_ALU_CTRL1_REG_OFFSET + (0x4 * (id)))

#define WIDE_ALU_CTRL1_TRIGGER_MASK 0x1
#define WIDE_ALU_CTRL1_CLEAR_ERR_MASK 0x1
#define WIDE_ALU_CTRL1_TRIGGER_LSB 0
#define WIDE_ALU_CTRL1_CLEAR_ERR_LSB 1

// Configures the operation and its delay within the deaccelerator.
#define WIDE_ALU_CTRL2_REG_OFFSET 0x84
#define WIDE_ALU_CTRL2(id) (ARCHI_WIDE_ALU_ADDR + WIDE_ALU_CTRL2_REG_OFFSET + (0x4 * (id)))

#define WIDE_ALU_CTRL2_OPSEL_MASK 0x7
#define WIDE_ALU_CTRL2_OPSEL_LSB 0
#define WIDE_ALU_CTRL2_OPSEL_OFFSET 0
#define WIDE_ALU_CTRL2_OPSEL_FIELD \
  ((bitfield_field32_t) { .mask = WIDE_ALU_CTRL2_OPSEL_MASK, .index = WIDE_ALU_CTRL2_OPSEL_OFFSET })
#define WIDE_ALU_CTRL2_OPSEL_VALUE_ADD 0x0
#define WIDE_ALU_CTRL2_OPSEL_VALUE_SUB 0x1
#define WIDE_ALU_CTRL2_OPSEL_VALUE_MUL 0x2
#define WIDE_ALU_CTRL2_OPSEL_VALUE_XOR 0x3
#define WIDE_ALU_CTRL2_OPSEL_VALUE_AND 0x4
#define WIDE_ALU_CTRL2_OPSEL_VALUE_OR 0x5
#define WIDE_ALU_CTRL2_DELAY_MASK 0xff
#define WIDE_ALU_CTRL2_DELAY_LSB 16
#define WIDE_ALU_CTRL2_DELAY_OFFSET 16
#define WIDE_ALU_CTRL2_DELAY_FIELD \
  ((bitfield_field32_t) { .mask = WIDE_ALU_CTRL2_DELAY_MASK, .index = WIDE_ALU_CTRL2_DELAY_OFFSET })

// Contains the current status of the Deaccelerator.
#define WIDE_ALU_STATUS_REG_OFFSET 0x88
#define WIDE_ALU_STATUS(id) (ARCHI_WIDE_ALU_ADDR + WIDE_ALU_STATUS_REG_OFFSET + (0x4 * (id)))
#define WIDE_ALU_STATUS_CODE_MASK 0x3
#define WIDE_ALU_STATUS_CODE_LSB 0
#define WIDE_ALU_STATUS_CODE_OFFSET 0
#define WIDE_ALU_STATUS_CODE_FIELD \
  ((bitfield_field32_t) { .mask = WIDE_ALU_STATUS_CODE_MASK, .index = WIDE_ALU_STATUS_CODE_OFFSET })
#define WIDE_ALU_STATUS_CODE_VALUE_IDLE 0x0
#define WIDE_ALU_STATUS_CODE_VALUE_PENDING 0x1
#define WIDE_ALU_STATUS_CODE_VALUE_ERROR_WRITE 0x2
#define WIDE_ALU_STATUS_CODE_VALUE_ERROR_OPCODE 0x3

#ifdef __cplusplus
}  // extern "C"
#endif
#endif  // _WIDE_ALU_REG_DEFS_
// End generated register defines for wide_alu
