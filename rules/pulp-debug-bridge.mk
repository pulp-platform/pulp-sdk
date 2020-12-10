pulp-debug-bridge.checkout:
	git submodule update --init tools/pulp-debug-bridge

pulp-debug-bridge.build:
	make -C tools/pulp-debug-bridge build install BUILD_DIR=$(BUILD_DIR)/pulp-debug-bridge INSTALL_DIR=$(INSTALL_DIR) TARGET_INSTALL_DIR=$(GAP_SDK_HOME)/install
	
pulp-debug-bridge.clean:
	make -C tools/pulp-debug-bridge clean BUILD_DIR=$(BUILD_DIR)/pulp-debug-bridge INSTALL_DIR=$(INSTALL_DIR) TARGET_INSTALL_DIR=$(GAP_SDK_HOME)/install

pulp-debug-bridge.all: pulp-debug-bridge.build

pulp-debug-bridge.artifact:
	rsync -avR --exclude=".git*" --exclude=__pycache__ tools/pulp-debug-bridge $(ARTIFACT_PATH)
