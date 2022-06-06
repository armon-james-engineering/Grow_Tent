################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/SD_Card/Middlewares/Third_Party/FatFs/src/diskio.c \
../Core/SD_Card/Middlewares/Third_Party/FatFs/src/ff.c \
../Core/SD_Card/Middlewares/Third_Party/FatFs/src/ff_gen_drv.c 

OBJS += \
./Core/SD_Card/Middlewares/Third_Party/FatFs/src/diskio.o \
./Core/SD_Card/Middlewares/Third_Party/FatFs/src/ff.o \
./Core/SD_Card/Middlewares/Third_Party/FatFs/src/ff_gen_drv.o 

C_DEPS += \
./Core/SD_Card/Middlewares/Third_Party/FatFs/src/diskio.d \
./Core/SD_Card/Middlewares/Third_Party/FatFs/src/ff.d \
./Core/SD_Card/Middlewares/Third_Party/FatFs/src/ff_gen_drv.d 


# Each subdirectory must supply rules for building sources it contributes
Core/SD_Card/Middlewares/Third_Party/FatFs/src/%.o Core/SD_Card/Middlewares/Third_Party/FatFs/src/%.su: ../Core/SD_Card/Middlewares/Third_Party/FatFs/src/%.c Core/SD_Card/Middlewares/Third_Party/FatFs/src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -DUSE_HAL_DRIVER -DSTM32L432xx -c -I../Core/Inc -I"C:/Users/armon/OneDrive/Documents/Embedded C Development/Code Files and Workspace/target/001GrowTent/Core/SD_Card" -I"C:/Users/armon/OneDrive/Documents/Embedded C Development/Code Files and Workspace/target/001GrowTent/Core/SD_Card/FATFS/App" -I"C:/Users/armon/OneDrive/Documents/Embedded C Development/Code Files and Workspace/target/001GrowTent/Core/SD_Card/FATFS/Target" -I"C:/Users/armon/OneDrive/Documents/Embedded C Development/Code Files and Workspace/target/001GrowTent/Core/SD_Card/Middlewares/Third_Party/FatFs/src" -I"C:/Users/armon/OneDrive/Documents/Embedded C Development/Code Files and Workspace/target/001GrowTent/Core/SD_Card/Middlewares/Third_Party/FatFs/src/option" -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-SD_Card-2f-Middlewares-2f-Third_Party-2f-FatFs-2f-src

clean-Core-2f-SD_Card-2f-Middlewares-2f-Third_Party-2f-FatFs-2f-src:
	-$(RM) ./Core/SD_Card/Middlewares/Third_Party/FatFs/src/diskio.d ./Core/SD_Card/Middlewares/Third_Party/FatFs/src/diskio.o ./Core/SD_Card/Middlewares/Third_Party/FatFs/src/diskio.su ./Core/SD_Card/Middlewares/Third_Party/FatFs/src/ff.d ./Core/SD_Card/Middlewares/Third_Party/FatFs/src/ff.o ./Core/SD_Card/Middlewares/Third_Party/FatFs/src/ff.su ./Core/SD_Card/Middlewares/Third_Party/FatFs/src/ff_gen_drv.d ./Core/SD_Card/Middlewares/Third_Party/FatFs/src/ff_gen_drv.o ./Core/SD_Card/Middlewares/Third_Party/FatFs/src/ff_gen_drv.su

.PHONY: clean-Core-2f-SD_Card-2f-Middlewares-2f-Third_Party-2f-FatFs-2f-src

