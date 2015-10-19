# invoke SourceDir generated makefile for Quad.p28FP
Quad.p28FP: .libraries,Quad.p28FP
.libraries,Quad.p28FP: package/cfg/Quad_p28FP.xdl
	$(MAKE) -f C:\Users\Chris\workspace_v6_0\Quad_RTSC/src/makefile.libs

clean::
	$(MAKE) -f C:\Users\Chris\workspace_v6_0\Quad_RTSC/src/makefile.libs clean

