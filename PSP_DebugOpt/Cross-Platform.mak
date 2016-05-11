####################################################
# PSP Makefile generated for GNU Make by ProDG VSI #
####################################################

.SILENT:

unexport MAKEFLAGS
unexport MFLAGS

OUTDIR = PSP_DebugOpt
INTDIR = PSP_DebugOpt

LIBS=\
	crt0_prx.o\
	libgu.a\
	libgum.a\
	libkernel.a\
	ctrl_stub.a\
	display_stub.a\
	ge_user_stub.a\
	kernel_bridge.o\

WLIBS=\

AS=pspsnc
AS_FLAGS=\
	-DSN_TARGET_PSP_PRX\
	-D_DEBUG\
	-I$(SCE_PSP_HW_ROOT)/include\
	-I.\
	-c\
	-Xmserrors\
	-g\

CC=pspsnc
CC_FLAGS=\
	-DSN_TARGET_PSP_PRX\
	-D_DEBUG\
	-I$(SCE_PSP_HW_ROOT)/include\
	-I.\
	-c\
	-Xmserrors\
	-MMD\
	-g\
	-O3\

CXX=pspsnc
CXX_FLAGS=\
	-DSN_TARGET_PSP_PRX\
	-D_DEBUG\
	-I$(SCE_PSP_HW_ROOT)/include\
	-I.\
	-c\
	-Xmserrors\
	-MMD\
	-g\
	-O3\

LD=pspsnc
LD_FLAGS=\
	-L$(SCE_PSP_HW_ROOT)/lib\
	-Wl,-r -Wl,-dc\
	-g\

OBJS=\
	"PSP_DebugOpt/CubeData.obj"\
	"PSP_DebugOpt/FrontEnd.obj"\
	"PSP_DebugOpt/Main.obj"\

CST_OBJS=\

CST_OUTS=\

CUSTOM: $(CST_OUTS) $(CST_OBJS)

COMPILE: $(OBJS)

"PSP_DebugOpt/CubeData.obj":
	@echo ProDG Compiling "//ctstudent2/users/100068439/My Documents/Semester 4/Console Development/Cross-Development/Cross-Platform/CubeData.cpp"
	$(CXX) $(CXX_FLAGS) "//ctstudent2/users/100068439/My Documents/Semester 4/Console Development/Cross-Development/Cross-Platform/CubeData.cpp" -o $@

"PSP_DebugOpt/FrontEnd.obj":
	@echo ProDG Compiling "//ctstudent2/users/100068439/My Documents/Semester 4/Console Development/Cross-Development/Cross-Platform/FrontEnd.cpp"
	$(CXX) $(CXX_FLAGS) "//ctstudent2/users/100068439/My Documents/Semester 4/Console Development/Cross-Development/Cross-Platform/FrontEnd.cpp" -o $@

"PSP_DebugOpt/Main.obj":
	@echo ProDG Compiling "//ctstudent2/users/100068439/My Documents/Semester 4/Console Development/Cross-Development/Cross-Platform/Main.cpp"
	$(CXX) $(CXX_FLAGS) "//ctstudent2/users/100068439/My Documents/Semester 4/Console Development/Cross-Development/Cross-Platform/Main.cpp" -o $@

LINK: "PSP_DebugOpt/tempprx.obj"

"PSP_DebugOpt/tempprx.obj" : $(CST_OBJS) $(OBJS)
	@echo ProDG Linking $@
	$(LD) $(LD_FLAGS) --start-group $(CST_OBJS) $(OBJS) $(LIBS) --end-group -o $@

