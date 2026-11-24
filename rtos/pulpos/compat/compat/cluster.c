#include "pulp.h"

void synch_barrier(void)
{
#ifdef ARCHI_FC_CID
    if (hal_cluster_id() != ARCHI_FC_CID)
#endif
    {
        // pi_cl_team_fork arms eu_bar_addr(0) with the team mask, so this is the
        // same barrier pulp-runtime's cluster_core_init() set up by hand.
        pi_cl_team_barrier();
    }
}

#if defined(ARCHI_HAS_CLUSTER) && !defined(ARCHI_NO_FC)

static struct pi_device       compat_cl_dev;
static struct pi_cluster_task compat_cl_task;
static int (*compat_cl_entry)(void);
static volatile int           compat_cl_ret;
static int                    compat_cl_open;

static void compat_cl_pe(void *arg)
{
    int ret = compat_cl_entry();
    if (hal_core_id() == 0)
        compat_cl_ret = ret;
}

static void compat_cl_fork(void *arg)
{
    pi_cl_team_fork(ARCHI_CLUSTER_NB_PE, compat_cl_pe, 0);
}

void cluster_start(int cid, int (*entry)())
{
    struct pi_cluster_conf conf;

    compat_cl_entry = entry;
    compat_cl_ret = 0;

    pi_cluster_conf_init(&conf);
    conf.id = cid;
    pi_open_from_conf(&compat_cl_dev, &conf);

    if (pi_cluster_open(&compat_cl_dev))
        return;

    compat_cl_open = 1;
    pi_cluster_send_task_to_cl(&compat_cl_dev,
                               pi_cluster_task(&compat_cl_task, compat_cl_fork, 0));
}

int cluster_wait(int cid)
{
    if (compat_cl_open)
    {
        pi_cluster_close(&compat_cl_dev);
        compat_cl_open = 0;
    }
    return compat_cl_ret;
}

#endif
