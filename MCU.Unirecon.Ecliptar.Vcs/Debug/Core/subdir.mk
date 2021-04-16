################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Core/main.cpp 

OBJS += \
./Core/main.o 

CPP_DEPS += \
./Core/main.d 


# Each subdirectory must supply rules for building sources it contributes
Core/main.o: ../Core/main.cpp Core/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -g3 -DUSE_HAL_DRIVER -DSTM32F446xx -DDEBUG -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I"E:/_DEVELING/_PROJECTS/MCU/Unirecon/Ecliptar/Devel/MCU.Unirecon.Ecliptar.Vcs/Core/Inc/Config" -I"E:/_DEVELING/_PROJECTS/MCU/Unirecon/Ecliptar/Devel/MCU.Unirecon.Ecliptar.Vcs/Core" -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -MMD -MP -MF"Core/main.d" -MT"$@" --specs=nano_c_standard_cpp.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

