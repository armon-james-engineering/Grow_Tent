################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/ST7565/ST7565.c \
../Core/ST7565/bitmap.c \
../Core/ST7565/fonts.c 

OBJS += \
./Core/ST7565/ST7565.o \
./Core/ST7565/bitmap.o \
./Core/ST7565/fonts.o 

C_DEPS += \
./Core/ST7565/ST7565.d \
./Core/ST7565/bitmap.d \
./Core/ST7565/fonts.d 


# Each subdirectory must supply rules for building sources it contributes
Core/ST7565/%.o Core/ST7565/%.su: ../Core/ST7565/%.c Core/ST7565/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L432xx -c -I"C:/Users/armon/OneDrive/Documents/Embedded C Development/Code Files and Workspace/target/001GrowTent/Core/SD_Card" -I"C:/Users/armon/OneDrive/Documents/Embedded C Development/Code Files and Workspace/target/Grow_Tent/001GrowTent/Core/ST7565" -I../Core/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/armon/Documents/Embedded C/Grow_Tent/001GrowTent/Core/SD_Card" -I"C:/Users/armon/Documents/Embedded C/Grow_Tent/001GrowTent/Core/SD_Card/FATFS/App" -I"C:/Users/armon/Documents/Embedded C/Grow_Tent/001GrowTent/Core/SD_Card/FATFS/Target" -I"C:/Users/armon/Documents/Embedded C/Grow_Tent/001GrowTent/Core/SD_Card/Middlewares/Third_Party/FatFs/src" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-ST7565

clean-Core-2f-ST7565:
	-$(RM) ./Core/ST7565/ST7565.d ./Core/ST7565/ST7565.o ./Core/ST7565/ST7565.su ./Core/ST7565/bitmap.d ./Core/ST7565/bitmap.o ./Core/ST7565/bitmap.su ./Core/ST7565/fonts.d ./Core/ST7565/fonts.o ./Core/ST7565/fonts.su

.PHONY: clean-Core-2f-ST7565

