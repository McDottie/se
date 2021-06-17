################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/portable/MemMang/heap_4.c 

OBJS += \
./src/portable/MemMang/heap_4.o 

C_DEPS += \
./src/portable/MemMang/heap_4.d 


# Each subdirectory must supply rules for building sources it contributes
src/portable/MemMang/%.o: ../src/portable/MemMang/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__NEWLIB__ -I"C:\Users\josee\Documents\ISEL\3_Ano-6_Semestre\SE2\CMSIS_CORE_LPC17xx\inc" -I"C:\Users\josee\Documents\ISEL\3_Ano-6_Semestre\SE2\FreeRTOS-Kernel\include" -I"C:\Users\josee\Documents\ISEL\3_Ano-6_Semestre\SE2\FreeRTOS-Kernel\src\portable" -I"C:\Users\josee\Documents\ISEL\3_Ano-6_Semestre\SE2\esp01Test\inc" -O0 -g3 -Wall -c -fmessage-length=0 -fmerge-constants -fmacro-prefix-map="../$(@D)/"=. -mcpu=cortex-m3 -mthumb -D__NEWLIB__ -fstack-usage -specs=nano.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


