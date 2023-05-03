################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Configurator/Option/Option.cpp 

OBJS += \
./src/Configurator/Option/Option.o 

CPP_DEPS += \
./src/Configurator/Option/Option.d 


# Each subdirectory must supply rules for building sources it contributes
src/Configurator/Option/%.o: ../src/Configurator/Option/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cygwin C++ Compiler'
	g++ -std=c++0x -I/usr/include/libxml2/ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


