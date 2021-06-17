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
all: FreeRTOS-template.axf

# Tool invocations
FreeRTOS-template.axf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: MCU Linker'
	arm-none-eabi-gcc -nostdlib -L"C:\Users\josee\Documents\ISEL\3_Ano-6_Semestre\SE2\CMSIS_CORE_LPC17xx\Debug" -L"C:\Users\josee\Documents\ISEL\3_Ano-6_Semestre\SE2\FreeRTOS-Kernel\Debug" -Xlinker -Map="FreeRTOS-template.map" -Xlinker --cref -Xlinker --gc-sections -Xlinker -print-memory-usage -mcpu=cortex-m3 -mthumb -T "FreeRTOS-template_Debug.ld" -o "FreeRTOS-template.axf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '
	$(MAKE) --no-print-directory post-build

# Other Targets
clean:
	-$(RM) $(EXECUTABLES)$(OBJS)$(C_DEPS) FreeRTOS-template.axf
	-@echo ' '

post-build:
	-@echo 'Performing post-build steps'
	-arm-none-eabi-size "FreeRTOS-template.axf"; # arm-none-eabi-objcopy -v -O binary "FreeRTOS-template.axf" "FreeRTOS-template.bin" ; # checksum -p LPC1769 -d "FreeRTOS-template.bin";
	-@echo ' '

.PHONY: all clean dependents post-build

-include ../makefile.targets
