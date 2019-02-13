OBJS = Net.cpp Session.cpp main.cpp
CC = g++
COMPILER_FLAGS = -w -std=c++11
INCLUDE_FLAGS =#-I./Dependencies/SDL2-2.0.4/include -I./Dependencies/SDL2_net-2.0.1/include
LINKER_FLAGS = -lSDL2 -lSDL2_net
OBJ_NAME = server
all : $(OBJ)
	@$(CC) $(OBJS) $(COMPILER_FLAGS) $(INCLUDE_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME) 
