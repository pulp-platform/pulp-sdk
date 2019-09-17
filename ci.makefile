SHELL = bash
THREADS ?= 1
MAX_TIMEOUT ?= 3600
TEST_THREADS ?= 16
PMSIS_KERNELS = 1
override TEST_OPT += --stdout
ifdef USE_DB
override OPT += --db --db-info=$(CURDIR)/db_info.txt --debug
endif
override OPT += --debug
ifdef USE_TEST
ifndef CONFIG
CONFIG += pulpissimo-rtl@config_file=systems/pulpissimo.json:platform=rtl
CONFIG += wolfe-rtl@config_file=systems/wolfe.json:platform=rtl
CONFIG += quentin-rtl@config_file=systems/quentin.json:platform=rtl
CONFIG += vivosoc3-rtl@config_file=systems/vivosoc3.json:platform=rtl
CONFIG += gap-rtl@config_file=systems/gap.json:platform=rtl
endif
else
ifndef CONFIG
CONFIG += pulpissimo@config_file=chips/pulpissimo/pulpissimo.json
CONFIG += pulpissimo_v1@config_file=chips/pulpissimo_v1/pulpissimo_v1.json
CONFIG += pulpissimo-riscy@config_file=chips/pulpissimo-riscy/pulpissimo-riscy.json
CONFIG += pulpissimo-zeroriscy@config_file=chips/pulpissimo-zeroriscy/pulpissimo-zeroriscy.json
CONFIG += pulpissimo-microriscy@config_file=chips/pulpissimo-microriscy/pulpissimo-microriscy.json
CONFIG += wolfe@config_file=chips/wolfe/wolfe.json
CONFIG += pulp@config_file=chips/pulp/pulp.json
CONFIG += pulp_v1@config_file=chips/pulp_v1/pulp_v1.json
CONFIG += vega@config_file=chips/vega/vega.json
CONFIG += gapuino@config_file=boards/gapuino.json
CONFIG += gapoc_a@config_file=boards/gapoc_a.json
CONFIG += gapuino_revb@config_file=boards/gapuino_revb.json
CONFIG += gapuino_revc@config_file=boards/gapuino_revc.json
CONFIG += oprecompkw@config_file=chips/oprecompkw/oprecompkw.json
CONFIG += oprecompkw_sa@config_file=chips/oprecompkw_sa/oprecompkw_sa.json
CONFIG += vivosoc3@config_file=chips/vivosoc3/vivosoc3.json
CONFIG += vivosoc3_5@config_file=chips/vivosoc3_5/vivosoc3_5.json
CONFIG += arnold@config_file=chips/arnold/arnold.json
CONFIG += bigpulp-standalone@config_file=chips/bigpulp-standalone/bigpulp.json
CONFIG += multino@config_file=chips/multino/multino.json
CONFIG += vivosoc3_1@config_file=chips/vivosoc3_1/vivosoc3_1.json
endif
endif
DISTRIBS ?= CentOS_7 Ubuntu_16


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

sdk_users:
	if [ ! -e sdk_users ]; then \
	  git clone git@github.com:pulp-platform/pulp-sdk-release.git sdk_users; \
	else \
	  cd sdk_users; \
	  git pull; \
	fi

ifneq '$(profile)' ''
profileName=$(profile)-
else
profile=default
endif

tag_prepare: sdk_users
	source init.sh && for distrib in $(DISTRIBS); do \
	  downloaders=`plpbuild --p sdk downloader --distrib=$$distrib --version=$(profileName)$(version)`; \
	  for downloader in $$downloaders; do \
	    mkdir -p sdk_users/artifacts/$(profile)/$$distrib; \
	    chmod guo+x $$downloader; \
	    mv $$downloader sdk_users/artifacts/$(profile)/$$distrib; \
	    done; \
	  done

tag_commit:
	git tag $(profileName)$(version)
	git push --tags

tag_user_commit:
	cd sdk_users; \
	git add -A; \
	git commit -a -m "Added tag $(profileName)$(version)"; \
	git push

tag: tag_prepare tag_user_commit tag_commit



test-prepare: pulp-tools test-platform test-deps test-checkout test-env

test-env:
	source init.sh && plpbuild --p tests env

test-all: pulp-tools test-platform test-deps test-checkout test-env test-run

test-run:
	source init.sh && source sourceme.sh && \
	  plpbuild --p tests test --threads $(TEST_THREADS) $(OPT) $(TEST_OPT) --env=sdk_validation \
	    --commit=`git rev-parse HEAD` --max-timeout=$(MAX_TIMEOUT)

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
ifdef BUILD_SDK
	source init.sh && plpbuild --p sdk deps checkout build env $(OPT)
endif
	source init.sh && plpbuild --p tests deps $(OPT)

test-platform:
ifndef USE_EXTERNAL_PLATFORM
	source init.sh && plpbuild --p tests env && source sourceme.sh && ./get-platform
endif


pulp-tools:
	git submodule update --init


report: pulp-tools
	source init.sh && plpdb tests --xls=report.xls --mail

.PHONY: deps checkout build deploy all pulp-tools doc sdk_users
