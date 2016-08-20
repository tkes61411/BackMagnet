$(BIN_TOP)/$(MODE)/simulation : $(PKG_DIR)/core/$(BIN_DIR)/$(MODE)/simulation
	@if [ ! -d $(dir $@) ]; then \
		mkdir -p $(dir $@); \
	fi
	$(AT)rm -f $@
	@echo "    CP $(notdir $@) $(PRT_MODE)"
	$(AT)cp -f $< $@
	@echo "    RM $(notdir $@) $(PRT_MODE)"
	$(AT)rm -rf pkg/core/bin

