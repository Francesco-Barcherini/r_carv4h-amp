## code generation
# FLASHSTART=0x0
RAMSTART=0xe6301000
TARGET=sieve.c stm_stimulus.c
# WATCHDOG=1
# BOOTMPIDR=0x0000
RAMSIZE=0x20000000
DEFINE=ETRSIZE=0x1000
SRC = serial.c cbfw_printf.c scif_icumx.c gen_timer.c memory.c

include ./cortex-a-aarch64/Makefile

target: target-a76
	

# include special-rule.mk.internal if existing - for Lauterbach internal usage
-include special-rule.mk.internal

