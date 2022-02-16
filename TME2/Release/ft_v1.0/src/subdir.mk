################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../ft_v1.0/src/automaton.c \
../ft_v1.0/src/broadcastlist.c \
../ft_v1.0/src/environment.c \
../ft_v1.0/src/event.c \
../ft_v1.0/src/instruction.c \
../ft_v1.0/src/locklist.c \
../ft_v1.0/src/scheduler.c \
../ft_v1.0/src/test.c \
../ft_v1.0/src/thread.c \
../ft_v1.0/src/threadlist.c 

O_SRCS += \
../ft_v1.0/src/automaton.o \
../ft_v1.0/src/broadcastlist.o \
../ft_v1.0/src/environment.o \
../ft_v1.0/src/event.o \
../ft_v1.0/src/instruction.o \
../ft_v1.0/src/locklist.o \
../ft_v1.0/src/scheduler.o \
../ft_v1.0/src/thread.o \
../ft_v1.0/src/threadlist.o 

OBJS += \
./ft_v1.0/src/automaton.o \
./ft_v1.0/src/broadcastlist.o \
./ft_v1.0/src/environment.o \
./ft_v1.0/src/event.o \
./ft_v1.0/src/instruction.o \
./ft_v1.0/src/locklist.o \
./ft_v1.0/src/scheduler.o \
./ft_v1.0/src/test.o \
./ft_v1.0/src/thread.o \
./ft_v1.0/src/threadlist.o 

C_DEPS += \
./ft_v1.0/src/automaton.d \
./ft_v1.0/src/broadcastlist.d \
./ft_v1.0/src/environment.d \
./ft_v1.0/src/event.d \
./ft_v1.0/src/instruction.d \
./ft_v1.0/src/locklist.d \
./ft_v1.0/src/scheduler.d \
./ft_v1.0/src/test.d \
./ft_v1.0/src/thread.d \
./ft_v1.0/src/threadlist.d 


# Each subdirectory must supply rules for building sources it contributes
ft_v1.0/src/%.o: ../ft_v1.0/src/%.c ft_v1.0/src/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


