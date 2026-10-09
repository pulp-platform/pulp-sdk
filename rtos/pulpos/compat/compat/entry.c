#include "pulp.h"

extern int __real_main(void);

#if defined(ARCHI_NO_FC) && defined(ARCHI_HAS_CLUSTER)

static int compat_main_ret;

static void compat_pe_entry(void *arg)
{
    int ret = __real_main();
    if (pi_core_id() == 0)
        compat_main_ret = ret;
}

static void compat_cluster_entry(void *arg)
{
    pi_cl_team_fork(pi_cl_cluster_nb_cores(), compat_pe_entry, NULL);
}

// pulp-runtime's crt0 runs main on every PE. Reproduce that with the SDK's
// standalone entry: send_task allocates the task stacks and pushes
// pos_set_slave_stack to the dispatcher, which forking directly would skip.
int __wrap_main(void)
{
    struct pi_device cluster_dev;
    struct pi_cluster_conf conf;
    struct pi_cluster_task task;

    pi_cluster_conf_init(&conf);
    conf.id = 0;
    pi_open_from_conf(&cluster_dev, &conf);

    if (pi_cluster_open(&cluster_dev))
        return -1;

    pi_cluster_task(&task, compat_cluster_entry, NULL);
    pi_cluster_send_task_to_cl(&cluster_dev, &task);
    pi_cluster_close(&cluster_dev);

    return compat_main_ret;
}

#else

int __wrap_main(void)
{
    return __real_main();
}

#endif
