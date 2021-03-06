################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/asynctask/abstract_asynctask.cpp \
../src/asynctask/abstract_asynctask_search_condition.cpp \
../src/asynctask/abstract_asynctask_thread_pool.cpp \
../src/asynctask/asynctask_default_thread_pool.cpp \
../src/asynctask/asynctask_queue.cpp 

OBJS += \
./src/asynctask/abstract_asynctask.o \
./src/asynctask/abstract_asynctask_search_condition.o \
./src/asynctask/abstract_asynctask_thread_pool.o \
./src/asynctask/asynctask_default_thread_pool.o \
./src/asynctask/asynctask_queue.o 

CPP_DEPS += \
./src/asynctask/abstract_asynctask.d \
./src/asynctask/abstract_asynctask_search_condition.d \
./src/asynctask/abstract_asynctask_thread_pool.d \
./src/asynctask/asynctask_default_thread_pool.d \
./src/asynctask/asynctask_queue.d 


# Each subdirectory must supply rules for building sources it contributes
src/asynctask/%.o: ../src/asynctask/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -D__BCRYPT=1 -DSPROTO_WITHOUT_OUT_BUFFER -DSRPC_WITHOUT_OUT_QUEUE -DSERVER_VERSION_23 -DUSE_DEPRECATED_EMEV_V1 -D__OPENSSL_TOOLS=1 -I$(INCMYSQL) -I../src/mqtt -I../src/client -I../src/user -I../src/device -I../src -I$(SSLDIR)/include -O3 -Wall -fsigned-char -c -fmessage-length=0 -fstack-protector-all -std=c++11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


