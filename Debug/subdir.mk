################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../filterStorage.c \
../filters.c \
../main.c \
../qrs.c \
../sensor.c 

OBJS += \
./filterStorage.o \
./filters.o \
./main.o \
./qrs.o \
./sensor.o 

C_DEPS += \
./filterStorage.d \
./filters.d \
./main.d \
./qrs.d \
./sensor.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -pg -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


