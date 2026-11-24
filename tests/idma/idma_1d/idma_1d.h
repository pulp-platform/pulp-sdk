#include "pmsis.h"
#include "stdio.h"

#include "idma_def_1d.h"
#include "idma_presets.h"
#include "idma_param_1d.h"

#ifndef _DMA_FRONTEND_REG_DEFS_
#define _DMA_FRONTEND_REG_DEFS_

#ifdef __cplusplus
extern "C" {
#endif

#define DMA_CONF_DECOUPLE 0
#define DMA_CONF_DEBURST 0
#define DMA_CONF_SERIALIZE 0

#define CORE_SPACE 1024
#define TOT_SIZE ARCHI_CLUSTER_NB_PE * CORE_SPACE
#define NB_PRESETS 14

#ifdef DEBUG_TEST
    #define PRINTF(...) printf(__VA_ARGS__)
#else
    #define PRINTF(...)
#endif

#ifdef CYCLE_COUNT
    static inline void start_cycle_count () { pi_perf_cl_start(); }
    static inline void stop_cycle_count () { pi_perf_cl_stop(); }
    static inline void reset_cycle_count () { pi_perf_conf(PI_PERF_CYCLES); pi_perf_cl_reset(); }
    static inline unsigned int getcycles() { return pi_perf_cl_read(PI_PERF_CYCLES); }
#else
    static inline void start_cycle_count () {}
    static inline void stop_cycle_count () {}
    static inline void reset_cycle_count () {}
    static inline unsigned int getcycles() {return 0;}
#endif


#ifdef __cplusplus
}  // extern "C"
#endif
#endif  // _DMA_FRONTEND_REG_DEFS_
