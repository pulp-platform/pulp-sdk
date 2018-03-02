SHELL=bash

all: pulp-tools
	plpbuild --p sdk checkout build --stdout

cmd: pulp-tools
	source configs/$(CONFIG).sh && plpbuild $(SDK_OPT)

pulp-tools:
	git submodule update --init

.PHONY: pulp-tools pulpissimo
