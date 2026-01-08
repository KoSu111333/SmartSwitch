################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../driver/buffer.c \
../driver/drv_gpio.c \
../driver/drv_i2c.c \
../driver/drv_rcc.c \
../driver/drv_timer.c \
../driver/drv_uart.c \
../driver/interrupt.c 

OBJS += \
./driver/buffer.o \
./driver/drv_gpio.o \
./driver/drv_i2c.o \
./driver/drv_rcc.o \
./driver/drv_timer.o \
./driver/drv_uart.o \
./driver/interrupt.o 

C_DEPS += \
./driver/buffer.d \
./driver/drv_gpio.d \
./driver/drv_i2c.d \
./driver/drv_rcc.d \
./driver/drv_timer.d \
./driver/drv_uart.d \
./driver/interrupt.d 


# Each subdirectory must supply rules for building sources it contributes
driver/%.o driver/%.su driver/%.cyclo: ../driver/%.c driver/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DSTM32F103RBTx -DSTM32 -DSTM32F1 -DNUCLEO_F103RB -c -I../driver/CMSIS/Device/ST/STM32F1xx/Include -I../driver/CMSIS/Include -I../driver/include -I../Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-driver

clean-driver:
	-$(RM) ./driver/buffer.cyclo ./driver/buffer.d ./driver/buffer.o ./driver/buffer.su ./driver/drv_gpio.cyclo ./driver/drv_gpio.d ./driver/drv_gpio.o ./driver/drv_gpio.su ./driver/drv_i2c.cyclo ./driver/drv_i2c.d ./driver/drv_i2c.o ./driver/drv_i2c.su ./driver/drv_rcc.cyclo ./driver/drv_rcc.d ./driver/drv_rcc.o ./driver/drv_rcc.su ./driver/drv_timer.cyclo ./driver/drv_timer.d ./driver/drv_timer.o ./driver/drv_timer.su ./driver/drv_uart.cyclo ./driver/drv_uart.d ./driver/drv_uart.o ./driver/drv_uart.su ./driver/interrupt.cyclo ./driver/interrupt.d ./driver/interrupt.o ./driver/interrupt.su

.PHONY: clean-driver

