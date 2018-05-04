
ENV_INSTALL_FILES += setup.sh
ENV_INSTALL_FILES += gap.sh
ENV_INSTALL_FILES += env/setup.sh
ENV_INSTALL_FILES += env/setup.csh
ENV_INSTALL_FILES += $(shell find -L configs -name "*.sh")
ENV_INSTALL_FILES += $(shell find -L configs -name "*.json")

define declareEnvInstallFile

$(PULP_SDK_HOME)/$(1): $(1)
	install -D $(1) $$@

INSTALL_HEADERS += $(PULP_SDK_HOME)/$(1)

endef

$(foreach file, $(ENV_INSTALL_FILES), $(eval $(call declareEnvInstallFile,$(file))))

all: $(INSTALL_HEADERS)