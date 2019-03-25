SHELL=bash

MODULES ?= --p sdk

all: pulp-tools
	./pulp-tools/bin/plpbuild $(MODULES) checkout build env --stdout

build: pulp-tools
	./pulp-tools/bin/plpbuild $(MODULES) build --stdout

distclean:
	-./pulp-tools/bin/plpbuild --p sdk distclean --stdout
	rm -rf build

clean: pulp-tools
	./pulp-tools/bin/plpbuild $(MODULES) clean

env: pulp-tools
	./pulp-tools/bin/plpbuild $(MODULES) env

deps: pulp-tools
	./pulp-tools/bin/plpbuild --p sdk deps --stdout

cmd: pulp-tools
	source configs/$(CONFIG).sh && ./pulp-tools/bin/plpbuild $(SDK_OPT)

pulp-tools:
	git submodule update --init

.PHONY: pulp-tools pulpissimo
