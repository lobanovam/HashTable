#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include <ctype.h>

typedef struct HashTable {
    struct Item * tableItems;
    size_t size;
} HashTable;

typedef struct Item {
    struct Node * node;
    size_t peers;
} Item;

typedef struct Node {
    char * string; 
    struct Node* next;
} Node;


//________________________________HashFuncs()
size_t OneHash(const char * word);
size_t AsciiHash(const char * word);
size_t StrlenHash(const char * word);
size_t AsciiSumHash(const char * word);
size_t rolHash(const char * word);
size_t rorHash(const char * word);

size_t RorFunc (size_t num, int shift);
size_t RolFunc (size_t num, int shift);
//___________________________________________

#endif 