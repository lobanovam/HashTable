#include "hashTable.hpp"

size_t OneHash(const char * word) {
    return 1;
}

size_t AsciiHash(const char * word) {
    return *word;
}

size_t StrlenHash(const char * word) {
    return strlen(word);
}

size_t AsciiSumHash(const char * word) {
    size_t sum = 0;
    while (*word) {
        sum += *word;
        word++;
    }
    return sum;
}

size_t rolHash(const char * word) {
    size_t hash = 0, n = strlen(word);

    for (size_t i = 0; i < n; i++) {
        hash = RolFunc(hash, 1) ^ word[i]; 
    }

    return hash;
}

size_t rorHash(const char * word) {
    size_t hash = 0, n = strlen(word);

    for (size_t i = 0; i < n; i++) {
        hash = RorFunc(hash, 1) ^ word[i]; 
    }

    return hash;
}

size_t RorFunc (size_t num, int shift) {                                 // 011000001 ---> 10110000 
    return (num >> shift) | (num << (sizeof (uint32_t) - shift));
}

size_t RolFunc (size_t num, int shift) {                                 // 10011000 ----> 0011001
    return (num << shift) | (num >> (sizeof (uint32_t) - shift));
}

