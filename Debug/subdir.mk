################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Edge.cpp \
../Graph.cpp \
../StringUtilities.cpp \
../Vertex.cpp 

OBJS += \
./Edge.o \
./Graph.o \
./StringUtilities.o \
./Vertex.o 

CPP_DEPS += \
./Edge.d \
./Graph.d \
./StringUtilities.d \
./Vertex.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -std=c++0x -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


