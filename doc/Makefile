pulp_chip_family = $(shell plpinfo  get --property=pulp_chip_family --silent)
pulp_chip_upper = $(shell echo $(pulp_chip_family) | tr [a-z] [A-Z])

rt:
	cat dox/pulp-rt/pulp-rt.dxy.in | sed s/@CHIP_CONFIG@/CONFIG_$(pulp_chip_upper)/ > dox/pulp-rt/pulp-rt.dxy && cd dox && make && mkdir -p $$PULP_SDK_HOME/doc/runtime/$(pulp_chip_family) && cp -r doc/runtime/html/* $$PULP_SDK_HOME/doc/runtime/$(pulp_chip_family)
	cd $$PULP_SDK_HOME/doc/runtime && rm -f default && ln -s $(pulp_chip_family) default

vp:
	cd 	top && rm -f index.rst
	cd 	top && ln -s vp_top.rst index.rst
	cd top && export DOC_PROJECT="GVSOC" && make html && mkdir -p $$DOC_INSTALL_DIR/vp && cp -r _build/html/* $$DOC_INSTALL_DIR/vp

all: rt
	cd 	top && rm -f index.rst
	cd 	top && ln -s sdk_top.rst index.rst
	cd top && make html && mkdir -p $$PULP_SDK_HOME/doc/sdk && cp -r _build/html/* $$PULP_SDK_HOME/doc/sdk

.PHONY: vp rt