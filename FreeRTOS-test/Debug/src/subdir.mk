################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/cr_startup_lpc175x_6x-FreeRTOS.c \
../src/crp.c \
../src/main.c \
../src/printf-stdarg.c 

OBJS += \
./src/cr_startup_lpc175x_6x-FreeRTOS.o \
./src/crp.o \
./src/main.o \
./src/printf-stdarg.o 

C_DEPS += \
./src/cr_startup_lpc175x_6x-FreeRTOS.d \
./src/crp.d \
./src/main.d \
./src/printf-stdarg.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DDEBUG -D__CODE_RED -DCORE_M3 -D__USE_CMSIS=CMSIS_CORE_LPC17xx -D__LPC17XX__ -D__NEWLIB__ -I"C:\Users\josee\Documents\ISEL\3_Ano-6_Semestre\SE2\SE2021\inc" -I"C:\Users\josee\Documents\ISEL\3_Ano-6_Semestre\SE2\FreeRTOS-test\inc" -I"C:\Users\josee\Documents\ISEL\3_Ano-6_Semestre\SE2\CMSIS_CORE_LPC17xx\inc" -I"C:\Users\josee\Documents\ISEL\3_Ano-6_Semestre\SE2\FreeRTOS-Kernel\include" -I"C:\Users\josee\Documents\ISEL\3_Ano-6_Semestre\SE2\FreeRTOS-Kernel\src\portable" -O0 -fno-common -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -fmerge-constants -fmacro-prefix-map="../$(@D)/"=. -mcpu=cortex-m3 -mthumb -D__NEWLIB__ -fstack-usage -specs=nano.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


