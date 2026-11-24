#ifndef __COMPAT_DATA_DATA_H__
#define __COMPAT_DATA_DATA_H__

#include <pos/data/data.h>

// Section names match the SDK link scripts; only these aliases are missing there.
#ifndef PI_L2
#define PI_L2 __attribute__((section(".l2_data")))
#endif

#define FC_GLOBAL_DATA __attribute__((section(".l2_data")))
#define FC_DATA        FC_GLOBAL_DATA
#define L2_DATA        PI_L2
#define L1_GLOBAL_DATA __attribute__((section(".data_l1")))
#define L1_DATA        L1_GLOBAL_DATA

#ifdef USE_CLUSTER
#define RT_LOCAL_DATA L1_DATA
#else
#define RT_LOCAL_DATA FC_DATA
#endif

#endif
