ext/xtensor:
	mkdir -p ext/xtensor
	rm -rf ext/xtensor/*
	git clone https://github.com/xtensor-stack/xtl     ext/xtensor/xtl
	git clone https://github.com/xtensor-stack/xtensor ext/xtensor/xtensor
	git clone https://github.com/xtensor-stack/xsimd   ext/xtensor/xsimd
	pushd ext/xtensor/xtl; git checkout tags/0.6.14; popd
	pushd ext/xtensor/xtensor; git checkout tags/0.21.5; popd
	pushd ext/xtensor/xsimd; git checkout tags/7.4.7; popd
	mkdir ext/xtensor/include
	mv ext/xtensor/xtl/include/xtl ext/xtensor/include/
	mv ext/xtensor/xtensor/include/xtensor ext/xtensor/include/
	mv ext/xtensor/xsimd/include/xsimd ext/xtensor/include/
	rm -rf ext/xtensor/x*

gvsoc.ext: ext/xtensor

gvsoc.checkout.deps: json-tools.checkout gap-configs.checkout dpi-models.checkout pulp-debug-bridge.checkout gvsoc.ext
	git submodule update --init rtos/pulpos/pulp_archi

gvsoc.checkout:
	git submodule update --init tools/gvsoc

gvsoc.checkout.all: gvsoc.checkout gvsoc.checkout.deps

gvsoc.build.deps: json-tools.all gap-configs.all pulp-debug-bridge.all dpi-models.all

gvsoc.build: ext/xtensor
	for gv_module in $$GVSOC_MODULES; do \
		make -C $$gv_module build BUILD_DIR=$(BUILD_DIR)/gvsoc INSTALL_DIR=$(INSTALL_DIR) TARGET_INSTALL_DIR=$(GAP_SDK_HOME)/install; \
		if [ $$? -ne 0 ]; then exit 1; fi; \
	done
	
gvsoc.clean:
	make -C tools/gvsoc/common clean BUILD_DIR=$(BUILD_DIR)/gvsoc INSTALL_DIR=$(INSTALL_DIR) TARGET_INSTALL_DIR=$(GAP_SDK_HOME)/install

gvsoc.src:
	make -C tools/gvsoc src INSTALL_DIR=$(ARTIFACT_PATH)/tools/gvsoc

gvsoc.build.all: gvsoc.build.deps gvsoc.build

gvsoc.all: gvsoc.checkout.all gvsoc.build.all

gvsoc.artifact: json-tools.artifact gap-configs.artifact dpi-models.artifact pulp-debug-bridge.artifact
	rsync -avR --exclude=".git*" --exclude="__pycache__/" rtos/pulpos/pulp_archi tools/gapy tools/gvsoc/pulp tools/gvsoc/common $(ARTIFACT_PATH)
