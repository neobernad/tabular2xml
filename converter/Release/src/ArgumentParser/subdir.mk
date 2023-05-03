################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/ArgumentParser/ArgumentParser.cpp 

OBJS += \
./src/ArgumentParser/ArgumentParser.o 

CPP_DEPS += \
./src/ArgumentParser/ArgumentParser.d 


# Each subdirectory must supply rules for building sources it contributes
src/ArgumentParser/%.o: ../src/ArgumentParser/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cygwin C++ Compiler'
	g++ -std=c++0x -I/usr/include/libxml2/ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


