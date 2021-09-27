include $(GVSOC_ISS_PATH)/iss.mk



ifeq '$(TARGET_CHIP)' 'PULP'


# PULP cluster core

$(eval $(call declare_iss_isa_build,pulp/cpu/iss/iss_wrapper/pulp_cluster_core,--core=Pulp_cluster_core@pulp.cpu.iss.pulp_cores,pulp/cpu/iss/pulp_cores.py))


IMPLEMENTATIONS += pulp/cpu/iss/iss_pulp_cluster

pulp/cpu/iss/iss_pulp_cluster_CFLAGS += -DEXTERNAL_ISS_WRAPPER=1 -DPIPELINE_STAGES=2 -DISS_SINGLE_REGFILE -DPCER_VERSION_2 -DPRIV_1_10
pulp/cpu/iss/iss_pulp_cluster_SRCS += $(VP_BUILD_DIR)/pulp/cpu/iss/iss_wrapper/pulp_cluster_core_decoder_gen.cpp pulp/cpu/iss/pulp_iss_wrapper.cpp
pulp/cpu/iss/iss_pulp_cluster_SRCS += $(COMMON_SRCS)
pulp/cpu/iss/iss_pulp_cluster_CFLAGS += $(COMMON_CFLAGS)
pulp/cpu/iss/iss_pulp_cluster_LDFLAGS += $(COMMON_LDFLAGS)




# PULP FC core

$(eval $(call declare_iss_isa_build,pulp/cpu/iss/iss_wrapper/pulp_fc_core,--core=Pulp_fc_core@pulp.cpu.iss.pulp_cores,pulp/cpu/iss/pulp_cores.py))

IMPLEMENTATIONS += pulp/cpu/iss/iss_pulp_fc

pulp/cpu/iss/iss_pulp_fc_CFLAGS += -DSECURE=1 -DEXTERNAL_ISS_WRAPPER=1 -DPIPELINE_STAGES=2 -DISS_SINGLE_REGFILE -DPCER_VERSION_2 -DPRIV_1_10
pulp/cpu/iss/iss_pulp_fc_SRCS += $(VP_BUILD_DIR)/pulp/cpu/iss/iss_wrapper/pulp_fc_core_decoder_gen.cpp pulp/cpu/iss/pulp_iss_wrapper.cpp
pulp/cpu/iss/iss_pulp_fc_SRCS += $(COMMON_SRCS)
pulp/cpu/iss/iss_pulp_fc_CFLAGS += $(COMMON_CFLAGS)
pulp/cpu/iss/iss_pulp_fc_LDFLAGS += $(COMMON_LDFLAGS)


endif