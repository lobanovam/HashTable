CC = g++
IFLAGS = -I./src/include/
FLAGS = -fno-omit-frame-pointer -no-pie -std=c++14 -mavx2 -mavx

SRC = ./src/main.cpp ./src/hashTable.cpp ./src/hashFuncs.cpp ./src/ReadFileUtils.cpp ./src/RolFunc.asm

OBJ_TEMP  = $(subst ./src/, ./src/build/, $(SRC))
OBJ_TEMP1 = $(subst .cpp,.o, $(OBJ_TEMP))
OBJ       = $(subst .asm,.o, $(OBJ_TEMP1))

all: $(OBJ)
	$(CC) $(FLAGS) $(IFLAGS) $^ -o main        # $^ - всё что справа от all:

./src/build/%.o: ./src/%.cpp
	mkdir -p ./src/build
	$(CC) $(FLAGS) $(IFLAGS) -c  $^ -o $@	   # $@ - всё что слева 

./src/build/RolFunc.o: ./src/RolFunc.asm
	nasm -f elf64 -o  $@  $^

clear: 
	rm -f ./src/build/*.o main
	
CSVclear:
	rm -f *.csv