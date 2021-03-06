#
# Copyright 2008-2012 Ettus Research LLC
#

##################################################
# Project Setup
##################################################
TOP_MODULE = x300
# NAME = <Input arg> 
# PART_ID = <Input arg> 

##################################################
# Include other makefiles
##################################################

BASE_DIR = $(abspath ..)
IP_DIR = $(abspath ./ip)
include $(BASE_DIR)/../tools/make/viv_design_builder.mak

include $(IP_DIR)/Makefile.inc
include coregen_dsp/Makefile.srcs
include $(LIB_DIR)/control/Makefile.srcs
include $(LIB_DIR)/fifo/Makefile.srcs
include $(LIB_DIR)/simple_gemac/Makefile.srcs
include $(LIB_DIR)/timing/Makefile.srcs
include $(LIB_DIR)/zpu/Makefile.srcs
include $(LIB_DIR)/wishbone/Makefile.srcs
include $(LIB_DIR)/packet_proc/Makefile.srcs
include $(LIB_DIR)/vita/Makefile.srcs
include $(LIB_DIR)/xge/Makefile.srcs
include $(LIB_DIR)/xge_interface/Makefile.srcs
include $(LIB_DIR)/dsp/Makefile.srcs
include $(LIB_DIR)/io_port2/Makefile.srcs
include $(LIB_DIR)/axi/Makefile.srcs
include $(LIB_DIR)/radio/Makefile.srcs
include $(LIB_DIR)/io_cap_gen/Makefile.srcs

##################################################
# Sources
##################################################
TOP_SRCS = \
x300.v \
x300_pcie_int.v \
x300_core.v \
x300_zpu_config.vhd \
soft_ctrl.v \
capture_ddrlvds.v \
gen_ddrlvds.v \
bus_int.v \
x300.xdc \
timing.xdc

ifdef BUILD_10G
ETH_SRCS += $(XGE_SRCS) $(XGE_INTERFACE_SRCS) $(abspath x300_10ge.xdc) $(TEN_GIGE_PHY_SRCS) $(IP_TEN_GIGE_PHY_XCI_SRCS)
endif

ifdef BUILD_1G
ETH_SRCS += $(abspath x300_1ge.xdc) $(ONE_GIGE_PHY_SRCS) $(IP_ONE_GIGE_PHY_XCI_SRCS)
endif

ifdef ETH10G_PORT0
ETH_SRCS += $(abspath x300_10ge_port0.xdc)
endif

ifdef ETH10G_PORT1
ETH_SRCS += $(abspath x300_10ge_port1.xdc)
endif

ifndef NO_DRAM_FIFOS
DRAM_SRCS = $(IP_DRAM_XCI_SRCS) $(abspath x300_dram.xdc)
else
DRAM_SRCS =
endif

DESIGN_SRCS = $(abspath $(TOP_SRCS)) $(FIFO_SRCS) \
$(CONTROL_LIB_SRCS) $(SDR_LIB_SRCS) $(SERDES_SRCS) \
$(SIMPLE_GEMAC_SRCS) $(TIMING_SRCS) $(COREGEN_DSP_SRCS) \
$(VRT_SRCS) $(UDP_SRCS) $(COREGEN_SRCS) $(EXTRAM_SRCS) \
$(ZPU_CORE_SRCS) $(WISHBONE_SRCS) \
$(ETH_SRCS) $(IOPORT2_SRCS) $(COREGEN_CHIPSCOPE_SRCS) \
$(PACKET_PROC_SRCS) $(VITA_SRCS) $(DSP_SRCS) $(DRAM_SRCS) \
$(AXI_SRCS) $(RADIO_SRCS) $(CAP_GEN_GENERIC_SRCS) $(IP_XCI_SRCS)


##################################################
# Dependency Targets
##################################################
.SECONDEXPANSION:

# DESIGN_SRCS and VERILOG_DEFS must be defined
bin: .prereqs $$(DESIGN_SRCS) ip
	$(call BUILD_VIVADO_DESIGN,$(abspath ./build_x300.tcl),$(TOP_MODULE),$(PART_ID))

.PHONY: bin
