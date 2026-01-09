################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/bluetooth.c \
../Src/debug.c \
../Src/deviceCtrl.c \
../Src/main.c \
../Src/servo.c \
../Src/syscalls.c \
../Src/sysmem.c \
../Src/util.c 

OBJS += \
./Src/bluetooth.o \
./Src/debug.o \
./Src/deviceCtrl.o \
./Src/main.o \
./Src/servo.o \
./Src/syscalls.o \
./Src/sysmem.o \
./Src/util.o 

C_DEPS += \
./Src/bluetooth.d \
./Src/debug.d \
./Src/deviceCtrl.d \
./Src/main.d \
./Src/servo.d \
./Src/syscalls.d \
./Src/sysmem.d \
./Src/util.d 


# Each subdirectory must supply rules for building sources it contributes
Src/%.o Src/%.su Src/%.cyclo: ../Src/%.c Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DSTM32F103RBTx -DSTM32 -DSTM32F1 -DNUCLEO_F103RB -c -I../driver/CMSIS/Device/ST/STM32F1xx/Include -I../driver/CMSIS/Include -I../driver/include -I../Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Src

clean-Src:
	-$(RM) ./Src/bluetooth.cyclo ./Src/bluetooth.d ./Src/bluetooth.o ./Src/bluetooth.su ./Src/debug.cyclo ./Src/debug.d ./Src/debug.o ./Src/debug.su ./Src/deviceCtrl.cyclo ./Src/deviceCtrl.d ./Src/deviceCtrl.o ./Src/deviceCtrl.su ./Src/main.cyclo ./Src/main.d ./Src/main.o ./Src/main.su ./Src/servo.cyclo ./Src/servo.d ./Src/servo.o ./Src/servo.su ./Src/syscalls.cyclo ./Src/syscalls.d ./Src/syscalls.o ./Src/syscalls.su ./Src/sysmem.cyclo ./Src/sysmem.d ./Src/sysmem.o ./Src/sysmem.su ./Src/util.cyclo ./Src/util.d ./Src/util.o ./Src/util.su

.PHONY: clean-Src

