################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Navio/ADS1115.cpp \
../src/Navio/I2Cdev.cpp \
../src/Navio/MB85RC04.cpp \
../src/Navio/MB85RC256.cpp \
../src/Navio/MPU9250.cpp \
../src/Navio/MS5611.cpp \
../src/Navio/PCA9685.cpp \
../src/Navio/Ublox.cpp \
../src/Navio/Util.cpp \
../src/Navio/gpio.cpp 

OBJS += \
./src/Navio/ADS1115.o \
./src/Navio/I2Cdev.o \
./src/Navio/MB85RC04.o \
./src/Navio/MB85RC256.o \
./src/Navio/MPU9250.o \
./src/Navio/MS5611.o \
./src/Navio/PCA9685.o \
./src/Navio/Ublox.o \
./src/Navio/Util.o \
./src/Navio/gpio.o 

CPP_DEPS += \
./src/Navio/ADS1115.d \
./src/Navio/I2Cdev.d \
./src/Navio/MB85RC04.d \
./src/Navio/MB85RC256.d \
./src/Navio/MPU9250.d \
./src/Navio/MS5611.d \
./src/Navio/PCA9685.d \
./src/Navio/Ublox.d \
./src/Navio/Util.d \
./src/Navio/gpio.d 


# Each subdirectory must supply rules for building sources it contributes
src/Navio/%.o: ../src/Navio/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++11 -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


