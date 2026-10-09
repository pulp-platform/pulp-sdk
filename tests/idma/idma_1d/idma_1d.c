#include "idma_1d.h"

int glob_errors = 0;
uint32_t l1_addr[8] = {0};
uint32_t l2_addr[8] = {0};
uint32_t l1_dst_addr[8] = {0};

int idma_1D (uint32_t size, int core_id, int ext2loc, int loc2loc) {
    int error = 0;
    volatile uint8_t *src_ptr, *dst_ptr;

    if (loc2loc == 1) {
        // L1 to L1 transfer
        src_ptr = (uint8_t*) l1_addr[core_id];
        dst_ptr = (uint8_t*) l1_dst_addr[core_id];
    } else if (ext2loc == 1) {
        // L2 to L1 transfer
        src_ptr = (uint8_t*) l2_addr[core_id];
        dst_ptr = (uint8_t*) l1_addr[core_id];
    } else {
        // L1 to L2 transfer
        src_ptr = (uint8_t*) l1_addr[core_id];
        dst_ptr = (uint8_t*) l2_addr[core_id];
    }

    for (int i = 0; i < size; i++) {
        src_ptr[i] = (uint8_t)(i & 0xFF);
    }
    #ifndef MULTI_CORE_P
    plp_idma_enable_clk();
    #endif
    if (loc2loc == 1) {
        reset_cycle_count();
        start_cycle_count();
        plp_cl_dma_wait_toL1(pulp_cl_idma_L1ToL1((unsigned int) src_ptr, (unsigned int) dst_ptr, size));
        stop_cycle_count();
    } else if (ext2loc == 1) {
        reset_cycle_count();
        start_cycle_count();
        plp_cl_dma_wait_toL1(pulp_cl_idma_L2ToL1((unsigned int) src_ptr, (unsigned int) dst_ptr, size));
        stop_cycle_count();
    } else {
        reset_cycle_count();
        start_cycle_count();
        plp_cl_dma_wait_toL2(pulp_cl_idma_L1ToL2((unsigned int) src_ptr, (unsigned int) dst_ptr, size));
        stop_cycle_count();
    }
    #ifndef MULTI_CORE_P
    plp_idma_disable_clk();
    #endif
    PRINTF ("This transfer took %d cycles \n", getcycles());

    // Check the results
    for (int i=0; i < size; i++) {
        uint8_t expected = src_ptr[i]; 
        uint8_t actual   = dst_ptr[i];

        if (expected != actual) {
            error++;
            if (core_id == 0) {
                PRINTF ("Error: expected @%8x = %8x vs actual @%8x = %8x \n", expected, &src_ptr[i], actual, &dst_ptr[i]);
            }
        }
    }

    return error;
}

void idma_task() {
    PRINTF ("Core[%d] has entered idma_task \n", pi_core_id());
    uint32_t size;
    uint32_t transfers_num;
    #ifdef QUICK_MODE
    transfers_num = NB_PRESETS;
    #else
    transfers_num = NB_TRANSFERS;
    #endif
    for (int k = 0; k < transfers_num; k ++) {
        #ifdef QUICK_MODE
        size = idma_presets[k].size_1d;
        #else
        size = params_1d[k].size_1d;
        #endif

        // L1 -> L2
        PRINTF ("L1 -> L2: Transfer %d with size %d \n", k, size);
        glob_errors += idma_1D(size, pi_core_id(), 0, 0);
        // L2 -> L1
        PRINTF ("L2 -> L1: Transfer %d with size %d \n", k, size);
        glob_errors += idma_1D(size, pi_core_id(), 1, 0);
        // L1 -> L1
        PRINTF ("L1 -> L1: Transfer %d with size %d \n", k, size);
        glob_errors += idma_1D(size, pi_core_id(), 0, 1);
    }
}

void allocate_mem_to_cores () {
    int core_id = pi_core_id();

    // Pre-allocate TOT_SIZE = 8 * CORE_SPACE: then we split this window to assign
    // each core its available space for iDMA transfers

    if (core_id == 0) {
        l1_addr[0]     = (uint32_t) pi_l1_malloc(0, TOT_SIZE);
        l1_dst_addr[0] = (uint32_t) pi_l1_malloc(0, TOT_SIZE);
        l2_addr[0]     = (uint32_t) pi_l2_malloc(TOT_SIZE);
    }
    pi_cl_team_barrier();
    // The following rt_team_barrier is needed so that
    // no core can assign its address range until the mallocs are executed

    l1_addr[core_id] = l1_addr[0] + core_id * CORE_SPACE;
    l1_dst_addr[core_id] = l1_dst_addr[0] + core_id * CORE_SPACE;
    l2_addr[core_id] = l2_addr[0] + core_id * CORE_SPACE;

    PRINTF ("Core %d: l1_addr = %8x \n", core_id, l1_addr[core_id]);
    PRINTF ("Core %d: l1_dst_addr = %8x \n", core_id, l1_dst_addr[core_id]);
    PRINTF ("Core %d: l2_addr = %8x \n", core_id, l2_addr[core_id]);

    // The following rt_team_barrier is needed so that
    // no core can start executing until all address ranges have been assigned
    pi_cl_team_barrier();
}

void free_allocated_memory () {
    pi_cl_team_barrier();
    // Only Core 0 takes care of freeing the allocated memory, since it's the one
    // allocated it at the beginning of the test
    if (pi_core_id() == 0) {
        pi_l1_free(0, l1_addr, TOT_SIZE);
        pi_l1_free(0, l1_dst_addr, TOT_SIZE);
        pi_l2_free(l2_addr, TOT_SIZE);
    }
}

static void pe_entry(void *arg)
{
    int *errors = (int *)arg;
    allocate_mem_to_cores();
#ifdef MULTI_CORE_P
    plp_idma_enable_clk();
    idma_task();
    pi_cl_team_barrier();
    plp_idma_disable_clk();
#elif MULTI_CORE_S
    pi_cl_team_critical_enter();
    idma_task();
    pi_cl_team_critical_exit();
#else
    if (pi_core_id() == 0) {
        idma_task();
    }
#endif
    free_allocated_memory();
}

static int check_fork()
{
  unsigned int var = 0;
  pi_cl_team_fork(pi_cl_team_nb_cores(), pe_entry, (void *)&var);
  return (1<<pi_cl_team_nb_cores()) - 1 != var;
}

static void cluster_entry(void *arg)
{
    check_fork((int) pi_cl_team_nb_cores());
}

static int test_task_sync()
{
    struct pi_device cluster_dev;
    struct pi_cluster_conf conf;
    struct pi_cluster_task task;
    int errors = 0;

    pi_cluster_conf_init(&conf);
    conf.id = 0;

    pi_open_from_conf(&cluster_dev, &conf);
    
    pi_cluster_open(&cluster_dev);

    pi_cluster_task(&task, &cluster_entry, NULL);
    pi_cluster_send_task_to_cl(&cluster_dev, &task);
    pi_cluster_close(&cluster_dev);
    errors = glob_errors;
    return errors;
}

int test_entry()
{
  int errors = 0;

  printf("Starting test\n");

  errors += test_task_sync();

  if (errors)
    printf("Test failure\n");
  else
    printf("Test success\n");

  return errors;
}

void test_kickoff(void *arg)
{
  int ret = test_entry();
  pmsis_exit(ret);
}


int main () {
    return pmsis_kickoff((void *)test_kickoff);
}