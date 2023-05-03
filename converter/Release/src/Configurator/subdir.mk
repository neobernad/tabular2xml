################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Configurator/Configurator.cpp 

OBJS += \
./src/Configurator/Configurator.o 

CPP_DEPS += \
./src/Configurator/Configurator.d 


# Each subdirectory must supply rules for building sources it contributes
src/Configurator/%.o: ../src/Configurator/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cygwin C++ Compiler'
	g++ -std=c++0x -I/usr/include/libxml2/ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


