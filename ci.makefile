
SHELL = bash
THREADS ?= 1
TEST_THREADS ?= 32
override OPT += --db --db-info=$(CURDIR)/db_info.txt --debug
ifdef USE_TEST
CONFIG ?= pulpissimo-rtl@system=pulpissimo:platform=rtl;wolfe-rtl@template=wolfe:platform=rtl;quentin-rtl@template=quentin:platform=rtl;vivosoc3-rtl@template=vivosoc3:platform=rtl;gap-rtl@template=gap:platform=rtl
else
ifndef CONFIG
CONFIG += pulpissimo@user_config_file=$(CURDIR)/configs/json/pulpissimo.json
CONFIG += pulpissimo-riscy@user_config_file=$(CURDIR)/configs/json/pulpissimo-riscy.json
CONFIG += pulpissimo-zeroriscy@user_config_file=$(CURDIR)/configs/json/pulpissimo-zeroriscy.json
CONFIG += pulpissimo-microriscy@user_config_file=$(CURDIR)/configs/json/pulpissimo-microriscy.json
CONFIG += devchip@system=devchip
CONFIG += wolfe@system=wolfe
CONFIG += pulp@user_config_file=$(CURDIR)/configs/json/pulp.json
CONFIG += quentin@system=quentin
CONFIG += gap@system=gap
CONFIG += oprecompkw@system=oprecompkw
CONFIG += vivosoc3@system=vivosoc3
CONFIG += honey@system=honey
CONFIG += bigpulp@system=bigpulp
endif
endif

export PULP_CONFIGS=$(CONFIG)
ifndef PULP_DB_MYSQL
#export PULP_DB_FILE=/home/jenkins/reports/pulpdb
endif

ifdef BRANCH
override OPT += --branch=$(BRANCH)
endif

SDK_TAG ?= dev
SDK_NAME ?= pulp-sdk
SDK_FULL_NAME = $(SDK_NAME)-$(SDK_TAG)

all: pulp-tools deps checkout build deploy

deploy:
	source init.sh && plpbuild --p sdk deploy $(OPT)

rt:
	source init.sh && plpbuild --g runtime build --threads $(THREADS) $(OPT)

build:
	source init.sh && plpbuild --p sdk build --threads $(THREADS) $(OPT)

checkout:
	source init.sh && plpbuild --p sdk checkout $(OPT)

deps:
	source init.sh && plpbuild --p sdk deps $(OPT)

clean:
	source init.sh && plpbuild --p sdk clean $(OPT)

tar:
	mkdir -p artifacts
	rm -f artifacts/$(SDK_FULL_NAME)
	ln -s $(CURDIR)/pkg/sdk/dev artifacts/$(SDK_FULL_NAME)
	cd artifacts && tar -jhcvf $(SDK_FULL_NAME).tar.bz2 $(SDK_FULL_NAME)




test-all: pulp-tools test-platform test-deps test-checkout test-run

test-run:
	source init.sh && plpbuild --p tests env && source sourceme.sh && \
	  plpbuild --p tests test --threads $(TEST_THREADS) $(OPT) --env=sdk_validation \
	    --commit=`git rev-parse HEAD`

	touch report.xlsx

	#source init.sh && source sourceme.sh && \
	#  plpdb tests --build=`cat $(CURDIR)/db_info.txt | grep tests.build.id= | sed s/tests.build.id=//` \
	#    --mail="SDK regression report" --xls=report.xlsx --branch $(BRANCH) --config=$$CONFIG \
	#    --url=$(BUILD_URL) --author-email=`git show -s --pretty=%ae` --env=sdk_validation

	#&& \
	#  plpdb check_reg --build=`cat $(CURDIR)/db_info.txt | grep tests.build.id= | sed s/tests.build.id=//` \
	#    --branch $(BRANCH) --config=$$CONFIG --env=sdk_validation


test-checkout:
	source init.sh && plpbuild --p tests checkout $(OPT)

test-deps:
	source init.sh && plpbuild --p tests deps $(OPT)

test-platform:
	source init.sh && plpbuild --p tests env && source sourceme.sh && ./get-platform


pulp-tools:
	git submodule update --init


report: pulp-tools
	source init.sh && plpdb tests --xls=report.xls --mail

.PHONY: deps checkout build deploy all pulp-tools doc
