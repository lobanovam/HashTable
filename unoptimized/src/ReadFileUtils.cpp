#include "ReadFileUtils.hpp"

const size_t defaultWordCt = 100; 

void readFile(Text* text, FILE* read) {

    assert(read != NULL);

    fseek (read, 0L, SEEK_END);
    int fileLen = ftell (read);
    fseek (read, 0L, SEEK_SET);

    text->buffer = (char*) (calloc(fileLen, sizeof(char)));
    assert(text->buffer != NULL);

    text->buffSize = fileLen;
    fread(text->buffer, sizeof(char), fileLen, read);

    text->wordsCt = defaultWordCt;
    text->words = (char**) calloc(defaultWordCt, sizeof(char*));
}

FILE * openFile(const char* filename, const char* mode) {

    FILE* file = fopen(filename, mode);

    if (!file) {
        printf("Error, can't open file\n");
        return NULL;
    }

    return file;
}

void SplitOnWords(Text * text) {

    assert(text != NULL);
    assert(text->buffer != NULL);

    size_t wordsCt = 0;

    for (size_t i = 0; i < text->buffSize; i++) {

        if (!isalpha(text->buffer[i]))
            continue;
        
        text->words[wordsCt] = text->buffer + i;
        int len = 0;

        while (isalpha(*(text->buffer + i))) {
            len++;
            i++;
        }
        *(text->buffer + i) = '\0';

        wordsCt++;

        if (text->wordsCt == wordsCt) {
            //printf("need resize from %zu to %zu\n", wordsCt, wordsCt*2);
            wordsResize(text, wordsCt * 2);
        }     
    }

    wordsResize(text, wordsCt);
    printf("last resize to %zu comleted\n", wordsCt);
    
}

void wordsResize(Text * text, size_t size) {

    assert(text != NULL);
    text->words = (char**) realloc(text->words, size * sizeof(char*));
    assert(text->words != NULL);
    
    if (!text->words) {
        printf("ERROR: realloc error in wordsResize\n");
    }
    text->wordsCt = size;
}

void textDTOR(Text * text) {

    assert(text != NULL);
    assert(text->buffer != NULL);
    assert(text->words != NULL);

    free(text->words);
    free(text->buffer);

    text->buffSize = 0;
    text->wordsCt = 0;
    text->buffer = NULL;
    text->words = NULL;
}
