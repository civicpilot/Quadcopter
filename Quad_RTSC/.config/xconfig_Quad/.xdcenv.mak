#
_XDCBUILDCOUNT = 0
ifneq (,$(findstring path,$(_USEXDCENV_)))
override XDCPATH = C:/ti/tirtos_c2000_2_10_01_38/packages;C:/ti/tirtos_c2000_2_10_01_38/products/bios_6_41_00_26/packages;C:/ti/tirtos_c2000_2_10_01_38/products/ipc_3_30_01_12/packages;C:/ti/tirtos_c2000_2_10_01_38/products/ndk_2_24_01_18/packages;C:/ti/tirtos_c2000_2_10_01_38/products/uia_2_00_02_39/packages;C:/ti/bios_6_41_01_36/packages;C:/ti/ccsv6/ccs_base;C:/Users/Chris/git/Quadcopter/Quad_RTSC/.config
override XDCROOT = c:/ti/xdctools_3_30_05_60_core
override XDCBUILDCFG = ./config.bld
endif
ifneq (,$(findstring args,$(_USEXDCENV_)))
override XDCARGS = 
override XDCTARGETS = 
endif
#
ifeq (0,1)
PKGPATH = C:/ti/tirtos_c2000_2_10_01_38/packages;C:/ti/tirtos_c2000_2_10_01_38/products/bios_6_41_00_26/packages;C:/ti/tirtos_c2000_2_10_01_38/products/ipc_3_30_01_12/packages;C:/ti/tirtos_c2000_2_10_01_38/products/ndk_2_24_01_18/packages;C:/ti/tirtos_c2000_2_10_01_38/products/uia_2_00_02_39/packages;C:/ti/bios_6_41_01_36/packages;C:/ti/ccsv6/ccs_base;C:/Users/Chris/git/Quadcopter/Quad_RTSC/.config;c:/ti/xdctools_3_30_05_60_core/packages;..
HOSTOS = Windows
endif
