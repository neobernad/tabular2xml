################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Factory/Factory.cpp 

OBJS += \
./src/Factory/Factory.o 

CPP_DEPS += \
./src/Factory/Factory.d 


# Each subdirectory must supply rules for building sources it contributes
src/Factory/%.o: ../src/Factory/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cygwin C++ Compiler'
	g++ -std=c++0x -I/usr/include/libxml2/ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


