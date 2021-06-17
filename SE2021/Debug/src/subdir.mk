################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/ADXL345.c \
../src/EEPROM.c \
../src/ESP01.c \
../src/Flash.c \
../src/NTP.c \
../src/button.c \
../src/i2c.c \
../src/lcd.c \
../src/led.c \
../src/printf-stdarg.c \
../src/rtc.c \
../src/spi.c \
../src/uart.c \
../src/utils.c \
../src/wait.c 

OBJS += \
./src/ADXL345.o \
./src/EEPROM.o \
./src/ESP01.o \
./src/Flash.o \
./src/NTP.o \
./src/button.o \
./src/i2c.o \
./src/lcd.o \
./src/led.o \
./src/printf-stdarg.o \
./src/rtc.o \
./src/spi.o \
./src/uart.o \
./src/utils.o \
./src/wait.o 

C_DEPS += \
./src/ADXL345.d \
./src/EEPROM.d \
./src/ESP01.d \
./src/Flash.d \
./src/NTP.d \
./src/button.d \
./src/i2c.d \
./src/lcd.d \
./src/led.d \
./src/printf-stdarg.d \
./src/rtc.d \
./src/spi.d \
./src/uart.d \
./src/utils.d \
./src/wait.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DDEBUG -D__CODE_RED -DCORE_M3 -D__USE_CMSIS=CMSIS_CORE_LPC17xx -D__LPC17XX__ -D__NEWLIB__ -I"C:\Users\josee\Documents\ISEL\3_Ano-6_Semestre\SE2\SE2021\inc" -I"C:\Users\josee\Documents\ISEL\3_Ano-6_Semestre\SE2\FreeRTOS-test\inc" -I"C:\Users\josee\Documents\ISEL\3_Ano-6_Semestre\SE2\CMSIS_CORE_LPC17xx\inc" -I"C:\Users\josee\Documents\ISEL\3_Ano-6_Semestre\SE2\FreeRTOS-Kernel\include" -I"C:\Users\josee\Documents\ISEL\3_Ano-6_Semestre\SE2\FreeRTOS-Kernel\src\portable" -O0 -fno-common -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -fmerge-constants -fmacro-prefix-map="../$(@D)/"=. -mcpu=cortex-m3 -mthumb -D__NEWLIB__ -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


