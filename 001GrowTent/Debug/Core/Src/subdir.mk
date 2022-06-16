################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/Clock_Control.c \
../Core/Src/GPIO_Control.c \
../Core/Src/LCD_Control.c \
../Core/Src/Monitor_GrowTent.c \
../Core/Src/Power_Control.c \
../Core/Src/RTC_Control.c \
../Core/Src/SD_Card_Control.c \
../Core/Src/TempHumidity_Control.c \
../Core/Src/TempHumidity_Timer2_Control.c \
../Core/Src/UART_Control.c \
../Core/Src/main.c \
../Core/Src/stm32l4xx_hal_msp.c \
../Core/Src/stm32l4xx_it.c \
../Core/Src/syscalls.c \
../Core/Src/sysmem.c \
../Core/Src/system_stm32l4xx.c 

OBJS += \
./Core/Src/Clock_Control.o \
./Core/Src/GPIO_Control.o \
./Core/Src/LCD_Control.o \
./Core/Src/Monitor_GrowTent.o \
./Core/Src/Power_Control.o \
./Core/Src/RTC_Control.o \
./Core/Src/SD_Card_Control.o \
./Core/Src/TempHumidity_Control.o \
./Core/Src/TempHumidity_Timer2_Control.o \
./Core/Src/UART_Control.o \
./Core/Src/main.o \
./Core/Src/stm32l4xx_hal_msp.o \
./Core/Src/stm32l4xx_it.o \
./Core/Src/syscalls.o \
./Core/Src/sysmem.o \
./Core/Src/system_stm32l4xx.o 

C_DEPS += \
./Core/Src/Clock_Control.d \
./Core/Src/GPIO_Control.d \
./Core/Src/LCD_Control.d \
./Core/Src/Monitor_GrowTent.d \
./Core/Src/Power_Control.d \
./Core/Src/RTC_Control.d \
./Core/Src/SD_Card_Control.d \
./Core/Src/TempHumidity_Control.d \
./Core/Src/TempHumidity_Timer2_Control.d \
./Core/Src/UART_Control.d \
./Core/Src/main.d \
./Core/Src/stm32l4xx_hal_msp.d \
./Core/Src/stm32l4xx_it.d \
./Core/Src/syscalls.d \
./Core/Src/sysmem.d \
./Core/Src/system_stm32l4xx.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/%.o Core/Src/%.su: ../Core/Src/%.c Core/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L432xx -c -I"C:/Users/armon/OneDrive/Documents/Embedded C Development/Code Files and Workspace/target/001GrowTent/Core/SD_Card" -I../Core/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/armon/OneDrive/Documents/Embedded C Development/Code Files and Workspace/target/Grow_Tent/001GrowTent/Core/SD_Card" -I"C:/Users/armon/OneDrive/Documents/Embedded C Development/Code Files and Workspace/target/Grow_Tent/001GrowTent/Core/SD_Card/FATFS/App" -I"C:/Users/armon/OneDrive/Documents/Embedded C Development/Code Files and Workspace/target/Grow_Tent/001GrowTent/Core/SD_Card/FATFS/Target" -I"C:/Users/armon/OneDrive/Documents/Embedded C Development/Code Files and Workspace/target/Grow_Tent/001GrowTent/Core/SD_Card/Middlewares/Third_Party/FatFs/src" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-Src

clean-Core-2f-Src:
	-$(RM) ./Core/Src/Clock_Control.d ./Core/Src/Clock_Control.o ./Core/Src/Clock_Control.su ./Core/Src/GPIO_Control.d ./Core/Src/GPIO_Control.o ./Core/Src/GPIO_Control.su ./Core/Src/LCD_Control.d ./Core/Src/LCD_Control.o ./Core/Src/LCD_Control.su ./Core/Src/Monitor_GrowTent.d ./Core/Src/Monitor_GrowTent.o ./Core/Src/Monitor_GrowTent.su ./Core/Src/Power_Control.d ./Core/Src/Power_Control.o ./Core/Src/Power_Control.su ./Core/Src/RTC_Control.d ./Core/Src/RTC_Control.o ./Core/Src/RTC_Control.su ./Core/Src/SD_Card_Control.d ./Core/Src/SD_Card_Control.o ./Core/Src/SD_Card_Control.su ./Core/Src/TempHumidity_Control.d ./Core/Src/TempHumidity_Control.o ./Core/Src/TempHumidity_Control.su ./Core/Src/TempHumidity_Timer2_Control.d ./Core/Src/TempHumidity_Timer2_Control.o ./Core/Src/TempHumidity_Timer2_Control.su ./Core/Src/UART_Control.d ./Core/Src/UART_Control.o ./Core/Src/UART_Control.su ./Core/Src/main.d ./Core/Src/main.o ./Core/Src/main.su ./Core/Src/stm32l4xx_hal_msp.d ./Core/Src/stm32l4xx_hal_msp.o ./Core/Src/stm32l4xx_hal_msp.su ./Core/Src/stm32l4xx_it.d ./Core/Src/stm32l4xx_it.o ./Core/Src/stm32l4xx_it.su ./Core/Src/syscalls.d ./Core/Src/syscalls.o ./Core/Src/syscalls.su ./Core/Src/sysmem.d ./Core/Src/sysmem.o ./Core/Src/sysmem.su ./Core/Src/system_stm32l4xx.d ./Core/Src/system_stm32l4xx.o ./Core/Src/system_stm32l4xx.su

.PHONY: clean-Core-2f-Src

