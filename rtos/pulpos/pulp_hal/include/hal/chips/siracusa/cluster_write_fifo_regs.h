// Generated register defines for cluster_write_fifo

#ifndef _CLUSTER_WRITE_FIFO_REG_DEFS_
#define _CLUSTER_WRITE_FIFO_REG_DEFS_

#ifdef __cplusplus
extern "C" {
#endif
// Register width
#define CLUSTER_WRITE_FIFO_PARAM_REG_WIDTH 32

// The current fill state, error count and other status of the cluster write
// fifo
#define CLUSTER_WRITE_FIFO_STATUS_REG_OFFSET 0x0
#define CLUSTER_WRITE_FIFO_STATUS_REQ_FIFO_FILL_MASK 0xff
#define CLUSTER_WRITE_FIFO_STATUS_REQ_FIFO_FILL_OFFSET 0
#define CLUSTER_WRITE_FIFO_STATUS_REQ_FIFO_FILL_FIELD \
  ((bitfield_field32_t) { .mask = CLUSTER_WRITE_FIFO_STATUS_REQ_FIFO_FILL_MASK, .index = CLUSTER_WRITE_FIFO_STATUS_REQ_FIFO_FILL_OFFSET })
#define CLUSTER_WRITE_FIFO_STATUS_PEND_REQ_FIFO_FILL_MASK 0xff
#define CLUSTER_WRITE_FIFO_STATUS_PEND_REQ_FIFO_FILL_OFFSET 8
#define CLUSTER_WRITE_FIFO_STATUS_PEND_REQ_FIFO_FILL_FIELD \
  ((bitfield_field32_t) { .mask = CLUSTER_WRITE_FIFO_STATUS_PEND_REQ_FIFO_FILL_MASK, .index = CLUSTER_WRITE_FIFO_STATUS_PEND_REQ_FIFO_FILL_OFFSET })
#define CLUSTER_WRITE_FIFO_STATUS_ERROR_FIFO_FILL_MASK 0xff
#define CLUSTER_WRITE_FIFO_STATUS_ERROR_FIFO_FILL_OFFSET 16
#define CLUSTER_WRITE_FIFO_STATUS_ERROR_FIFO_FILL_FIELD \
  ((bitfield_field32_t) { .mask = CLUSTER_WRITE_FIFO_STATUS_ERROR_FIFO_FILL_MASK, .index = CLUSTER_WRITE_FIFO_STATUS_ERROR_FIFO_FILL_OFFSET })

// Allows flushing the various FIFOs within the module.
#define CLUSTER_WRITE_FIFO_CTRL_REG_OFFSET 0x4
#define CLUSTER_WRITE_FIFO_CTRL_FLUSH_REQ_FIFO_BIT 0
#define CLUSTER_WRITE_FIFO_CTRL_FLUSH_OUTSTD_REQ_FIFO_BIT 1
#define CLUSTER_WRITE_FIFO_CTRL_FLUSH_ERROR_FIFO_BIT 2

// Read from error response FIFO. The error response FIFO contains the
// address of every request that was answered with an error response. Every
// read from this reg will pop one error item from the FIFO.
#define CLUSTER_WRITE_FIFO_ERROR_RSP_FIFO_REG_OFFSET 0x8

#ifdef __cplusplus
}  // extern "C"
#endif
#endif  // _CLUSTER_WRITE_FIFO_REG_DEFS_
// End generated register defines for cluster_write_fifo