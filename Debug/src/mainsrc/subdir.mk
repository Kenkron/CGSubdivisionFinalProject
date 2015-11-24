################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
O_SRCS += \
../src/mainsrc/main.o 

CPP_SRCS += \
../src/mainsrc/main.cpp 

OBJ_SRCS += \
../src/mainsrc/output.obj 

OBJS += \
./src/mainsrc/main.o 

CPP_DEPS += \
./src/mainsrc/main.d 


# Each subdirectory must supply rules for building sources it contributes
src/mainsrc/%.o: ../src/mainsrc/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


