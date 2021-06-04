#ifndef __STATS_BOARD_H__
#define __STATS_BOARD_H__

#ifdef STATS

#define HOTTING 2 /* Necessary Iterations to avoid cache cold effects */
#define REPEAT  5 /* Averaging on 5 successive Iterations */ 


#define INIT_STATS()  \
    unsigned long _cycles   = 0; \
    unsigned long _instr    = 0; \
    unsigned long _ldstall  = 0; \
    unsigned long _btaken   = 0; \
    unsigned long _jrstall  = 0;  


#define ENTER_LOOP_STATS()  \
    for(int _k = 0; _k < HOTTING + REPEAT; _k++) \
    { \
      pi_perf_conf( \
        (1<<PI_PERF_CYCLES)   | \
        (1<<PI_PERF_INSTR)    | \
        (1<<PI_PERF_LD_STALL) | \
        (1<<PI_PERF_BTAKEN)   | \
        (1<<PI_PERF_JR_STALL) \
      );


#define START_STATS()  \
      pi_perf_reset(); \
      pi_perf_start();


#define STOP_STATS() \
    pi_perf_stop(); \
    if (_k >= HOTTING) \
    { \
      _cycles   += pi_perf_read (PI_PERF_CYCLES); \
      _instr    += pi_perf_read (PI_PERF_INSTR); \
      _ldstall  += pi_perf_read (PI_PERF_LD_STALL); \
      _btaken  += pi_perf_read (PI_PERF_BTAKEN); \
      _jrstall  += pi_perf_read (PI_PERF_JR_STALL); \
    }


#define EXIT_LOOP_STATS()  \
    } \
    _cycles   = _cycles/REPEAT; \
    _instr    = _instr/REPEAT; \
    _ldstall  = _ldstall/REPEAT; \
    _btaken   = _btaken/REPEAT; \
    _jrstall  = _jrstall/REPEAT; \
    printf("Core Statistics\n"); \
    printf("Total Cycles          = %lu\n", _cycles); \
    printf("Executed Instructions = %lu\n", _instr);  \
    printf("Load Stalls           = %lu\n", _ldstall); \
    printf("Branch Taken          = %lu\n", _btaken); \
    printf("Jump Stalls           = %lu\n", _jrstall); \

#else  // ! STATS

#define INIT_STATS()
#define ENTER_LOOP_STATS()
#define START_STATS()
#define STOP_STATS()
#define EXIT_LOOP_STATS()

#endif


#endif