SHELL=bash

all: pulp-tools
	plpbuild --p sdk checkout build --stdout

build: pulp-tools
	plpbuild --p sdk build --stdout

clean: pulp-tools
	plpbuild --p sdk clean

env: pulp-tools
	plpbuild --p sdk env

deps: pulp-tools
	plpbuild --p sdk deps --stdout

cmd: pulp-tools
	source configs/$(CONFIG).sh && plpbuild $(SDK_OPT)

pulp-tools:
	git submodule update --init

.PHONY: pulp-tools pulpissimo
