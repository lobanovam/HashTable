#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include <ctype.h>

typedef struct HashTable HashTable;

typedef struct Node {
    char * string; 
    struct Node* next;
} Node;


HashTable * tableCTOR(size_t size, size_t (*hashFunc)(const char * word));
void tableDTOR(HashTable * hashTable);
void TableInsert(HashTable * hashTable, const char * str);
Node* TableSearch(HashTable * hashTable, const char * key);
void TableToCsv(HashTable * hashTable, FILE * CsvFile);
void IndexDump(HashTable * hashTable, const char * key);
void ReplaceHashFunc(HashTable * hashTable, size_t (*hashFunc)(const char * word));




#endif 