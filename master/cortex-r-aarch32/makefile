export SHELLOPTS := igncr

FLASHSTART ?= 0x00000000
RAMSTART   ?= 0x20000000
RAMSIZE    ?=
WATCHDOG   ?= 0
DEFINE     ?= 
SRC        ?= 
LIB        ?= 

CFLAGS     ?=
CPPFLAGS   ?=
LDFLAGS    ?=
LOPT       ?=

INCLUDEDIR ?=
HARDFP     ?= 0

GCCARMEMBEDDED   := $(shell arm-none-eabi-gcc --version | grep "GNU Arm Embedded" | sed 's/.*[0-9]*\.[0-9]*-\([0-9]*\)\.\([0-9]*\).*/\1\2/g')
MKFILEPATH       := $(dir $(abspath $(lastword $(MAKEFILE_LIST))))
WORKPATH         := $(dir $(abspath $(CURDIR))/)
TMP              := $(WORKPATH)tmp
MISCSRC          :=
COMMA            := ,

IFLAGS           := -I$(CURDIR) -I$(MKFILEPATH)/src $(addprefix -I,$(INCLUDEDIR))
HEADERS          := $(wildcard $(CURDIR)/*.h $(MKFILEPATH)/src/*.h $(addsuffix /*.h,$(INCLUDEDIR)))
DFLAGS           := $(addprefix -D,$(DEFINE))
DFLAGS           += $(if $(RAMSIZE),-DRAMSTART=$(RAMSTART) -DRAMSIZE=$(RAMSIZE))
DFLAGS           += $(if $(BOOTMPIDR),-DBOOTMPIDR=$(BOOTMPIDR))
ifneq ($(WATCHDOG),0)
DFLAGS           += -DWATCHDOG
MISCSRC          += watchdog.c
endif

LIBS       := -Wl,-lgcc $(addprefix -Wl$(COMMA),$(LIB))

ifeq ($(shell expr $(GCCARMEMBEDDED) \>= 202110), 1)
ARCH             := -march=armv8-r
LOPT             += -march=armv8-r
else
ARCH             := -march=armv7-r
endif

ifeq ($(HARDFP),0)
CPU_AARCH32_ARM_V8R_LE      := $(ARCH) -marm -mlittle-endian -msoft-float
else
CPU_AARCH32_ARM_V8R_LE      := $(ARCH) -marm -mlittle-endian -mfloat-abi=hard -mfpu=vfpv3-d16-fp16
endif
AOPT_AARCH32_ARM_V8R_LE     := -g -Wall $(CPU_AARCH32_ARM_V8R_LE) -Wa,--gdwarf2 -xassembler-with-cpp -c $(DFLAGS)
COPT_AARCH32_ARM_V8R_LE     := -g -Wall $(CPU_AARCH32_ARM_V8R_LE) -fshort-enums -c $(IFLAGS)  $(DFLAGS) $(CFLAGS)
CPPOPT_AARCH32_ARM_V8R_LE   := -g -Wall $(CPU_AARCH32_ARM_V8R_LE) -fshort-enums -fno-unwind-tables -fno-exceptions -fno-rtti  -c $(IFLAGS)  $(DFLAGS) $(CFLAGS)
LOPT_AARCH32_ARM_V8R_LE     := -g -Wall $(CPU_AARCH32_ARM_V8R_LE) -Wl,--nmagic -nostartfiles -nodefaultlibs -Wl,--entry=_start $(LOPT) $(addprefix -Wl$(COMMA),$(LDFLAGS))

ifeq ($(HARDFP),0)
CPU_AARCH32_ARM_V8R_BE      := $(ARCH) -marm -mbig-endian    -msoft-float
else
CPU_AARCH32_ARM_V8R_BE      := $(ARCH) -marm -mbig-endian    -mfloat-abi=hard -mfpu=vfpv3-d16-fp16
endif
AOPT_AARCH32_ARM_V8R_BE     := -g -Wall $(CPU_AARCH32_ARM_V8R_BE) -Wa,--gdwarf2 -xassembler-with-cpp -c $(DFLAGS)
COPT_AARCH32_ARM_V8R_BE     := -g -Wall $(CPU_AARCH32_ARM_V8R_BE) -fshort-enums -c $(IFLAGS)  $(DFLAGS) $(CFLAGS)
CPPOPT_AARCH32_ARM_V8R_BE   := -g -Wall $(CPU_AARCH32_ARM_V8R_BE) -fshort-enums -fno-unwind-tables -fno-exceptions -fno-rtti -c $(IFLAGS)  $(DFLAGS) $(CPPFLAGS)
LOPT_AARCH32_ARM_V8R_BE     := -g -Wall $(CPU_AARCH32_ARM_V8R_BE) -Wl,--nmagic -nostartfiles -nodefaultlibs -Wl,--entry=_start $(addprefix -Wl$(COMMA),$(LDFLAGS))

ifeq ($(HARDFP),0)
CPU_AARCH32_THUMB_V8R_LE    := $(ARCH) -mthumb -mlittle-endian -msoft-float
else
CPU_AARCH32_THUMB_V8R_LE    := $(ARCH) -mthumb -mlittle-endian -mfloat-abi=hard -mfpu=vfpv3-d16-fp16
endif
AOPT_AARCH32_THUMB_V8R_LE   := -g -Wall $(CPU_AARCH32_THUMB_V8R_LE) -Wa,--gdwarf2 -Wa,-mthumb -xassembler-with-cpp -c $(DFLAGS)
COPT_AARCH32_THUMB_V8R_LE   := -g -Wall $(CPU_AARCH32_THUMB_V8R_LE) -fshort-enums -c $(IFLAGS) $(DFLAGS) $(CFLAGS)
CPPOPT_AARCH32_THUMB_V8R_LE := -g -Wall $(CPU_AARCH32_THUMB_V8R_LE) -fshort-enums -fno-unwind-tables -fno-exceptions -fno-rtti -c $(IFLAGS) $(DFLAGS) $(CPPFLAGS)
LOPT_AARCH32_THUMB_V8R_LE   := -g -Wall $(CPU_AARCH32_THUMB_V8R_LE) -Wl,--nmagic -nostartfiles -nodefaultlibs -Wl,--thumb-entry=_start $(addprefix -Wl$(COMMA),$(LDFLAGS))

ifeq ($(HARDFP),0)
CPU_AARCH32_THUMB_V8R_BE    := $(ARCH) -mthumb -mbig-endian -msoft-float
else
CPU_AARCH32_THUMB_V8R_BE    := $(ARCH) -mthumb -mbig-endian -mfloat-abi=hard -mfpu=vfpv3-d16-fp16
endif
AOPT_AARCH32_THUMB_V8R_BE   := -g -Wall $(CPU_AARCH32_THUMB_V8R_BE) -Wa,--gdwarf2 -Wa,-mthumb -xassembler-with-cpp -c $(DFLAGS)
COPT_AARCH32_THUMB_V8R_BE   := -g -Wall $(CPU_AARCH32_THUMB_V8R_BE) -fshort-enums -c $(IFLAGS) $(DFLAGS) $(CFLAGS)
CPPOPT_AARCH32_THUMB_V8R_BE := -g -Wall $(CPU_AARCH32_THUMB_V8R_BE) -fshort-enums -fno-unwind-tables -fno-exceptions -fno-rtti -c $(IFLAGS) $(DFLAGS) $(CPPFLAGS)
LOPT_AARCH32_THUMB_V8R_BE   := -g -Wall $(CPU_AARCH32_THUMB_V8R_BE) -Wl,--nmagic -nostartfiles -nodefaultlibs -Wl,--thumb-entry=_start $(addprefix -Wl$(COMMA),$(LDFLAGS))

CCGRP              := arm-none-eabi-

TARGET             ?= sieve.c 
SRCS               := crt0.s $(SRC) $(MISCSRC)
TARGETS             = $(TARGET:%.c=%_ram_aarch32_arm_v8r_le.elf)     $(TARGET:%.c=%_ram_aarch32_arm_v8r_be.elf) \
                      $(TARGET:%.c=%_flash_aarch32_arm_v8r_le.elf)   $(TARGET:%.c=%_flash_aarch32_arm_v8r_be.elf) \
                      $(TARGET:%.c=%_ram_aarch32_thumb_v8r_le.elf)   $(TARGET:%.c=%_ram_aarch32_thumb_v8r_be.elf) \
                      $(TARGET:%.c=%_flash_aarch32_thumb_v8r_le.elf) $(TARGET:%.c=%_flash_aarch32_thumb_v8r_be.elf)

-include ./special-cortex-r-defines.mk

all: $(TARGETS)
	

target-r% target-le-r% : $(filter %_aarch32_arm_v8r_le.elf, $(TARGETS))
	

target-be-r% : $(filter %_aarch32_arm_v8r_be.elf, $(TARGETS))
	

.PHONY: tmpdir
tmpdir:
	@-mkdir -p $(TMP)
	@-mkdir -p $(addprefix $(TMP)/,$(foreach src,$(SRCS),$(dir $(src))))

./%.c : $(MKFILEPATH)src/%.c
	cp $< ./$@ 

./%.s : $(MKFILEPATH)src/%.s
	cp $< ./$@ 

$(TMP)/headers: $(HEADERS) | tmpdir
	touch $(TMP)/headers

$(TMP)/%.armv7rle.o:        ./%.c   $(MAKEFILE_LIST) | tmpdir
	$(CCGRP)gcc $(COPT_AARCH32_ARM_V8R_LE)     -o $@ $<

$(TMP)/%.armv7rbe.o:        ./%.c   $(MAKEFILE_LIST) | tmpdir
	$(CCGRP)gcc $(COPT_AARCH32_ARM_V8R_BE)     -o $@ $<

$(TMP)/%.thumbIIv7rle.o:   ./%.c    $(MAKEFILE_LIST) | tmpdir
	$(CCGRP)gcc $(COPT_AARCH32_THUMB_V8R_LE)   -o $@ $<

$(TMP)/%.thumbIIv7rbe.o:   ./%.c    $(MAKEFILE_LIST) | tmpdir
	$(CCGRP)gcc $(COPT_AARCH32_THUMB_V8R_BE)   -o $@ $<

$(TMP)/%.armv7rle.o:        ./%.cpp $(MAKEFILE_LIST) | tmpdir
	$(CCGRP)g++ $(CPPOPT_AARCH32_ARM_V8R_LE)   -o $@ $<

$(TMP)/%.armv7rbe.o:        ./%.cpp $(MAKEFILE_LIST) | tmpdir
	$(CCGRP)g++ $(CPPOPT_AARCH32_ARM_V8R_BE)   -o $@ $<

$(TMP)/%.thumbIIv7rle.o:    ./%.cpp  $(MAKEFILE_LIST) | tmpdir
	$(CCGRP)g++ $(CPPOPT_AARCH32_THUMB_V8R_LE) -o $@ $<

$(TMP)/%.thumbIIv7rbe.o:    ./%.cpp  $(MAKEFILE_LIST) | tmpdir
	$(CCGRP)g++ $(CPPOPT_AARCH32_THUMB_V8R_BE) -o $@ $<

$(TMP)/%.armv7rle.o:        ./%.s   $(MAKEFILE_LIST) | tmpdir
	$(CCGRP)gcc $(AOPT_AARCH32_ARM_V8R_LE)     -o $@ $<

$(TMP)/%.armv7rbe.o:        ./%.s   $(MAKEFILE_LIST) | tmpdir
	$(CCGRP)gcc $(AOPT_AARCH32_ARM_V8R_BE)     -o $@ $<

$(TMP)/%.thumbIIv7rle.o:   ./%.s    $(MAKEFILE_LIST) | tmpdir
	$(CCGRP)gcc $(AOPT_AARCH32_THUMB_V8R_LE)   -o $@ $<

$(TMP)/%.thumbIIv7rbe.o:   ./%.s    $(MAKEFILE_LIST) | tmpdir
	$(CCGRP)gcc $(AOPT_AARCH32_THUMB_V8R_BE)   -o $@ $<

.INTERMEDIATE :          $(TMP)/ram.ld
$(TMP)/ram.ld:           $(MKFILEPATH)src/linker.ldx $(MAKEFILE_LIST) | tmpdir
	$(CCGRP)cpp -P -DRAMSTART=$(RAMSTART) $(DFLAGS) -o $@ $< 

.INTERMEDIATE :          $(TMP)/flash.ld
$(TMP)/flash.ld:         $(MKFILEPATH)src/linker.ldx $(MAKEFILE_LIST) | tmpdir
	$(CCGRP)cpp -P -DFLASHSTART=$(FLASHSTART) -DRAMSTART=$(RAMSTART) $(DFLAGS) -o $@ $<

%_ram_aarch32_arm_v8r_le.elf:        ./%.c $(addprefix ./,$(SRCS)) $(addprefix $(TMP)/,ram.ld   $(filter %.o, $(SRCS:%.s=%.armv7rle.o))     %.armv7rle.o     $(filter %.o, $(SRCS:%.c=%.armv7rle.o))     $(filter %.o, $(SRCS:%.cpp=%.armv7rle.o))    ) $(TMP)/headers
	$(CCGRP)gcc $(LOPT_AARCH32_ARM_V8R_LE)   -o $@ -T $(filter %.ld,$(^)) $(filter %.o,$(^)) -Wl,--start-group $(LIBS) -Wl,--end-group

%_ram_aarch32_arm_v8r_be.elf:        ./%.c $(addprefix ./,$(SRCS)) $(addprefix $(TMP)/,ram.ld   $(filter %.o, $(SRCS:%.s=%.armv7rbe.o))     %.armv7rbe.o     $(filter %.o, $(SRCS:%.c=%.armv7rbe.o))     $(filter %.o, $(SRCS:%.cpp=%.armv7rbe.o))    ) $(TMP)/headers
	$(CCGRP)gcc $(LOPT_AARCH32_ARM_V8R_BE)   -o $@ -T $(filter %.ld,$(^)) $(filter %.o,$(^)) -Wl,--start-group $(LIBS) -Wl,--end-group

%_flash_aarch32_arm_v8r_le.elf:      ./%.c $(addprefix ./,$(SRCS)) $(addprefix $(TMP)/,flash.ld $(filter %.o, $(SRCS:%.s=%.armv7rle.o))     %.armv7rle.o     $(filter %.o, $(SRCS:%.c=%.armv7rle.o))     $(filter %.o, $(SRCS:%.cpp=%.armv7rle.o))    ) $(TMP)/headers
	$(CCGRP)gcc $(LOPT_AARCH32_ARM_V8R_LE)   -o $@ -T $(filter %.ld,$(^)) $(filter %.o,$(^)) -Wl,--start-group $(LIBS) -Wl,--end-group

%_flash_aarch32_arm_v8r_be.elf:      ./%.c $(addprefix ./,$(SRCS)) $(addprefix $(TMP)/,flash.ld $(filter %.o, $(SRCS:%.s=%.armv7rbe.o))     %.armv7rbe.o     $(filter %.o, $(SRCS:%.c=%.armv7rbe.o))     $(filter %.o, $(SRCS:%.cpp=%.armv7rbe.o))    ) $(TMP)/headers
	$(CCGRP)gcc $(LOPT_AARCH32_ARM_V8R_BE)   -o $@ -T $(filter %.ld,$(^)) $(filter %.o,$(^)) -Wl,--start-group $(LIBS) -Wl,--end-group

%_ram_aarch32_thumb_v8r_le.elf:   ./%.c $(addprefix ./,$(SRCS)) $(addprefix $(TMP)/,ram.ld   $(filter %.o, $(SRCS:%.s=%.thumbIIv7rle.o)) %.thumbIIv7rle.o $(filter %.o, $(SRCS:%.c=%.thumbIIv7rle.o)) $(filter %.o, $(SRCS:%.cpp=%.thumbIIv7rle.o))) $(TMP)/headers
	$(CCGRP)gcc $(LOPT_AARCH32_THUMB_V8R_LE) -o $@ -T $(filter %.ld,$(^)) $(filter %.o,$(^)) -Wl,--start-group $(LIBS) -Wl,--end-group

%_ram_aarch32_thumb_v8r_be.elf:   ./%.c $(addprefix ./,$(SRCS)) $(addprefix $(TMP)/,ram.ld   $(filter %.o, $(SRCS:%.s=%.thumbIIv7rbe.o)) %.thumbIIv7rbe.o $(filter %.o, $(SRCS:%.c=%.thumbIIv7rbe.o)) $(filter %.o, $(SRCS:%.cpp=%.thumbIIv7rbe.o))) $(TMP)/headers
	$(CCGRP)gcc $(LOPT_AARCH32_THUMB_V8R_BE) -o $@ -T $(filter %.ld,$(^)) $(filter %.o,$(^)) -Wl,--start-group $(LIBS) -Wl,--end-group

%_flash_aarch32_thumb_v8r_le.elf: ./%.c $(addprefix ./,$(SRCS)) $(addprefix $(TMP)/,flash.ld $(filter %.o, $(SRCS:%.s=%.thumbIIv7rle.o)) %.thumbIIv7rle.o $(filter %.o, $(SRCS:%.c=%.thumbIIv7rle.o)) $(filter %.o, $(SRCS:%.cpp=%.thumbIIv7rle.o))) $(TMP)/headers
	$(CCGRP)gcc $(LOPT_AARCH32_THUMB_V8R_LE) -o $@ -T $(filter %.ld,$(^)) $(filter %.o,$(^)) -Wl,--start-group $(LIBS) -Wl,--end-group

%_flash_aarch32_thumb_v8r_be.elf: ./%.c $(addprefix ./,$(SRCS)) $(addprefix $(TMP)/,flash.ld $(filter %.o, $(SRCS:%.s=%.thumbIIv7rbe.o)) %.thumbIIv7rbe.o $(filter %.o, $(SRCS:%.c=%.thumbIIv7rbe.o)) $(filter %.o, $(SRCS:%.cpp=%.thumbIIv7rbe.o))) $(TMP)/headers
	$(CCGRP)gcc $(LOPT_AARCH32_THUMB_V8R_BE) -o $@ -T $(filter %.ld,$(^)) $(filter %.o,$(^)) -Wl,--start-group $(LIBS) -Wl,--end-group

clean:
	-rm -fvr $(TMP)
	-rm -fv ./*_ram_aarch32_arm_v8r_le.elf
	-rm -fv ./*_ram_aarch32_arm_v8r_be.elf
	-rm -fv ./*_flash_aarch32_arm_v8r_le.elf
	-rm -fv ./*_flash_aarch32_arm_v8r_be.elf
	-rm -fv ./*_ram_aarch32_thumb_v8r_le.elf
	-rm -fv ./*_ram_aarch32_thumb_v8r_be.elf
	-rm -fv ./*_flash_aarch32_thumb_v8r_le.elf
	-rm -fv ./*_flash_aarch32_thumb_v8r_be.elf

-include ./special-cortex-r-rules.mk
