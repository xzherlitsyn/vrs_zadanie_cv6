################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/HTS221/hts221.c 

OBJS += \
./Core/Src/HTS221/hts221.o 

C_DEPS += \
./Core/Src/HTS221/hts221.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/HTS221/%.o Core/Src/HTS221/%.su Core/Src/HTS221/%.cyclo: ../Core/Src/HTS221/%.c Core/Src/HTS221/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DSTM32F303x8 -DUSE_FULL_LL_DRIVER -DHSE_VALUE=8000000 -DHSE_STARTUP_TIMEOUT=100 -DLSE_STARTUP_TIMEOUT=5000 -DLSE_VALUE=32768 -DEXTERNAL_CLOCK_VALUE=8000000 -DHSI_VALUE=8000000 -DLSI_VALUE=40000 -DVDD_VALUE=3300 -DPREFETCH_ENABLE=1 -c -I../Core/Inc -I../Drivers/STM32F3xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F3xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-HTS221

clean-Core-2f-Src-2f-HTS221:
	-$(RM) ./Core/Src/HTS221/hts221.cyclo ./Core/Src/HTS221/hts221.d ./Core/Src/HTS221/hts221.o ./Core/Src/HTS221/hts221.su

.PHONY: clean-Core-2f-Src-2f-HTS221

