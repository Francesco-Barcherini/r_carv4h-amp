export SHELLOPTS := igncr

FLASHSTART ?= 0x00000000
RAMSTART   ?= 0x20000000
WATCHDOG   ?= 0
DEFINE     ?= 
SRC        ?= serial.c cbfw_printf.c scif_icumx.c
LIB        ?= 
HARDFP     ?= 0

CFLAGS     ?=
LDFLAGS    ?=
INCLUDEDIR ?=

MKFILEPATH      := $(dir $(abspath $(lastword $(MAKEFILE_LIST))))
WORKPATH        := $(dir $(abspath $(CURDIR))/)
TMP             := $(WORKPATH)tmp
MISCSRC         :=
COMMA           := ,

IFLAGS          := -I$(CURDIR) -I$(MKFILEPATH)/src $(addprefix -I,$(INCLUDEDIR))
HEADERS         := $(wildcard $(CURDIR)/*.h $(MKFILEPATH)/src/*.h $(addsuffix /*.h,$(INCLUDEDIR)))
DFLAGS          := $(addprefix -D,$(DEFINE))
DFLAGS          += $(if $(RAMSIZE),-DRAMSTART=$(RAMSTART) -DRAMSIZE=$(RAMSIZE))
DFLAGS          += $(if $(BOOTMPIDR),-DBOOTMPIDR=$(BOOTMPIDR))
ifneq ($(WATCHDOG),0)
DFLAGS          += -DWATCHDOG
MISCSRC         += watchdog.c
endif

LIBS            := -Wl,-lgcc $(addprefix -Wl$(COMMA),$(LIB))

AOPT            := -g -Wall -Wa,--gdwarf2 -xassembler-with-cpp -c $(DFLAGS)
COPT            := -g -Wall -fshort-enums -c $(IFLAGS) $(DFLAGS) $(CFLAGS)
CPPOPT          := -g -Wall -fshort-enums -fno-unwind-tables -fno-exceptions -fno-rtti -c $(IFLAGS) $(DFLAGS) $(CPPFLAGS)
LOPT            := -g -Wall -Wl,--nmagic -nostartfiles -nodefaultlibs $(CFLAGS) $(addprefix -Wl$(COMMA),$(LDFLAGS))

CPU_AARCH64     := -mlittle-endian -mstrict-align 
AOPT_AARCH64    := $(CPU_AARCH64)  $(AOPT)
COPT_AARCH64    := $(CPU_AARCH64)  $(COPT)
CPPOPT_AARCH64  := $(CPU_AARCH64)  $(CPPOPT)
LOPT_AARCH64    := $(CPU_AARCH64)  $(LOPT)  -Wl,--entry=_start -DRCAR_V4H=100 -DRCAR_LSI=RCAR_V4H

CC-AARCH64      := aarch64-none-elf-

TARGET          ?= sieve.c
SRCS            := crt0.s $(SRC) $(MISCSRC)
TARGETS          = $(TARGET:%.c=%_ram_aarch64_v8.elf) \
                   $(TARGET:%.c=%_flash_aarch64_v8.elf)

-include ./special-cortex-a-aarch64-defines.mk
all: $(TARGETS)

target-a% : $(filter %_aarch64_v8.elf, $(TARGETS))

$(TMP):
	mkdir $(TMP)

./%.c   : $(MKFILEPATH)src/%.c
	cp $< ./$@ 

./%.cpp : $(MKFILEPATH)src/%.cpp
	cp $< ./$@ 

./%.s   : $(MKFILEPATH)src/%.s
	cp $< ./$@ 

$(TMP)/headers: $(HEADERS)
	touch $(TMP)/headers

$(TMP)/%.aarch64.o:        ./%.c   $(MAKEFILE_LIST)
	$(CC-AARCH64)gcc $(COPT_AARCH64)   -o $@ $<

$(TMP)/%.aarch64.o:        ./%.cpp $(MAKEFILE_LIST)
	$(CC-AARCH64)gcc $(CPPOPT_AARCH64) -o $@ $<

$(TMP)/%.aarch64.o:        ./%.s   $(MAKEFILE_LIST)
	$(CC-AARCH64)gcc $(AOPT_AARCH64)   -o $@ $<

.INTERMEDIATE :          $(TMP)/ram.ld 
$(TMP)/ram.ld:           $(MKFILEPATH)src/linker.ldx $(MAKEFILE_LIST) | $(TMP)
	$(CC-AARCH64)cpp -P -DRAMSTART=$(RAMSTART) $(DFLAGS) -o $@ $< 

.INTERMEDIATE :          $(TMP)/flash.ld
$(TMP)/flash.ld:         $(MKFILEPATH)src/linker.ldx $(MAKEFILE_LIST) | $(TMP)
	$(CC-AARCH64)cpp -P -DFLASHSTART=$(FLASHSTART) -DRAMSTART=$(RAMSTART) $(DFLAGS) -o $@ $<

%_ram_aarch64_v8.elf:    ./%.c $(addprefix ./,$(SRCS)) $(addprefix $(TMP)/,ram.ld   $(filter %.o, $(SRCS:%.s=%.aarch64.o))    %.aarch64.o    $(filter %.o, $(SRCS:%.c=%.aarch64.o) $(SRCS:%.cpp=%.aarch64.o))   ) $(TMP)/headers
	$(CC-AARCH64)gcc $(LOPT_AARCH64)  -o $@ -T $(filter %.ld,$(^)) $(filter %.o,$(^)) -Wl,--start-group $(LIBS) -Wl,--end-group

%_flash_aarch64_v8.elf:  ./%.c $(addprefix ./,$(SRCS)) $(addprefix $(TMP)/,flash.ld $(filter %.o, $(SRCS:%.s=%.aarch64.o))    %.aarch64.o    $(filter %.o, $(SRCS:%.c=%.aarch64.o) $(SRCS:%.cpp=%.aarch64.o))   ) $(TMP)/headers
	$(CC-AARCH64)gcc $(LOPT_AARCH64)  -o $@ -T $(filter %.ld,$(^)) $(filter %.o,$(^)) -Wl,--start-group $(LIBS) -Wl,--end-group

clean:
	-rm -fvr $(TMP)
	-rm -fv ./*_ram_aarch64*.elf
	-rm -fv ./*_ram_aarch64*.elf

-include ./special-cortex-a-aarch64-rules.mk
