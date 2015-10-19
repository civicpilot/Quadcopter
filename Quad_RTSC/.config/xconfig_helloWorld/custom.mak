## THIS IS A GENERATED FILE -- DO NOT EDIT
.configuro: .libraries,28FP linker.cmd package/cfg/helloWorld_p28FP.o28FP

# To simplify configuro usage in makefiles:
#     o create a generic linker command file name 
#     o set modification times of compiler.opt* files to be greater than
#       or equal to the generated config header
#
linker.cmd: package/cfg/helloWorld_p28FP.xdl
	$(SED) 's"^\"\(package/cfg/helloWorld_p28FPcfg.cmd\)\"$""\"C:/Users/Chris/workspace_v6_0/Quad RTSC/.config/xconfig_helloWorld/\1\""' package/cfg/helloWorld_p28FP.xdl > $@
	-$(SETDATE) -r:max package/cfg/helloWorld_p28FP.h compiler.opt compiler.opt.defs
