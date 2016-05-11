####################################################
# PSP Makefile generated for GNU Make by ProDG VSI #
####################################################

.SILENT:

unexport MAKEFLAGS
unexport MFLAGS

OUTDIR = PSP_Debug
INTDIR = PSP_Debug

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
	-DPSP\
	-I$(SCE_PSP_HW_ROOT)/include\
	-I.\
	-c\
	-Xmserrors\
	-g\

CC=pspsnc
CC_FLAGS=\
	-DSN_TARGET_PSP_PRX\
	-D_DEBUG\
	-DPSP\
	-I$(SCE_PSP_HW_ROOT)/include\
	-I.\
	-c\
	-Xmserrors\
	-MMD\
	-g\
	-O0\

CXX=pspsnc
CXX_FLAGS=\
	-DSN_TARGET_PSP_PRX\
	-D_DEBUG\
	-DPSP\
	-I$(SCE_PSP_HW_ROOT)/include\
	-I.\
	-c\
	-Xmserrors\
	-MMD\
	-g\
	-O0\

LD=pspsnc
LD_FLAGS=\
	-L$(SCE_PSP_HW_ROOT)/lib\
	-Wl,-r -Wl,-dc\
	-g\

OBJS=\
	"PSP_Debug/CubeData.obj"\
	"PSP_Debug/FrontEnd.obj"\
	"PSP_Debug/InputManager.obj"\
	"PSP_Debug/Main.obj"\

CST_OBJS=\

CST_OUTS=\

CUSTOM: $(CST_OUTS) $(CST_OBJS)

COMPILE: $(OBJS)

"PSP_Debug/CubeData.obj":
	@echo ProDG Compiling "//ctstudent2/users/100068439/My Documents/Semester 4/Console Development/Cross-Development/Cross-Platform/CubeData.cpp"
	$(CXX) $(CXX_FLAGS) "//ctstudent2/users/100068439/My Documents/Semester 4/Console Development/Cross-Development/Cross-Platform/CubeData.cpp" -o $@

"PSP_Debug/FrontEnd.obj":
	@echo ProDG Compiling "//ctstudent2/users/100068439/My Documents/Semester 4/Console Development/Cross-Development/Cross-Platform/FrontEnd.cpp"
	$(CXX) $(CXX_FLAGS) "//ctstudent2/users/100068439/My Documents/Semester 4/Console Development/Cross-Development/Cross-Platform/FrontEnd.cpp" -o $@

"PSP_Debug/InputManager.obj":
	@echo ProDG Compiling "//ctstudent2/users/100068439/My Documents/Semester 4/Console Development/Cross-Development/Cross-Platform/InputManager.cpp"
	$(CXX) $(CXX_FLAGS) "//ctstudent2/users/100068439/My Documents/Semester 4/Console Development/Cross-Development/Cross-Platform/InputManager.cpp" -o $@

"PSP_Debug/Main.obj":
	@echo ProDG Compiling "//ctstudent2/users/100068439/My Documents/Semester 4/Console Development/Cross-Development/Cross-Platform/Main.cpp"
	$(CXX) $(CXX_FLAGS) "//ctstudent2/users/100068439/My Documents/Semester 4/Console Development/Cross-Development/Cross-Platform/Main.cpp" -o $@

LINK: "PSP_Debug/tempprx.obj"

"PSP_Debug/tempprx.obj" : $(CST_OBJS) $(OBJS)
	@echo ProDG Linking $@
	$(LD) $(LD_FLAGS) --start-group $(CST_OBJS) $(OBJS) $(LIBS) --end-group -o $@

