json-tools.checkout:
	git submodule update --init tools/json-tools

json-tools.build:
	make -C tools/json-tools all install BUILD_DIR=$(BUILD_DIR)/json-tools INSTALL_DIR=$(INSTALL_DIR) TARGET_INSTALL_DIR=$(GAP_SDK_HOME)/install
	
json-tools.clean:
	make -C tools/json-tools clean BUILD_DIR=$(BUILD_DIR)/json-tools INSTALL_DIR=$(INSTALL_DIR) TARGET_INSTALL_DIR=$(GAP_SDK_HOME)/install

json-tools.all: json-tools.build

json-tools.artifact:
	rsync -avR --exclude=".git*" --exclude=__pycache__ tools/json-tools $(ARTIFACT_PATH)
