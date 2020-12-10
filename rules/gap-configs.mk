gap-configs.checkout:
	git submodule update --init tools/gap-configs

gap-configs.build:
	make -C tools/gap-configs all_scons BUILD_DIR=$(BUILD_DIR)/gap-configs INSTALL_DIR=$(INSTALL_DIR) TARGET_INSTALL_DIR=$(GAP_SDK_HOME)/install
	
gap-configs.clean:
	make -C tools/gap-configs clean BUILD_DIR=$(BUILD_DIR)/gap-configs INSTALL_DIR=$(INSTALL_DIR) TARGET_INSTALL_DIR=$(GAP_SDK_HOME)/install

gap-configs.all: gap-configs.build

gap-configs.artifact:
	rsync -avR --exclude=".git*" --exclude=__pycache__ --exclude=".sconsign.dblite" tools/gap-configs $(ARTIFACT_PATH)
