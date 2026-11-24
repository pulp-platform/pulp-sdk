#include "pmsis.h"
#include "stdio.h"
#include "mchan_def_1d.h"
#include "mchan_param_1d.h"

#ifdef DEBUG_TEST
    #define PRINTF(...) printf(__VA_ARGS__)
#else
    #define PRINTF(...)
#endif

#define CORE_SPACE 2048
#define TOT_SIZE ARCHI_CLUSTER_NB_PE * CORE_SPACE
#define NB_PRESETS 13

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