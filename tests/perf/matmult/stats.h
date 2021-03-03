#ifndef _STATS_H
#define _STATS_H

#define HOTTING 2
#define REPEAT  5

#ifdef STATS

#define INIT_STATS()  

#define PRE_START_STATS()  \
    unsigned long _cycles = 0; \
    unsigned long _instr = 0; \
    unsigned long _active = 0; \
    unsigned long _ldext = 0; \
    unsigned long _tcdmcont = 0; \
    unsigned long _ldstall = 0; \
    unsigned long _imiss = 0; \
    for(int _k=0; _k<HOTTING+REPEAT; _k++) { \
      pi_perf_conf((1<<PI_PERF_CYCLES) | (1<<PI_PERF_INSTR) | (1<<PI_PERF_ACTIVE_CYCLES) | (1<<PI_PERF_LD_EXT) | (1<<PI_PERF_TCDM_CONT) | (1<<PI_PERF_LD_STALL) | (1<<PI_PERF_IMISS) );


#define START_STATS()  \
    pi_perf_reset(); \
    pi_perf_start();

#define STOP_STATS() \
   pi_perf_stop(); \
   if (_k >= HOTTING) \
    { \
      _cycles   += pi_perf_read (PI_PERF_CYCLES); \
      _instr    += pi_perf_read (PI_PERF_INSTR); \
    	_active   += pi_perf_read (PI_PERF_ACTIVE_CYCLES); \
      _ldext    += pi_perf_read (PI_PERF_LD_EXT); \
    	_tcdmcont += pi_perf_read (PI_PERF_TCDM_CONT); \
    	_ldstall  += pi_perf_read (PI_PERF_LD_STALL); \
      _imiss    += pi_perf_read (PI_PERF_IMISS); \
    } \
   if (_k == HOTTING+REPEAT-1) \
    { \
      int id = pi_core_id(); \
      printf("[%d] cycles = %lu\n", id, _cycles/REPEAT); \
      printf("[%d] instr = %lu\n", id, _instr/REPEAT); \
    	printf("[%d] active cycles = %lu\n", id, _active/REPEAT); \
      printf("[%d] ext load = %lu\n", id, _ldext/REPEAT); \
    	printf("[%d] TCDM cont = %lu\n", id, _tcdmcont/REPEAT); \
    	printf("[%d] ld stall = %lu\n", id, _ldstall/REPEAT); \
    	printf("[%d] imiss = %lu\n", id, _imiss/REPEAT); \
    } \
  }

#else // STATS

#define INIT_STATS()
#define PRE_START_STATS()
#define START_STATS()
#define STOP_STATS()

#endif  // STATS

#endif
