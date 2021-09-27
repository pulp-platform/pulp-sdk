gvsoc.checkout.deps: json-tools.checkout gap-configs.checkout dpi-models.checkout pulp-debug-bridge.checkout
	git submodule update --init rtos/pulpos/pulp_archi

gvsoc.checkout:
	git submodule update --init tools/gvsoc

gvsoc.checkout.all: gvsoc.checkout gvsoc.checkout.deps

gvsoc.build.deps: json-tools.all gap-configs.all pulp-debug-bridge.all dpi-models.all

gvsoc.build:
	for gv_module in $$GVSOC_MODULES; do \
		make -C $$gv_module build BUILD_DIR=$(BUILD_DIR)/gvsoc INSTALL_DIR=$(INSTALL_DIR) TARGET_INSTALL_DIR=$(GAP_SDK_HOME)/install; \
	done
	
gvsoc.clean:
	make -C tools/gvsoc/common clean BUILD_DIR=$(BUILD_DIR)/gvsoc INSTALL_DIR=$(INSTALL_DIR) TARGET_INSTALL_DIR=$(GAP_SDK_HOME)/install

gvsoc.src:
	make -C tools/gvsoc src INSTALL_DIR=$(ARTIFACT_PATH)/tools/gvsoc

gvsoc.build.all: gvsoc.build.deps gvsoc.build

gvsoc.all: gvsoc.checkout.all gvsoc.build.all

gvsoc.artifact: json-tools.artifact gap-configs.artifact dpi-models.artifact pulp-debug-bridge.artifact
	rsync -avR --exclude=".git*" --exclude=__pycache__ tools/gvsoc/common $(ARTIFACT_PATH)
	rsync -avR --exclude=".git*" --exclude=__pycache__ tools/gvsoc/pulp $(ARTIFACT_PATH)
	rsync -avR --exclude=".git*" --exclude=__pycache__ tools/gapy $(ARTIFACT_PATH)
	rsync -avR --exclude=".git*" --exclude=__pycache__ rtos/pulpos/pulp_archi $(ARTIFACT_PATH)
