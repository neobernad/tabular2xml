################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Tokenizer/Tokenizer.cpp 

OBJS += \
./src/Tokenizer/Tokenizer.o 

CPP_DEPS += \
./src/Tokenizer/Tokenizer.d 


# Each subdirectory must supply rules for building sources it contributes
src/Tokenizer/%.o: ../src/Tokenizer/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cygwin C++ Compiler'
	g++ -std=c++0x -I/usr/include/libxml2/ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


