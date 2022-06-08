ifeq '$(pulp_chip)' 'siracusa'
IMPLEMENTATIONS += pulp/chips/siracusa/apb_soc_impl
pulp/chips/siracusa/apb_soc_impl_SRCS = pulp/chips/siracusa/apb_soc_impl.cpp

VP_INSTALL_TARGETS += $(INSTALL_DIR)/python/pulp/chips/siracusa/rom.bin
VP_INSTALL_TARGETS += $(INSTALL_DIR)/python/pulp/chips/siracusa/debug_rom.bin

$(INSTALL_DIR)/python/pulp/chips/siracusa/rom.bin: $(CURDIR)/pulp/chips/siracusa/rom.bin
	install -D $^ $@

$(INSTALL_DIR)/python/pulp/chips/siracusa/debug_rom.bin: $(CURDIR)/pulp/chips/siracusa/debug_rom.bin
	install -D $^ $@

endif
