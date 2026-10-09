#define NB_TRANSFERS 10

typedef struct { 
  unsigned int size_3d;
  unsigned int length;
  unsigned int src_stride_2d;
  unsigned int dst_stride_2d;
  unsigned int src_stride_3d;
  unsigned int dst_stride_3d;
  unsigned int num_reps_3d;
} transfer_3d;

