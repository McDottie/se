################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/CarRunner.c \
../src/cr_startup_lpc175x_6x-FreeRTOS.c \
../src/crp.c \
../src/definitions_variables.c \
../src/game.c \
../src/mqtt.c \
../src/network.c \
../src/publisher.c \
../src/saver.c \
../src/time_helper.c 

OBJS += \
./src/CarRunner.o \
./src/cr_startup_lpc175x_6x-FreeRTOS.o \
./src/crp.o \
./src/definitions_variables.o \
./src/game.o \
./src/mqtt.o \
./src/network.o \
./src/publisher.o \
./src/saver.o \
./src/time_helper.o 

C_DEPS += \
./src/CarRunner.d \
./src/cr_startup_lpc175x_6x-FreeRTOS.d \
./src/crp.d \
./src/definitions_variables.d \
./src/game.d \
./src/mqtt.d \
./src/network.d \
./src/publisher.d \
./src/saver.d \
./src/time_helper.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DDEBUG -D__CODE_RED -DCORE_M3 -D__USE_CMSIS=CMSIS_CORE_LPC17xx -D__LPC17XX__ -D__NEWLIB__ -I"G:\isel\SE2\se\CarRunner\inc" -I"G:\isel\SE2\se\MQTTPacket\inc" -I"G:\isel\SE2\se\SE2021\inc" -I"G:\isel\SE2\se\CMSIS_CORE_LPC17xx\inc" -I"G:\isel\SE2\se\FreeRTOS-Kernel\include" -I"G:\isel\SE2\se\FreeRTOS-Kernel\src\portable" -O0 -fno-common -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -fmerge-constants -fmacro-prefix-map="../$(@D)/"=. -mcpu=cortex-m3 -mthumb -D__NEWLIB__ -fstack-usage -specs=nano.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


