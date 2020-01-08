################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Utilities/sequencer/stm32_seq.c 

OBJS += \
./Utilities/sequencer/stm32_seq.o 

C_DEPS += \
./Utilities/sequencer/stm32_seq.d 


# Each subdirectory must supply rules for building sources it contributes
Utilities/sequencer/stm32_seq.o: ../Utilities/sequencer/stm32_seq.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DDEBUG -DSTM32WB55xx -c -I../Drivers/BSP/Components/Common/ -I../Middlewares/ST/STM32_WPAN/ble/core/template -I../MEMS/Target -I../tiny_lpm -I../Middlewares/ST/STM32_WPAN/ble/core -I../Utilities/lpm/tiny_lpm -I../STM32_WPAN/App -I../Middlewares/ST/STM32_WPAN/ble -I../Middlewares/ST/STM32_WPAN/interface/patterns/ble_thread/tl -I../Middlewares/ST/STM32_WPAN/ble/svc/Inc -I../Drivers/BSP/Components/lsm6dsl/ -I../Middlewares/ST/STM32_WPAN/ble/core/auto -I../Core/Inc -I../Utilities/sequencer -I../Drivers/BSP/Components/hts221/ -I../Drivers/STM32WBxx_HAL_Driver/Inc -I../Middlewares/ST/STM32_WPAN/ble/svc/Src -I../Middlewares/ST/STM32_WPAN/interface/patterns/ble_thread/shci -I../Middlewares/ST/STM32_WPAN -I../Drivers/BSP/Components/lsm303agr/ -I..//CMSIS/DSP/Include/ -I../MEMS/App -I../Drivers/CMSIS/Include -I../Drivers/BSP/Components/lps22hb/ -I../Middlewares/ST/STM32_WPAN/utilities -I../Middlewares/ST/STM32_WPAN/interface/patterns/ble_thread -I../Drivers/BSP/IKS01A2/ -I../Drivers/STM32WBxx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32WBxx/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Utilities/sequencer/stm32_seq.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

