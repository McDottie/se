################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/I2CTest.c \
../src/cr_startup_lpc175x_6x-FreeRTOS.c \
../src/crp.c 

OBJS += \
./src/I2CTest.o \
./src/cr_startup_lpc175x_6x-FreeRTOS.o \
./src/crp.o 

C_DEPS += \
./src/I2CTest.d \
./src/cr_startup_lpc175x_6x-FreeRTOS.d \
./src/crp.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DDEBUG -D__CODE_RED -DCORE_M3 -D__USE_CMSIS=CMSIS_CORE_LPC17xx -D__LPC17XX__ -D__REDLIB__ -I"G:\isel\SE2\se\I2CTest\inc" -I"G:\isel\SE2\se\SE2021\inc" -I"G:\isel\SE2\se\CMSIS_CORE_LPC17xx\inc" -I"G:\isel\SE2\se\FreeRTOS-Kernel\src\portable" -I"G:\isel\SE2\se\FreeRTOS-Kernel\include" -O0 -fno-common -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -fmerge-constants -fmacro-prefix-map="../$(@D)/"=. -mcpu=cortex-m3 -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


