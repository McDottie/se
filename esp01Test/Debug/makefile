################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: esp01Test.axf

# Tool invocations
esp01Test.axf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: MCU Linker'
	arm-none-eabi-gcc -nostdlib -L"G:\isel\SE2\se\CMSIS_CORE_LPC17xx\Debug" -L"G:\isel\SE2\se\FreeRTOS-Kernel\Debug" -L"G:\isel\SE2\se\SE2021\Debug" -Xlinker -Map="esp01Test.map" -Xlinker --cref -Xlinker --gc-sections -Xlinker -print-memory-usage -mcpu=cortex-m3 -mthumb -T "esp01Test_Debug.ld" -o "esp01Test.axf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '
	$(MAKE) --no-print-directory post-build

# Other Targets
clean:
	-$(RM) $(EXECUTABLES)$(OBJS)$(C_DEPS) esp01Test.axf
	-@echo ' '

post-build:
	-@echo 'Performing post-build steps'
	-arm-none-eabi-size "esp01Test.axf"; # arm-none-eabi-objcopy -v -O binary "esp01Test.axf" "esp01Test.bin" ; # checksum -p LPC1769 -d "esp01Test.bin";
	-@echo ' '

.PHONY: all clean dependents post-build

-include ../makefile.targets
