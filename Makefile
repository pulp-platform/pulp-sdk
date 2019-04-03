SHELL=bash

MODULES ?= --p sdk

all: pulp-tools
	source init.sh && ./pulp-tools/bin/plpbuild $(MODULES) checkout build env --stdout

build: pulp-tools
	source init.sh && ./pulp-tools/bin/plpbuild $(MODULES) build --stdout

distclean: pulp-tools
	-source init.sh && ./pulp-tools/bin/plpbuild --p sdk distclean --stdout
	rm -rf build

clean: pulp-tools
	-source init.sh && ./pulp-tools/bin/plpbuild --p sdk distclean --stdout
	rm -rf build

env: pulp-tools
	source init.sh && ./pulp-tools/bin/plpbuild $(MODULES) env --stdout

deps: pulp-tools
	source init.sh && ./pulp-tools/bin/plpbuild --p sdk deps --stdout

cmd: pulp-tools
	source configs/$(CONFIG).sh && ./pulp-tools/bin/plpbuild $(SDK_OPT)

pulp-tools:
	git submodule update --init

.PHONY: pulp-tools pulpissimo

autotiler: pulp-tools
	export GWT_AUTOTILER=1 && source init.sh && ./scripts/get-autotiler
	export GWT_AUTOTILER=1 && source init.sh && ./pulp-tools/bin/plpbuild --m auto-tiler checkout build --stdout
