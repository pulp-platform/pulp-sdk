MODULES = rtos/pulpos/common rtos/pmsis/pmsis_api rtos/pmsis/pmsis_bsp

ifeq '$(TARGET_CHIP_FAMILY)' 'PULP'
MODULES += rtos/pulpos/pulp rtos/pulpos/pulp_archi rtos/pulpos/pulp_hal
endif


pulpos.checkout.deps:

pulpos.checkout:
	git submodule update --init $(MODULES)

pulpos.checkout.all: pulpos.checkout pulpos.checkout.deps

pulpos.build.deps:

pulpos.build:
	
pulpos.clean:

pulpos.src:
	make -C tools/pulpos src INSTALL_DIR=$(ARTIFACT_PATH)/tools/pulpos

pulpos.build.all: pulpos.build.deps pulpos.build

pulpos.all: pulpos.checkout.all pulpos.build.all

pulpos.artifact:
	rsync -avR --exclude=".git*" --exclude=__pycache__ rtos/pulpos/common rtos/pulpos/pulp rtos/pulpos/pulp_archi rtos/pulpos/pulp_hal rtos/pmsis $(ARTIFACT_PATH)
