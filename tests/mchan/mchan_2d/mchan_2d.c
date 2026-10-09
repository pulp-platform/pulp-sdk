#include "mchan_2d.h"

int glob_errors = 0;
uint32_t l1_addr[8] = {0};
uint32_t l2_addr[8] = {0};
uint32_t l1_dst_addr[8] = {0};

int mchan_2d(transfer_2d transfer_params, int core_id, int ext2loc) {
    int error = 0;
    volatile uint8_t *l1_ptr, *l2_ptr;
    uint32_t offset_2d;

    // Adapting transfer parameters
    uint32_t num_reps = 0;
    uint32_t size = transfer_params.size_2d;
    
    uint32_t l1_stride = transfer_params.l1_stride_2d;
    uint32_t l1_length = transfer_params.l1_length_2d;
    uint32_t l2_stride = transfer_params.l2_stride_2d;
    uint32_t l2_length = transfer_params.l2_length_2d;

    if (ext2loc == 1) {
        l1_stride = transfer_params.l1_stride_2d;
        l1_length = transfer_params.l1_length_2d;
        num_reps = size / l1_length;
        l2_stride = 1;
        l2_length = 1;
    } else {
        l1_stride = 1;
        l1_length = 1;
        l2_stride = transfer_params.l2_stride_2d;
        l2_length = transfer_params.l2_length_2d;
        num_reps = size / l2_length;
    }

    l1_ptr = (uint8_t*) l1_addr[core_id];
    l2_ptr = (uint8_t*) l2_addr[core_id];
    int j = 0;

    // Filling L1 and L2 regions with test data
    for (int i = 0; i < CORE_SPACE; i++) {
        l1_ptr[i] = (uint8_t)(i & 0xFF);
        l2_ptr[i] = (uint8_t)((i+1) & 0xFF);
    }

    PRINTF ("Passing parameters: \n");
    if (ext2loc==1) {
        PRINTF ("TRANSFER TOWARDS L1 --> L1_Addr: 0x%8x | L2_Addr: 0x%8x | Size: %d | Stride: %d | Length: %d \n", 
            l1_addr[core_id], l2_addr[core_id], size, l1_stride, l1_length);    
    } else {
        PRINTF ("TRANSFER TOWARDS L2 --> L1_Addr: 0x%8x | L2_Addr: 0x%8x | Size: %d | Stride: %d | Length: %d \n", 
            l1_addr[core_id], l2_addr[core_id], size, l2_stride, l2_length);    
    }

    if (ext2loc==1) {
        reset_cycle_count();
        start_cycle_count();
        plp_dma_wait(plp_dma_extToL1_2d(l1_addr[core_id], l2_addr[core_id], size, l1_stride, l1_length));
        stop_cycle_count();
    } else {
        reset_cycle_count();
        start_cycle_count();
        plp_dma_wait(plp_dma_l1ToExt_2d(l2_addr[core_id], l1_addr[core_id], size, l2_stride, l2_length));
        stop_cycle_count();
    }

    PRINTF ("This transfer took %d cycles \n", getcycles());stop_cycle_count();

    // Check the results
    if (ext2loc == 1) {
        // Contiguous access in L2 | Strided access to L1
        int rep_counter = 0;
        int length_counter = 0;
        for (int i = 0; i < size; i++) {
            uint32_t l1_idx = length_counter + rep_counter * l1_stride;
            uint32_t l2_idx = i;
            if (length_counter == (l1_length-1)) {
                length_counter = 0;
                rep_counter ++;
            } else {
                length_counter ++;
            }
            if (l1_ptr[l1_idx] != l2_ptr[l2_idx]) {
                error++;
                if (core_id == 0) {
                    PRINTF ("ERROR: L1[%d] @%8x = %8x vs L2[%d] @%8x = %8x \n", l1_idx, &l1_ptr[l1_idx], 
                            l1_ptr[l1_idx], l2_idx, &l2_ptr[l2_idx], l2_ptr[l2_idx]);
                }
            }
        }
    } else {
        // Contiguous access in L1 | Strided access to L2
        int rep_counter = 0;
        int length_counter = 0;
        for (int i = 0; i < size; i++) {
            uint32_t l1_idx = i;
            uint32_t l2_idx = length_counter + rep_counter * l2_stride;
            if (length_counter == (l2_length-1)) {
                length_counter = 0;
                rep_counter ++;
            } else {
                length_counter ++;
            }

            if (l1_ptr[l1_idx] != l2_ptr[l2_idx]) {
                error++;
                if (core_id == 0) {
                    PRINTF ("ERROR: L1[%d] @%8x = %8x vs L2[%d] @%8x = %8x \n", l1_idx, &l1_ptr[l1_idx], 
                            l1_ptr[l1_idx], l2_idx, &l2_ptr[l2_idx], l2_ptr[l2_idx]);
                }
            }
        }
    }

    return error;
}


void mchan_task() {
    PRINTF ("Core[%d] has entered mchan_task \n", pi_core_id());
    uint32_t size;

    uint32_t transfers_num = NB_TRANSFERS;

    for (int k=0; k < transfers_num; k++) {
        // MCHAN 2d L1 -> L2
        glob_errors += mchan_2d(params_2d[k], pi_core_id(), 0);
        // MCHAN 2d L2 -> L1
        glob_errors += mchan_2d(params_2d[k], pi_core_id(), 1);
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
    mchan_task();
#elif MULTI_CORE_S
    pi_cl_team_critical_enter();
    mchan_task();
    pi_cl_team_critical_exit();
#else
    if (pi_core_id() == 0) {
        mchan_task();
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

  PRINTF("Starting test\n");

  errors += test_task_sync();

  if (errors)
    PRINTF("Test failure\n");
  else
    PRINTF("Test success\n");

  return errors;
}

void test_kickoff(void *arg) {
    int ret=test_entry();
    pmsis_exit(ret);
}

int main () {
    return pmsis_kickoff((void *)test_kickoff);
}