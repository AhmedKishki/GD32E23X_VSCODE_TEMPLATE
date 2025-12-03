################################################################################
# Makefile for GD32E23x project with full C/C++/ASM support and debug/release
################################################################################

include common.mk

# Do not print "Entering directory ..." for recursive make
MAKEFLAGS += --no-print-directory

# === RTOS toggle ===
USE_FREERTOS ?= 1
USE_DYNAMIC  ?= 0

# === Name ===
APPLICATION := app_$(PRODUCT_ID)_$(FW_VERSION_MAJOR).$(FW_VERSION_MINOR).$(FW_VERSION_PATCH)

# === Toolchain ===
CC      := arm-none-eabi-gcc
CXX     := arm-none-eabi-g++
AS      := arm-none-eabi-as
LD      := arm-none-eabi-gcc
OBJCOPY := arm-none-eabi-objcopy
OBJDUMP := arm-none-eabi-objdump
SIZE    := arm-none-eabi-size

DEFINES := -D$(DEVICE) -DUSE_FULL_ASSERT -DUSE_STDPERIPH_DRIVER \
           -D$(PRODUCT_NAME) \
           -DPRODUCT_ID=$(PRODUCT_ID) \
           -DFW_VERSION_MAJOR=$(FW_VERSION_MAJOR) \
           -DFW_VERSION_MINOR=$(FW_VERSION_MINOR) \
           -DFW_VERSION_PATCH=$(FW_VERSION_PATCH)

COMMON_FLAGS := -mcpu=cortex-m23 -mthumb -Wall -Wno-missing-braces -Wno-format \
                -Wno-strict-aliasing -fstack-usage -fdata-sections -ffunction-sections \
                -fmessage-length=0 -fsigned-char -w

DEBUG_OPT_FLAGS   := -g3 -O0 -DDEBUG
RELEASE_OPT_FLAGS := -Os

C_STD   := -std=gnu11
CXX_STD := -std=c++20

ASFLAGS := -x assembler-with-cpp -mcpu=cortex-m23 -mthumb

LDFLAGS := -Tldscripts/linker.ld -static --specs=nano.specs --specs=nosys.specs \
           -Wl,--gc-sections -Wl,--print-memory-usage \
           -Wl,--start-group -lc -lm -Wl,--end-group

# === Directories ===
SRC_DIR      := source
CMSIS_SRC    := firmware/cmsis/source
PERIPH_SRC   := firmware/gd32e23x_standard_peripheral/source
STARTUP_SRC  := firmware/cmsis/gcc_startup

CMSIS_INC    := firmware/cmsis/include
PERIPH_INC   := firmware/gd32e23x_standard_peripheral/include
INCLUDE_DIR  := include

LINKER_SCRIPT := ldscripts/linker.ld

DEBUG_DIR   := build/debug
RELEASE_DIR := build/release

# === Source files ===
C_SOURCES   := $(wildcard $(SRC_DIR)/*.c) \
               $(wildcard $(CMSIS_SRC)/*.c) \
               $(wildcard $(PERIPH_SRC)/*.c)
CPP_SOURCES := $(wildcard $(SRC_DIR)/*.cpp)
ASM_SOURCES := $(wildcard $(STARTUP_SRC)/*.S)

# === Include paths ===
INCLUDES := -I$(CMSIS_INC) -I$(PERIPH_INC) -I$(INCLUDE_DIR)

# === FreeRTOS base ===
FREERTOS_DIR        := freertos-kernel
FREERTOS_CONFIG_DIR := $(INCLUDE_DIR)/freertos

# Use Cortex-M23 Non-TrustZone (non_secure)
FREERTOS_PORT := $(FREERTOS_DIR)/portable/GCC/ARM_CM23_NTZ/non_secure

# HEAP for dynamic allocation
FREERTOS_HEAP_C := $(FREERTOS_PORT)/MemMang/heap_4.c

ifeq ($(USE_FREERTOS),1)
  DEFINES   += -DUSE_FREERTOS
  INCLUDES  += -I$(FREERTOS_DIR)/include -I$(FREERTOS_PORT) -I$(FREERTOS_CONFIG_DIR)
  C_SOURCES += \
    $(FREERTOS_DIR)/list.c \
    $(FREERTOS_DIR)/queue.c \
    $(FREERTOS_DIR)/tasks.c \
    $(FREERTOS_DIR)/timers.c \
    $(FREERTOS_DIR)/event_groups.c \
    $(FREERTOS_DIR)/stream_buffer.c \
    $(FREERTOS_PORT)/port.c \
    $(FREERTOS_PORT)/portasm.c
  ifeq ($(USE_DYNAMIC),1)
    C_SOURCES += $(FREERTOS_HEAP_C)
  endif
endif

# === Set Flags ===
C_BASEFLAGS   := $(COMMON_FLAGS) $(C_STD)   $(DEFINES)
CXX_BASEFLAGS := $(COMMON_FLAGS) $(CXX_STD) $(DEFINES) \
                 -fno-exceptions -fno-rtti -fno-use-cxa-atexit \
                 -fno-non-call-exceptions -fno-threadsafe-statics

CFLAGS   := $(C_BASEFLAGS) $(OPT)
CXXFLAGS := $(CXX_BASEFLAGS) $(OPT)

# === Object mapping ===
C_OBJS   := $(patsubst %.c,  $(BUILD_DIR)/%.o, $(C_SOURCES))
CPP_OBJS := $(patsubst %.cpp,$(BUILD_DIR)/%.o, $(CPP_SOURCES))
ASM_OBJS := $(patsubst %.S,  $(BUILD_DIR)/%.o, $(ASM_SOURCES))
OBJS     := $(C_OBJS) $(CPP_OBJS) $(ASM_OBJS)

# === Default targets ===
.PHONY: all clean debug release build print-objs

all: clean debug release

clean:
	@rm -rf $(DEBUG_DIR) $(RELEASE_DIR)

debug:
	@$(MAKE) build BUILD_DIR=$(DEBUG_DIR)   OPT="$(DEBUG_OPT_FLAGS)"   BUILD_TYPE=DEBUG

release:
	@$(MAKE) build BUILD_DIR=$(RELEASE_DIR) OPT="$(RELEASE_OPT_FLAGS)" BUILD_TYPE=RELEASE

print-objs:
	@echo $(OBJS)

################################################################################
# Internal build targets
################################################################################
build: $(BUILD_DIR)/$(PROJECT).elf \
       $(BUILD_DIR)/$(PROJECT).lst \
       $(BUILD_DIR)/$(APPLICATION).bin \
       $(BUILD_DIR)/$(PROJECT).hex \
       $(BUILD_DIR)/$(PROJECT).map

################################################################################
# Build rules
################################################################################

# Compile C files
$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Compile C++ files
$(BUILD_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	@$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# Compile ASM files
$(BUILD_DIR)/%.o: %.S
	@mkdir -p $(dir $@)
	@$(CC) $(ASFLAGS) $(INCLUDES) -c $< -o $@

# Link all objects into ELF (prints memory usage + build type)
$(BUILD_DIR)/$(PROJECT).elf: $(OBJS)
	@mkdir -p $(dir $@)
	@echo "================ LINKING $(BUILD_TYPE) ================"
	@$(LD) $(CFLAGS) $(LDFLAGS) -Wl,-Map,$(BUILD_DIR)/$(PROJECT).map -o $@ $^

# Convert ELF to binary
$(BUILD_DIR)/$(APPLICATION).bin: $(BUILD_DIR)/$(PROJECT).elf
	@$(OBJCOPY) -O binary $< $@

# Convert ELF to Intel HEX
$(BUILD_DIR)/$(PROJECT).hex: $(BUILD_DIR)/$(PROJECT).elf
	@$(OBJCOPY) -O ihex $< $@

# Generate disassembly listing
$(BUILD_DIR)/$(PROJECT).lst: $(BUILD_DIR)/$(PROJECT).elf
	@$(OBJDUMP) -S $< > $@
