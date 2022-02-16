################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../ft_v1.0/examples/dialogc.c \
../ft_v1.0/examples/dialogft.c \
../ft_v1.0/examples/hello.c \
../ft_v1.0/examples/nbread.c \
../ft_v1.0/examples/ndhello.c \
../ft_v1.0/examples/printer.c \
../ft_v1.0/examples/prodcons1c.c \
../ft_v1.0/examples/prodcons1ft.c \
../ft_v1.0/examples/prodconsc.c \
../ft_v1.0/examples/prodconsft.c \
../ft_v1.0/examples/twoserversft.c 

OBJS += \
./ft_v1.0/examples/dialogc.o \
./ft_v1.0/examples/dialogft.o \
./ft_v1.0/examples/hello.o \
./ft_v1.0/examples/nbread.o \
./ft_v1.0/examples/ndhello.o \
./ft_v1.0/examples/printer.o \
./ft_v1.0/examples/prodcons1c.o \
./ft_v1.0/examples/prodcons1ft.o \
./ft_v1.0/examples/prodconsc.o \
./ft_v1.0/examples/prodconsft.o \
./ft_v1.0/examples/twoserversft.o 

C_DEPS += \
./ft_v1.0/examples/dialogc.d \
./ft_v1.0/examples/dialogft.d \
./ft_v1.0/examples/hello.d \
./ft_v1.0/examples/nbread.d \
./ft_v1.0/examples/ndhello.d \
./ft_v1.0/examples/printer.d \
./ft_v1.0/examples/prodcons1c.d \
./ft_v1.0/examples/prodcons1ft.d \
./ft_v1.0/examples/prodconsc.d \
./ft_v1.0/examples/prodconsft.d \
./ft_v1.0/examples/twoserversft.d 


# Each subdirectory must supply rules for building sources it contributes
ft_v1.0/examples/%.o: ../ft_v1.0/examples/%.c ft_v1.0/examples/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


