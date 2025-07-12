################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../fuzzyLib/Src/Fuzzy.cpp \
../fuzzyLib/Src/FuzzyComposition.cpp \
../fuzzyLib/Src/FuzzyIO.cpp \
../fuzzyLib/Src/FuzzyInput.cpp \
../fuzzyLib/Src/FuzzyOutput.cpp \
../fuzzyLib/Src/FuzzyRule.cpp \
../fuzzyLib/Src/FuzzyRuleAntecedent.cpp \
../fuzzyLib/Src/FuzzyRuleConsequent.cpp \
../fuzzyLib/Src/FuzzySet.cpp 

OBJS += \
./fuzzyLib/Src/Fuzzy.o \
./fuzzyLib/Src/FuzzyComposition.o \
./fuzzyLib/Src/FuzzyIO.o \
./fuzzyLib/Src/FuzzyInput.o \
./fuzzyLib/Src/FuzzyOutput.o \
./fuzzyLib/Src/FuzzyRule.o \
./fuzzyLib/Src/FuzzyRuleAntecedent.o \
./fuzzyLib/Src/FuzzyRuleConsequent.o \
./fuzzyLib/Src/FuzzySet.o 

CPP_DEPS += \
./fuzzyLib/Src/Fuzzy.d \
./fuzzyLib/Src/FuzzyComposition.d \
./fuzzyLib/Src/FuzzyIO.d \
./fuzzyLib/Src/FuzzyInput.d \
./fuzzyLib/Src/FuzzyOutput.d \
./fuzzyLib/Src/FuzzyRule.d \
./fuzzyLib/Src/FuzzyRuleAntecedent.d \
./fuzzyLib/Src/FuzzyRuleConsequent.d \
./fuzzyLib/Src/FuzzySet.d 


# Each subdirectory must supply rules for building sources it contributes
fuzzyLib/Src/%.o fuzzyLib/Src/%.su fuzzyLib/Src/%.cyclo: ../fuzzyLib/Src/%.cpp fuzzyLib/Src/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m3 -std=gnu++17 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I"D:/CE320_FuzzyLogic/project/ssd1306/Inc" -I"D:/CE320_FuzzyLogic/project/fuzzyLib/Inc" -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-fuzzyLib-2f-Src

clean-fuzzyLib-2f-Src:
	-$(RM) ./fuzzyLib/Src/Fuzzy.cyclo ./fuzzyLib/Src/Fuzzy.d ./fuzzyLib/Src/Fuzzy.o ./fuzzyLib/Src/Fuzzy.su ./fuzzyLib/Src/FuzzyComposition.cyclo ./fuzzyLib/Src/FuzzyComposition.d ./fuzzyLib/Src/FuzzyComposition.o ./fuzzyLib/Src/FuzzyComposition.su ./fuzzyLib/Src/FuzzyIO.cyclo ./fuzzyLib/Src/FuzzyIO.d ./fuzzyLib/Src/FuzzyIO.o ./fuzzyLib/Src/FuzzyIO.su ./fuzzyLib/Src/FuzzyInput.cyclo ./fuzzyLib/Src/FuzzyInput.d ./fuzzyLib/Src/FuzzyInput.o ./fuzzyLib/Src/FuzzyInput.su ./fuzzyLib/Src/FuzzyOutput.cyclo ./fuzzyLib/Src/FuzzyOutput.d ./fuzzyLib/Src/FuzzyOutput.o ./fuzzyLib/Src/FuzzyOutput.su ./fuzzyLib/Src/FuzzyRule.cyclo ./fuzzyLib/Src/FuzzyRule.d ./fuzzyLib/Src/FuzzyRule.o ./fuzzyLib/Src/FuzzyRule.su ./fuzzyLib/Src/FuzzyRuleAntecedent.cyclo ./fuzzyLib/Src/FuzzyRuleAntecedent.d ./fuzzyLib/Src/FuzzyRuleAntecedent.o ./fuzzyLib/Src/FuzzyRuleAntecedent.su ./fuzzyLib/Src/FuzzyRuleConsequent.cyclo ./fuzzyLib/Src/FuzzyRuleConsequent.d ./fuzzyLib/Src/FuzzyRuleConsequent.o ./fuzzyLib/Src/FuzzyRuleConsequent.su ./fuzzyLib/Src/FuzzySet.cyclo ./fuzzyLib/Src/FuzzySet.d ./fuzzyLib/Src/FuzzySet.o ./fuzzyLib/Src/FuzzySet.su

.PHONY: clean-fuzzyLib-2f-Src

