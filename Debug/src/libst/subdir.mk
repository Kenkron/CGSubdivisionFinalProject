################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
O_SRCS += \
../src/libst/STColor3f.o \
../src/libst/STColor4f.o \
../src/libst/STColor4ub.o \
../src/libst/STFont.o \
../src/libst/STImage.o \
../src/libst/STImage_jpeg.o \
../src/libst/STImage_png.o \
../src/libst/STImage_ppm.o \
../src/libst/STJoystick.o \
../src/libst/STJoystick_linux.o \
../src/libst/STMatrix4.o \
../src/libst/STPoint2.o \
../src/libst/STPoint3.o \
../src/libst/STShaderProgram.o \
../src/libst/STShape.o \
../src/libst/STTexture.o \
../src/libst/STTimer.o \
../src/libst/STTriangleMesh.o \
../src/libst/STVector2.o \
../src/libst/STVector3.o \
../src/libst/tiny_obj_loader.o 

CPP_SRCS += \
../src/libst/STColor3f.cpp \
../src/libst/STColor4f.cpp \
../src/libst/STColor4ub.cpp \
../src/libst/STFont.cpp \
../src/libst/STImage.cpp \
../src/libst/STImage_jpeg.cpp \
../src/libst/STImage_png.cpp \
../src/libst/STImage_ppm.cpp \
../src/libst/STJoystick.cpp \
../src/libst/STJoystick_darwin.cpp \
../src/libst/STJoystick_linux.cpp \
../src/libst/STJoystick_win32.cpp \
../src/libst/STMatrix4.cpp \
../src/libst/STPoint2.cpp \
../src/libst/STPoint3.cpp \
../src/libst/STShaderProgram.cpp \
../src/libst/STShape.cpp \
../src/libst/STTexture.cpp \
../src/libst/STTimer.cpp \
../src/libst/STTriangleMesh.cpp \
../src/libst/STVector2.cpp \
../src/libst/STVector3.cpp \
../src/libst/tiny_obj_loader.cpp 

OBJS += \
./src/libst/STColor3f.o \
./src/libst/STColor4f.o \
./src/libst/STColor4ub.o \
./src/libst/STFont.o \
./src/libst/STImage.o \
./src/libst/STImage_jpeg.o \
./src/libst/STImage_png.o \
./src/libst/STImage_ppm.o \
./src/libst/STJoystick.o \
./src/libst/STJoystick_darwin.o \
./src/libst/STJoystick_linux.o \
./src/libst/STJoystick_win32.o \
./src/libst/STMatrix4.o \
./src/libst/STPoint2.o \
./src/libst/STPoint3.o \
./src/libst/STShaderProgram.o \
./src/libst/STShape.o \
./src/libst/STTexture.o \
./src/libst/STTimer.o \
./src/libst/STTriangleMesh.o \
./src/libst/STVector2.o \
./src/libst/STVector3.o \
./src/libst/tiny_obj_loader.o 

CPP_DEPS += \
./src/libst/STColor3f.d \
./src/libst/STColor4f.d \
./src/libst/STColor4ub.d \
./src/libst/STFont.d \
./src/libst/STImage.d \
./src/libst/STImage_jpeg.d \
./src/libst/STImage_png.d \
./src/libst/STImage_ppm.d \
./src/libst/STJoystick.d \
./src/libst/STJoystick_darwin.d \
./src/libst/STJoystick_linux.d \
./src/libst/STJoystick_win32.d \
./src/libst/STMatrix4.d \
./src/libst/STPoint2.d \
./src/libst/STPoint3.d \
./src/libst/STShaderProgram.d \
./src/libst/STShape.d \
./src/libst/STTexture.d \
./src/libst/STTimer.d \
./src/libst/STTriangleMesh.d \
./src/libst/STVector2.d \
./src/libst/STVector3.d \
./src/libst/tiny_obj_loader.d 


# Each subdirectory must supply rules for building sources it contributes
src/libst/%.o: ../src/libst/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


