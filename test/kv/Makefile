
######################################
# target
######################################
TARGET = BabyOS


######################################
# building variables
######################################
# debug build?
DEBUG = 1
# optimization
OPT = -O3


#######################################
# paths
#######################################
# Build path
BUILD_DIR = build

######################################
# source
######################################
SOURCE_ROOT = ../..
# C sources
BOS_SOURCE_DIR = \
$(SOURCE_ROOT)/bos/algorithm/*.c \
$(SOURCE_ROOT)/bos/core/*.c \
$(SOURCE_ROOT)/bos/hal/*.c \
$(SOURCE_ROOT)/bos/modules/*.c \
$(SOURCE_ROOT)/bos/utils/*.c \
$(SOURCE_ROOT)/bos/thirdparty/fatfs/*.c \
$(SOURCE_ROOT)/bos/thirdparty/flexiblebutton/*.c \
$(SOURCE_ROOT)/bos/thirdparty/littlefs/*.c \
$(SOURCE_ROOT)/bos/thirdparty/nr_micro_shell/src/*.c \
$(SOURCE_ROOT)/bos/drivers/sfud/*.c \
$(SOURCE_ROOT)/bos/thirdparty/ugui/*.c \
$(SOURCE_ROOT)/bos/thirdparty/coremark/*.c \
$(SOURCE_ROOT)/bos/thirdparty/cjson/*.c  \
$(SOURCE_ROOT)/bos/thirdparty/unity/*.c  

BOS_DRV_SOURCE_DIR = \
$(SOURCE_ROOT)/bos/drivers/b_drv_testflash.c

EXAMPLE_SOURCE_DIR = \
$(SOURCE_ROOT)/test/*.c \
./*.c


C_SOURCES =  $(wildcard $(BOS_SOURCE_DIR) $(BOS_DRV_SOURCE_DIR) $(EXAMPLE_SOURCE_DIR))

# ASM sources
ASM_SOURCES =  


#######################################
# binaries
#######################################
PREFIX = 
# The gcc compiler bin path can be either defined in make command via GCC_PATH variable (> make GCC_PATH=xxx)
# either it can be added to the PATH environment variable.
ifdef GCC_PATH
CC = $(GCC_PATH)/$(PREFIX)gcc
AS = $(GCC_PATH)/$(PREFIX)gcc -x assembler-with-cpp
CP = $(GCC_PATH)/$(PREFIX)objcopy
SZ = $(GCC_PATH)/$(PREFIX)size
else
CC = $(PREFIX)gcc
AS = $(PREFIX)gcc -x assembler-with-cpp
CP = $(PREFIX)objcopy
SZ = $(PREFIX)size
endif
 
#######################################
# CFLAGS
#######################################

# fpu
# NONE for Cortex-M0/M0+/M3

# float-abi

# macros for gcc
# AS defines
AS_DEFS = 

# C defines
C_DEFS =  

# AS includes
AS_INCLUDES = 

# C includes
C_INCLUDES =  \
-I$(SOURCE_ROOT)/bos \
-I$(SOURCE_ROOT)/test/ \
-I../ \
-I./_config/ \

# compile gcc flags
ASFLAGS = $(AS_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections

CFLAGS = $(C_DEFS) $(C_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections -std=c99

ifeq ($(DEBUG), 1)
CFLAGS += -g -gdwarf-2
endif


# Generate dependency information
CFLAGS += -MMD -MP -MF"$(@:%.o=%.d)"


#######################################
# LDFLAGS
#######################################
# link script
LDSCRIPT = $(SOURCE_ROOT)/test/babyos.ld
LDFLAGS = -T$(LDSCRIPT) -lpthread
# libraries  


# default action: build all -specs=nano.specs  
all: $(BUILD_DIR)/$(TARGET)


#######################################
# build the application
#######################################
# list of objects
OBJECTS = $(addprefix $(BUILD_DIR)/,$(notdir $(C_SOURCES:.c=.o)))
vpath %.c $(sort $(dir $(C_SOURCES)))
# list of ASM program objects
OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(ASM_SOURCES:.s=.o)))
vpath %.s $(sort $(dir $(ASM_SOURCES)))

$(BUILD_DIR)/%.o: %.c Makefile | $(BUILD_DIR) 
	$(CC) -c $(CFLAGS) -Wa,-a,-ad,-alms=$(BUILD_DIR)/$(notdir $(<:.c=.lst)) $< -o $@

$(BUILD_DIR)/%.o: %.s Makefile | $(BUILD_DIR)
	$(AS) -c $(CFLAGS) $< -o $@

$(BUILD_DIR)/$(TARGET): $(OBJECTS) Makefile
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@
	$(SZ) $@
	
$(BUILD_DIR):
	mkdir $@		

#######################################
# clean up
#######################################
clean:
	-rm -fR $(BUILD_DIR)

#######################################
# menuconfig
#######################################
menuconfig:
	cd _config ; ./b_config.sh

#######################################
# dependencies
#######################################
-include $(wildcard $(BUILD_DIR)/*.d)

# *** EOF ***
