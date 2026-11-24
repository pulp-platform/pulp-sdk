#ifndef __PULP_H__
#define __PULP_H__

#ifndef CLUSTER_STACK_SIZE
#define CLUSTER_STACK_SIZE 0x800
#endif

#ifndef LANGUAGE_ASSEMBLY

// pulp-runtime's pulp.h left these to the application; most tests call printf
// without including stdio, which GCC 14 rejects.
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pmsis.h"

#include <data/data.h>
#include <bench/bench.h>

#define get_core_id     hal_core_id
#define rt_core_id      hal_core_id
#define rt_cluster_id   hal_cluster_id
#define get_cluster_id  hal_cluster_id

#ifdef ARCHI_CLUSTER_NB_PE
static inline int get_core_num() { return ARCHI_CLUSTER_NB_PE; }
#endif

void synch_barrier(void);

#ifdef ARCHI_HAS_CLUSTER
void cluster_start(int cid, int (*entry)());
int  cluster_wait(int cid);
void cluster_entry_stub(void);
#else
static inline void cluster_start(int cid, int (*entry)()) {}
static inline int  cluster_wait(int cid) { return 0; }
#endif

void _start();

#endif /* LANGUAGE_ASSEMBLY */
#endif
