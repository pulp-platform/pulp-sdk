
all: pulp-tools
	plpbuild --p sdk checkout build --stdout

pulp-tools:
	git submodule update --init

.PHONY: pulp-tools pulpissimo
