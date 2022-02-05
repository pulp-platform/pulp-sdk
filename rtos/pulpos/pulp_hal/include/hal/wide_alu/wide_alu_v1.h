#include "archi/wide_alu/wide_alu_v1.h"

static inline int set_delay(uint8_t delay);
static inline int set_operands(uint32_t *a, uint32_t *b);
static inline int poll_done(void);
static inline int get_result(uint32_t *result);
static inline void clear_error(void);
static inline int set_operation(int opsel);
static inline int trigger_operation(void);
static inline int wide_multiply(uint32_t *a, uint32_t *b, uint32_t *c);

static inline int set_delay(uint8_t delay)
{
  if(poll_done() != WIDE_ALU_STATUS_CODE_VALUE_IDLE)
  {
    return -1;
  } 
 
  volatile uint32_t *ctrl2_reg = (volatile uint32_t *)WIDE_ALU_CTRL2(0);
  // Read the old value to save the opsel fields
  uint32_t ctrl2_old_value = *(volatile uint32_t *)ctrl2_reg;
  // Overwrite
  *(volatile uint32_t *)ctrl2_reg = ctrl2_old_value | ((delay & WIDE_ALU_CTRL2_DELAY_MASK)<<WIDE_ALU_CTRL2_DELAY_LSB);

  return 0;
}


static inline int set_operands(uint32_t *a, uint32_t *b)
{
  if(poll_done() != WIDE_ALU_STATUS_CODE_VALUE_IDLE)
  {
    return -1;
  } 

  volatile uint32_t *op_a_reg = (volatile uint32_t *)WIDE_ALU_OP_A(0);
  volatile uint32_t *op_b_reg = (volatile uint32_t *)WIDE_ALU_OP_B(0);

  for (int i = 0; i<8; i++)
  {
    op_a_reg[i] = a[i];
    op_b_reg[i] = b[i];
  }

  return 0;
}

static inline int poll_done(void)
{
  volatile uint32_t *status_reg = (volatile uint32_t *)WIDE_ALU_STATUS(0);
  uint32_t current_status;
  do {
    current_status = (((*(volatile uint32_t *)status_reg) & WIDE_ALU_STATUS_CODE_MASK)<<WIDE_ALU_STATUS_CODE_LSB);
  } while(current_status == WIDE_ALU_STATUS_CODE_VALUE_PENDING);

  return current_status;
}

static inline int get_result(uint32_t *result)
{
  if(poll_done() != WIDE_ALU_STATUS_CODE_VALUE_IDLE)
  {
    return -1;
  } 

  volatile uint32_t *result_reg = (volatile uint32_t *)WIDE_ALU_RESULT(0);
  for (int i = 0; i<16; i++)
  {
    result[i] = result_reg[i];
  }

  return 0;
}

static inline void clear_error(void)
{
  volatile uint32_t *ctrl1_reg = (volatile uint32_t *)WIDE_ALU_CTRL1(0);
  uint32_t ctrl1_old_value = *(volatile uint32_t *)ctrl1_reg;
  *(volatile uint32_t *)ctrl1_reg = ctrl1_old_value | ((1 & WIDE_ALU_CTRL1_CLEAR_ERR_MASK)<<WIDE_ALU_CTRL1_CLEAR_ERR_LSB);
}

static inline int set_operation(int opsel)
{
  if(poll_done() != WIDE_ALU_STATUS_CODE_VALUE_IDLE)
  {
    return -1;
  } 

  volatile uint32_t *ctrl2_reg = (volatile uint32_t *)WIDE_ALU_CTRL2(0);
  uint32_t ctrl2_old_value = *(volatile uint32_t *)ctrl2_reg;
  *(volatile uint32_t *)ctrl2_reg = ctrl2_old_value | ((opsel & WIDE_ALU_CTRL2_OPSEL_MASK)<<WIDE_ALU_CTRL2_OPSEL_LSB);

  return 0;
}

static inline int trigger_operation(void)
{
  if(poll_done() != WIDE_ALU_STATUS_CODE_VALUE_IDLE)
  {
    return -1;
  } 

  volatile uint32_t *ctrl1_reg = (volatile uint32_t *)WIDE_ALU_CTRL1(0);
  uint32_t ctrl1_old_value = *(volatile uint32_t *)ctrl1_reg;
  *(volatile uint32_t *)ctrl1_reg = ctrl1_old_value | ((1 & WIDE_ALU_CTRL1_TRIGGER_MASK)<<WIDE_ALU_CTRL1_TRIGGER_LSB);

  return 0; 
}

static inline int wide_multiply(uint32_t *a, uint32_t *b, uint32_t *c)
{
  // Set the operands 
  if(set_operands(a,b))
  {
    return -1;
  }

  // Set multiplication
  if(set_operation(WIDE_ALU_CTRL2_OPSEL_VALUE_MUL))
  {
    return -1;
  } 

  // Trigger the operation
  if(trigger_operation())
  {
    return -1;
  }

  // Read the result
  if(get_result(c))
  {
    return -1;
  }

  return 0;
}
