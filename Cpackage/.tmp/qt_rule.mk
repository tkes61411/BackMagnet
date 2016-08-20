$(TMP_TOP)/$(MODE)/$(TMP_QT)/$(PKG_DIR)/core/$(INC_DIR)/%.h.cpp : $(PKG_DIR)/core/$(INC_DIR)/%.h
	@if [ ! -d $(dir $@) ]; then \
		mkdir -p $(dir $@); \
	fi
	$(AT)rm -f $@
	@echo "    MOC $(notdir $@) $(PRT_MODE)"
	$(AT)$(MOC) $(if $(PKG_core_MODE_$(MODE)_MOC_FLAGS),$(PKG_core_MODE_$(MODE)_MOC_FLAGS),$(if $(PKG_core_MOC_FLAGS),$(PKG_core_MOC_FLAGS),$(if $(MODE_$(MODE)_MOC_FLAGS),$(MODE_$(MODE)_MOC_FLAGS),$(CMN_MOC_FLAGS)))) -p core/$(INC_DIR) -o $@ $<

$(TMP_TOP)/$(MODE)/$(TMP_DEP)/$(PKG_DIR)/core/$(INC_DIR)/%.cpp.d : $(TMP_TOP)/$(MODE)/$(TMP_QT)/$(PKG_DIR)/core/$(INC_DIR)/%.cpp
	@if [ ! -d $(dir $@) ]; then \
		mkdir -p $(dir $@); \
	fi
	$(AT)rm -f $@
	@echo "    DEP $(notdir $@) $(PRT_MODE)"
	$(AT)$(CXX) -MM $(addprefix -I,$(PKG_DIR)) $(addprefix -I,$(PKG_DIR)/core/$(INC_DIR)) $(addprefix -I,$(PKG_core_MODE_$(MODE)_INCS)) $(addprefix -I,$(PKG_core_INCS)) $(MODE_INCS) $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,$(TMP_TOP)/$(MODE)/$(TMP_OBJ)/$(TMP_STA)/$(PKG_DIR)/core/$(SRC_DIR)/$(notdir $<).o $@ : ,g' < $@.$$$$ > $@; \
	sed 's,\($*\)\.o[ :]*,$(TMP_TOP)/$(MODE)/$(TMP_OBJ)/$(TMP_DYN)/$(PKG_DIR)/core/$(SRC_DIR)/$(notdir $<).o $@ : ,g' < $@.$$$$ >> $@; \
	rm $@.$$$$

$(TMP_TOP)/$(MODE)/$(TMP_OBJ)/$(TMP_STA)/$(PKG_DIR)/core/$(INC_DIR)/%.cpp.o : $(TMP_TOP)/$(MODE)/$(TMP_QT)/$(PKG_DIR)/core/$(INC_DIR)/%.cpp
	@if [ ! -d $(dir $@) ]; then \
		mkdir -p $(dir $@); \
	fi
	$(AT)rm -f $@
	@echo "    CXX $(notdir $<) $(PRT_MODE)"
	$(AT)$(CXX) $(if $(PKG_core_MODE_$(MODE)_C_FLAGS),$(PKG_core_MODE_$(MODE)_C_FLAGS),$(if $(PKG_core_C_FLAGS),$(PKG_core_C_FLAGS),$(if $(MODE_$(MODE)_C_FLAGS),$(MODE_$(MODE)_C_FLAGS),$(CMN_C_FLAGS)))) $(addprefix -I,$(PKG_DIR)) $(addprefix -I,$(PKG_DIR)/core/$(INC_DIR)) $(addprefix -I,$(PKG_core_MODE_$(MODE)_INCS)) $(addprefix -I,$(PKG_core_INCS)) $(MODE_INCS) -c -o $@ $<

$(TMP_TOP)/$(MODE)/$(TMP_OBJ)/$(TMP_DYN)/$(PKG_DIR)/core/$(INC_DIR)/%.cpp.o : $(TMP_TOP)/$(MODE)/$(TMP_QT)/$(PKG_DIR)/core/$(INC_DIR)/%.cpp
	@if [ ! -d $(dir $@) ]; then \
		mkdir -p $(dir $@); \
	fi
	$(AT)rm -f $@
	@echo "    CXX $(notdir $<) $(PRT_MODE) ($(C_DYN_FLAGS))"
	$(AT)$(CXX) $(C_DYN_FLAGS) $(if $(PKG_core_MODE_$(MODE)_C_FLAGS),$(PKG_core_MODE_$(MODE)_C_FLAGS),$(if $(PKG_core_C_FLAGS),$(PKG_core_C_FLAGS),$(if $(MODE_$(MODE)_C_FLAGS),$(MODE_$(MODE)_C_FLAGS),$(CMN_C_FLAGS)))) $(addprefix -I,$(PKG_DIR)) $(addprefix -I,$(PKG_DIR)/core/$(INC_DIR)) $(addprefix -I,$(PKG_core_MODE_$(MODE)_INCS)) $(addprefix -I,$(PKG_core_INCS)) $(MODE_INCS) -c -o $@ $<

