#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include <ctype.h>
#include <immintrin.h>

typedef struct HashTable HashTable;

typedef struct Node {
    __m256i* str; 
    struct Node* next;
} Node;


HashTable * tableCTOR(size_t size, size_t (*hashFunc)(const char * word));
void tableDTOR(HashTable * hashTable);
void TableInsert(HashTable * hashTable, __m256i* key);
Node* TableSearch(HashTable * hashTable, __m256i* key);
void TableToCsv(HashTable * hashTable, FILE * CsvFile);
void IndexDump(HashTable * hashTable, const char * key);
void ReplaceHashFunc(HashTable * hashTable, size_t (*hashFunc)(const char * word));




#endif 