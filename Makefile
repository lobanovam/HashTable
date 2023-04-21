CC = g++
IFLAGS = -I./src/include/
FLAGS = -fno-omit-frame-pointer

SRC = ./src/main.cpp ./src/hashTable.cpp ./src/hashFuncs.cpp ./src/ReadFileUtils.cpp

OBJ_TEMP = $(subst ./src/, ./src/build/, $(SRC))
OBJ      = $(subst .cpp,.o, $(OBJ_TEMP))

all: $(OBJ)
	$(CC) $(FLAGS) $(IFLAGS) $^ -o main        # $^ - всё что справа от all:

./src/build/%.o: ./src/%.cpp
	mkdir -p ./src/build
	$(CC) $(FLAGS) $(IFLAGS) -c  $^ -o $@	   # $@ - всё что слева 

clear: 
	rm -f ./src/build/*.o main
	
CSVclear:
	rm -f *.csv