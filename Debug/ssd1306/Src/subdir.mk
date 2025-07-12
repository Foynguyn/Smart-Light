################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../ssd1306/Src/fonts.c \
../ssd1306/Src/ssd1306.c 

C_DEPS += \
./ssd1306/Src/fonts.d \
./ssd1306/Src/ssd1306.d 

OBJS += \
./ssd1306/Src/fonts.o \
./ssd1306/Src/ssd1306.o 


# Each subdirectory must supply rules for building sources it contributes
ssd1306/Src/%.o ssd1306/Src/%.su ssd1306/Src/%.cyclo: ../ssd1306/Src/%.c ssd1306/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I"D:/CE320_FuzzyLogic/project/ssd1306/Inc" -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-ssd1306-2f-Src

clean-ssd1306-2f-Src:
	-$(RM) ./ssd1306/Src/fonts.cyclo ./ssd1306/Src/fonts.d ./ssd1306/Src/fonts.o ./ssd1306/Src/fonts.su ./ssd1306/Src/ssd1306.cyclo ./ssd1306/Src/ssd1306.d ./ssd1306/Src/ssd1306.o ./ssd1306/Src/ssd1306.su

.PHONY: clean-ssd1306-2f-Src

