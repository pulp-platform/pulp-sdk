dpi-models.checkout:
	git submodule update --init tools/dpi-models

dpi-models.build:
	make -C tools/dpi-models build install BUILD_DIR=$(BUILD_DIR)/dpi-models INSTALL_DIR=$(INSTALL_DIR) TARGET_INSTALL_DIR=$(GAP_SDK_HOME)/install
	
dpi-models.clean:
	make -C tools/dpi-models clean BUILD_DIR=$(BUILD_DIR)/dpi-models INSTALL_DIR=$(INSTALL_DIR) TARGET_INSTALL_DIR=$(GAP_SDK_HOME)/install

dpi-models.all: dpi-models.build

dpi-models.artifact:
	rsync -avR --exclude=".git*" --exclude=__pycache__ tools/dpi-models $(ARTIFACT_PATH)
