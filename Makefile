NCT_PROJECTS    ?= ../NewtonDev/NCT_Projects

CC              := arm-none-eabi-gcc
CXX             := arm-none-eabi-g++
AS              := arm-none-eabi-as
LINKER          := arm-none-eabi-ld

MACHINEFLAGS    := -march=armv3 -mbig-endian
ASFLAGS         := ${MACHINEFLAGS}
CODEGENFLAGS    := -nostdinc -fPIC -fdata-sections -ffunction-sections -O3 -Os
DEFINES         := -D__arm -DhasPCMCIA -DforARM -DNO_ERRNO_H -DSTDC
CFLAGS          := ${MACHINEFLAGS} ${CODEGENFLAGS} ${DEFINES}
CXXFLAGS        := ${MACHINEFLAGS} ${CODEGENFLAGS} ${DEFINES} -fno-exceptions -fno-rtti
OBJDIR          := obj

NEWTONINCLUDES  := -I$(NCT_PROJECTS)/DDKIncludes -I$(NCT_PROJECTS)/DDKIncludes/Bootstrap -I$(NCT_PROJECTS)/DDKIncludes/CLibrary -I$(NCT_PROJECTS)/DDKIncludes/CommAPI -I$(NCT_PROJECTS)/DDKIncludes/Communications -I$(NCT_PROJECTS)/DDKIncludes/Frames -I$(NCT_PROJECTS)/DDKIncludes/HAL -I$(NCT_PROJECTS)/DDKIncludes/Lantern -I$(NCT_PROJECTS)/DDKIncludes/OS600 -I$(NCT_PROJECTS)/DDKIncludes/Packages -I$(NCT_PROJECTS)/DDKIncludes/Power -I$(NCT_PROJECTS)/DDKIncludes/QD -I$(NCT_PROJECTS)/DDKIncludes/Toolbox -I$(NCT_PROJECTS)/DDKIncludes/UtilityClasses -I$(NCT_PROJECTS)/DDKIncludes/PCMCIA -I$(NCT_PROJECTS)/Includes -I$(NCT_PROJECTS)/Includes/CLibrary -I$(NCT_PROJECTS)/Includes/Frames -I$(NCT_PROJECTS)/Includes/Utilities -I$(NCT_PROJECTS)/LanternDriver_DDK/Includes -I$(NCT_PROJECTS)/NSandDDK

RUNTIMESRCDIR   := NewtonRuntime/
RUNTIMEINCLUDES := -I$(RUNTIMESRCDIR)
RUNTIMECXXSRCS  := $(shell find $(RUNTIMESRCDIR) -type f -name "*.cpp")
RUNTIMECSRCS    := $(shell find $(RUNTIMESRCDIR) -type f -name "*.c")
RUNTIMEASMSRCS  := $(shell find $(RUNTIMESRCDIR) -type f -name "*.s")
RUNTIMECXXOBJS  := $(patsubst $(RUNTIMESRCDIR)/%,$(OBJDIR)/%,$(RUNTIMECXXSRCS:.cpp=.cpp.o))
RUNTIMECOBJS    := $(patsubst $(RUNTIMESRCDIR)/%,$(OBJDIR)/%,$(RUNTIMECSRCS:.c=.c.o))
RUNTIMEASMOBJS  := $(patsubst $(RUNTIMESRCDIR)/%,$(OBJDIR)/%,$(RUNTIMEASMSRCS:.s=.s.o))
RUNTIMEOBJS     := $(RUNTIMECXXOBJS) $(RUNTIMECOBJS) $(RUNTIMEASMOBJS)

all : $(OBJDIR) mbedTLSInterface.ntkc mbedTLSProtocol.pkg ca-certificates.txt

clean :
	rm -rf $(OBJDIR)
	rm -f mbedTLSInterface.ntkc
	rm -f mbedTLSProtocol.pkg
	rm -f ca-certificates.txt

$(OBJDIR) :
	mkdir $(OBJDIR)

$(RUNTIMECXXOBJS) : $(OBJDIR)/%.cpp.o : %.cpp
	$(CXX) $(CXXFLAGS) $(RUNTIMEINCLUDES) $(NEWTONINCLUDES) -c $< -o $@
	$(CXX) -MM $(CXXFLAGS) $(RUNTIMEINCLUDES) $(NEWTONINCLUDES) -c $< -MT $@ > $(@:.o=.d)

$(RUNTIMECOBJS) : $(OBJDIR)/%.c.o : %.c
	$(CC) $(CFLAGS) $(RUNTIMEINCLUDES) $(NEWTONINCLUDES) -c $< -o $@
	$(CC) -MM $(CFLAGS) $(RUNTIMEINCLUDES) $(NEWTONINCLUDES) -c $< -MT $@ > $(@:.o=.d)

$(RUNTIMEASMOBJS) : $(OBJDIR)/%.s.o : %.s
	$(AS) $(ASFLAGS) -c $< -o $@

-include $(RUNTIMECOBJS:.o=.d)
-include $(RUNTIMECXXOBJS:.o=.d)

# Common rules

COMMONSRCDIR    := Common
COMMONINCLUDES  := -I$(COMMONSRCDIR) -I$(COMMONSRCDIR)/mbedtls
COMMONCXXSRCS   := $(shell find $(COMMONSRCDIR) -type f -name "*.cpp")
COMMONASMSRCS   := $(shell find $(COMMONSRCDIR) -type f -name "*.s")
COMMONCXXOBJS   := $(patsubst $(COMMONSRCDIR)/%,$(OBJDIR)/%,$(COMMONCXXSRCS:.cpp=.cpp.o))
COMMONASMOBJS   := $(patsubst $(COMMONSRCDIR)/%,$(OBJDIR)/%,$(COMMONASMSRCS:.s=.s.o))
COMMONOBJS      := $(COMMONCXXOBJS) $(COMMONASMOBJS)

$(COMMONCXXOBJS) : $(OBJDIR)/%.cpp.o : %.cpp
	$(CXX) $(CXXFLAGS) $(COMMONINCLUDES) $(RUNTIMEINCLUDES) $(NEWTONINCLUDES) -c $< -o $@
	$(CXX) -MM $(CFLAGS) $(COMMONINCLUDES) $(RUNTIMEINCLUDES) $(NEWTONINCLUDES) -c $< -MT $@ > $(@:.o=.d)

$(COMMONASMOBJS) : $(OBJDIR)/%.s.o : %.s
	$(AS) $(ASFLAGS) -c $< -o $@

-include $(COMMONCXXOBJS:.o=.d)

# NTKC rules

NTKCSRCDIR      := NTKModule
NTKCCXXSRCS     := $(shell find $(NTKCSRCDIR) -type f -name "*.cpp")
NTKCASMSRCS     := $(shell find $(NTKCSRCDIR) -type f -name "*.s")
NTKCCXXOBJS     := $(patsubst $(NTKCSRCDIR)/%,$(OBJDIR)/%,$(NTKCCXXSRCS:.cpp=.cpp.o))
NTKCASMOBJS     := $(patsubst $(NTKCSRCDIR)/%,$(OBJDIR)/%,$(NTKCASMSRCS:.s=.s.o))
NTKCOBJS        := $(NTKCCXXOBJS) $(NTKCASMOBJS)
NTKCLINKFLAGS   := -EB -shared --gc-sections --no-allow-shlib-undefined --no-export-dynamic -x --no-undefined -T$(NTKCSRCDIR)/mbedTLSInterface.ld --version-script=$(NTKCSRCDIR)/mbedTLSInterface.map

mbedTLSInterface.ntkc : $(OBJDIR)/mbedTLSInterface.elf $(NTKCSRCDIR)/mbedTLSInterface.exp
	rm -f $@
	ELFtoNTK -via $(NTKCSRCDIR)/mbedTLSInterface.exp -o $@ $(OBJDIR)/mbedTLSInterface.elf
	SetFile -c NTP1 -t ntkc $@

$(OBJDIR)/mbedTLSInterface.elf : $(NTKCOBJS) $(RUNTIMEOBJS) $(COMMONOBJS)
	$(LINKER) $(NTKCLINKFLAGS) $^ -o $@

$(NTKCCXXOBJS) : $(OBJDIR)/%.cpp.o : %.cpp
	$(CXX) $(CXXFLAGS) $(COMMONINCLUDES) $(RUNTIMEINCLUDES) $(NEWTONINCLUDES) -c $< -o $@
	$(CXX) -MM $(CXXFLAGS) $(COMMONINCLUDES) $(RUNTIMEINCLUDES) $(NEWTONINCLUDES) -c $< -MT $@ > $(@:.o=.d)

$(NTKCASMOBJS) : $(OBJDIR)/%.s.o : %.s
	$(AS) $(ASFLAGS) -c $< -o $@

-include $(NTKCCXXOBJS:.o=.d)

# Protocol rules

MBEDTLSSRCDIR   := mbedtls/library/
MBEDTLSINCLUDES := -Imbedtls/include
MBEDTLSSRCS     := $(shell find $(MBEDTLSSRCDIR) -type f -name "*.c")
MBEDTLSOBJECTS  := $(patsubst $(MBEDTLSSRCDIR)/%,$(OBJDIR)/%,$(MBEDTLSSRCS:.c=.c.o))

PROTOSRCDIR   := Protocol
PROTOCXXSRCS  := $(shell find $(PROTOSRCDIR) -type f -name "*.cpp")
PROTOASMSRCS  := $(shell find $(PROTOSRCDIR) -type f -name "*.s")
PROTOCXXOBJS  := $(patsubst $(PROTOSRCDIR)/%,$(OBJDIR)/%,$(PROTOCXXSRCS:.cpp=.cpp.o))
PROTOASMOBJS  := $(patsubst $(PROTOSRCDIR)/%,$(OBJDIR)/%,$(PROTOASMSRCS:.s=.s.o))
PROTOLINKFLAGS  := -EB -shared --gc-sections --no-allow-shlib-undefined --no-export-dynamic -x --no-undefined -T$(PROTOSRCDIR)/mbedTLSProtocol.ld --version-script=$(PROTOSRCDIR)/mbedTLSProtocol.map
PROTOPKGNAME    := "mbedTLSProtocol:Kallisys"

mbedTLSProtocol.pkg : $(OBJDIR)/mbedTLSProtocol.elf
	rm -f $@
	ELFtoPKG -o $@ $(PROTOPKGNAME) -protocol $< -autoLoad -autoRemove
	SetFile -t 'pkg ' -c 'pkgX' $@

$(OBJDIR)/mbedTLSProtocol.elf : $(PROTOCXXOBJS) $(PROTOASMOBJS) $(RUNTIMEOBJS) $(COMMONOBJS) $(MBEDTLSOBJECTS)
	$(LINKER) $(PROTOLINKFLAGS) $^ -o $@

$(PROTOCXXOBJS) : $(OBJDIR)/%.cpp.o : %.cpp
	$(CXX) $(CXXFLAGS) $(COMMONINCLUDES) $(MBEDTLSINCLUDES) $(RUNTIMEINCLUDES) $(NEWTONINCLUDES) -c $< -o $@
	$(CXX) -MM $(CXXFLAGS) $(COMMONINCLUDES) $(MBEDTLSINCLUDES) $(RUNTIMEINCLUDES) $(NEWTONINCLUDES) -c $< -MT $@ > $(@:.o=.d)

$(MBEDTLSOBJECTS) : $(OBJDIR)/%.c.o : %.c
	$(CC) $(CFLAGS) $(COMMONINCLUDES) $(MBEDTLSINCLUDES) $(RUNTIMEINCLUDES) $(NEWTONINCLUDES) -c $< -o $@
	$(CC) -MM $(CFLAGS) $(COMMONINCLUDES) $(MBEDTLSINCLUDES) $(RUNTIMEINCLUDES) $(NEWTONINCLUDES) -c $< -MT $@ > $(@:.o=.d)

$(PROTOASMOBJS) : $(OBJDIR)/%.s.o : %.s
	$(AS) $(ASFLAGS) -c $< -o $@

-include $(PROTOCXXOBJS:.o=.d)

# Incomplete support for ProtocolGenTool generation
# Currently missing rules:
# from $(OBJDIR)/PMbedTLSGlue.a to PMbedTLS.impl.s
# from $(OBJDIR)/PMbedTLSInterfaceGlue.a to PMbedTLSInterfaceGlue.s

$(OBJDIR)/PMbedTLS.impl.i : PMbedTLS.impl.h
	$(CXX) $(CXXFLAGS) $(COMMONINCLUDES) $(MBEDTLSINCLUDES) $(RUNTIMEINCLUDES) $(NEWTONINCLUDES) -c $< -E -DPROTOCOLGEN -o $@
	tr '\n' '\r' < $(OBJDIR)/PMbedTLS.impl.i > $(OBJDIR)/PMbedTLS.impl.i.mac
	mv $(OBJDIR)/PMbedTLS.impl.i.mac $(OBJDIR)/PMbedTLS.impl.i

$(OBJDIR)/PMbedTLSGlue.a : $(OBJDIR)/PMbedTLS.impl.i
	ProtocolGenTool -ServerGlue $<
	tr '\r' '\n' < PMbedTLSGlue.a > $@

$(OBJDIR)/PMbedTLSInterface.i : Common/PMbedTLSInterface.h
	$(CXX) $(CXXFLAGS) $(COMMONINCLUDES) $(MBEDTLSINCLUDES) $(RUNTIMEINCLUDES) $(NEWTONINCLUDES) -c $< -E -DPROTOCOLGEN -o $@
	tr '\n' '\r' < $(OBJDIR)/PMbedTLSInterface.i > $(OBJDIR)/PMbedTLSInterface.i.mac
	mv $(OBJDIR)/PMbedTLSInterface.i.mac $(OBJDIR)/PMbedTLSInterface.i

$(OBJDIR)/PMbedTLSInterfaceGlue.a : $(OBJDIR)/PMbedTLSInterface.i
	ProtocolGenTool -ClientGlue $<
	tr '\r' '\n' < $@ > $@.mac
	mv $@.mac $@

VPATH           := $(COMMONSRCDIR):$(RUNTIMESRCDIR):$(NTKCSRCDIR):$(PROTOSRCDIR):$(MBEDTLSSRCDIR)

ca-certificates.txt : ca-certificates/mozilla/
	./pack-ca-certificates.sh
