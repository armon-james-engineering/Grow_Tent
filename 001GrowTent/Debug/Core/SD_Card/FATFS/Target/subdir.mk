################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/SD_Card/FATFS/Target/user_diskio.c \
../Core/SD_Card/FATFS/Target/user_diskio_spi.c 

OBJS += \
./Core/SD_Card/FATFS/Target/user_diskio.o \
./Core/SD_Card/FATFS/Target/user_diskio_spi.o 

C_DEPS += \
./Core/SD_Card/FATFS/Target/user_diskio.d \
./Core/SD_Card/FATFS/Target/user_diskio_spi.d 


# Each subdirectory must supply rules for building sources it contributes
Core/SD_Card/FATFS/Target/%.o Core/SD_Card/FATFS/Target/%.su: ../Core/SD_Card/FATFS/Target/%.c Core/SD_Card/FATFS/Target/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L432xx -c -I"C:/Users/armon/OneDrive/Documents/Embedded C Development/Code Files and Workspace/target/001GrowTent/Core/SD_Card" -I../Core/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/armon/Documents/Embedded C/Grow_Tent/001GrowTent/Core/SD_Card" -I"C:/Users/armon/Documents/Embedded C/Grow_Tent/001GrowTent/Core/SD_Card/FATFS/App" -I"C:/Users/armon/Documents/Embedded C/Grow_Tent/001GrowTent/Core/SD_Card/FATFS/Target" -I"C:/Users/armon/Documents/Embedded C/Grow_Tent/001GrowTent/Core/SD_Card/Middlewares/Third_Party/FatFs/src" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-SD_Card-2f-FATFS-2f-Target

clean-Core-2f-SD_Card-2f-FATFS-2f-Target:
	-$(RM) ./Core/SD_Card/FATFS/Target/user_diskio.d ./Core/SD_Card/FATFS/Target/user_diskio.o ./Core/SD_Card/FATFS/Target/user_diskio.su ./Core/SD_Card/FATFS/Target/user_diskio_spi.d ./Core/SD_Card/FATFS/Target/user_diskio_spi.o ./Core/SD_Card/FATFS/Target/user_diskio_spi.su

.PHONY: clean-Core-2f-SD_Card-2f-FATFS-2f-Target

