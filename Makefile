OBJS = MemoryManager.o 
CC = g++
DEBUG = -g
CFLAGS = -Wall -Wextra -Werror -c $(DEBUG)
LFlags = -Wall $(DEBUG)

MemoryManager.o : MemoryManager.h MemoryManager.cpp main.cpp	
	$(CC) $(CFLAGS) MemoryManager.cpp main.cpp