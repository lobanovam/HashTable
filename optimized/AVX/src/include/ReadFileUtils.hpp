#ifndef FILE_U_H
#define FILE_U_H

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>
#include <immintrin.h>

typedef struct Text {

    size_t buffSize;
    char * buffer;
    size_t wordsCt;

    char    **    words;
    __m256i ** _256words;

} Text;



void readFile(Text* text, FILE* read);
FILE* openFile(const char* filename, const char* mode);
void SplitOnWords(Text * text);
void Set256Word(Text * text);
void wordsResize(Text * text, size_t size);
void textDTOR(Text * text);


#endif