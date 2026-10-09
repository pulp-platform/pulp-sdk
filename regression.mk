# Bare-metal regression tests, built through the pulp-runtime compat layer.
# Point REGRESSIONS at an existing checkout to use it instead of the submodule.

REGRESSIONS        ?= $(PULP_SDK_HOME)/regression_tests
BWRUNTEST          ?= $(PULP_SDK_HOME)/scripts/bwruntests.py
REGRESSION_TIMEOUT ?= 3600
REGRESSION_PROCS   ?= 2
REGRESSION_OUT     ?= $(REGRESSIONS)

# $(call run_regression,<suite>.yaml)
define run_regression
	@test -f $(REGRESSIONS)/$(1) || { echo "No $(1) in $(REGRESSIONS) -- run 'make regression-init'"; exit 1; }
	cd $(REGRESSIONS) && $(BWRUNTEST) --proc-verbose -v --yaml \
		-t $(REGRESSION_TIMEOUT) --max-procs $(2) \
		--report-junit --output $(REGRESSION_OUT)/$(basename $(1)).xml $(REGRESSIONS)/$(1)
endef

.PHONY: regression-init
regression-init:
	git submodule update --init --recursive -- $(REGRESSIONS)

# Fast-forward to the tip of the tracked branch. --remote must not recurse, or it
# also moves the nested pins (pulp-nnx, redmule-golden-model) off their commits.
.PHONY: regression-update

VENV := venv

export PATH := $(abspath $(VENV))/bin:$(PATH)

regression-venv:
	python3 -m venv $(VENV) && \
	$(VENV)/bin/python -m pip install -U pip && \
	$(VENV)/bin/python -m pip install junit-xml pyyaml -r tools/gapy/requirements.txt

regression-update:
	git submodule update --init --remote -- $(REGRESSIONS)
	git -C $(REGRESSIONS) submodule update --init --recursive

.PHONY: test-regression
test-regression: regression-venv
	$(call run_regression,pulp_cluster-subset.yaml, ${REGRESSION_PROCS})

.PHONY: test-cluster
test-cluster: regression-venv
	$(call run_regression,pulp_cluster.yaml, ${REGRESSION_PROCS})

.PHONY: test-mchan
test-mchan: regression-venv
	$(call run_regression,pulp_cluster-mchan-tests.yaml, ${REGRESSION_PROCS})

.PHONY: test-idma
# Run with just 1 process since the same test is launched multiple times with different flags
test-idma: regression-venv
	$(call run_regression,idma-tests.yaml, 1)

.PHONY: test-par-bare
test-par-bare: regression-venv
	$(call run_regression,parallel-bare-tests.yaml, ${REGRESSION_PROCS})

.PHONY: test-fpu
test-fpu: regression-venv
	$(call run_regression,fpu_tests.yaml, ${REGRESSION_PROCS})

.PHONY: test-tcdm
test-tcdm: regression-venv
	$(call run_regression,tcdm-tests.yaml, ${REGRESSION_PROCS})

# Any suite by name: make test-suite SUITE=riscv-tests.yaml
.PHONY: test-suite
test-suite: regression-venv
	$(if $(SUITE),,$(error Set SUITE, e.g. make test-suite SUITE=riscv-tests.yaml))
	$(call run_regression,$(SUITE), ${REGRESSION_PROCS})
