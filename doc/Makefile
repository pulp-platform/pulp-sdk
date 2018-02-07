all:
	cd dox && make && mkdir -p $$PULP_SDK_HOME/doc/runtime && cp -r doc/runtime/html/* $$PULP_SDK_HOME/doc/runtime
	cd top && make html && mkdir -p $$PULP_SDK_HOME/doc/sdk && cp -r _build/html/* $$PULP_SDK_HOME/doc/sdk
