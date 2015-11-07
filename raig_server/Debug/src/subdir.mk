################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../src/ai_manager.cc \
../src/raig_main.cc \
../src/raig_server_manager.cc 

CC_DEPS += \
./src/ai_manager.d \
./src/raig_main.d \
./src/raig_server_manager.d 

OBJS += \
./src/ai_manager.o \
./src/raig_main.o \
./src/raig_server_manager.o 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++0x -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


