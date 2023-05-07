#include "ReadFileUtils.hpp"

const size_t defaultWordCt = 100; 

void readFile(Text* text, FILE* read) {

    assert(read != NULL);

    fseek (read, 0L, SEEK_END);
    int fileLen = ftell (read);
    fseek (read, 0L, SEEK_SET);

    text->buffer = (char*) (calloc(fileLen, sizeof(char)));

    text->buffSize = fileLen;
    fread(text->buffer, sizeof(char), fileLen, read);

    text->wordsCt = defaultWordCt;
    text->words = (char**) calloc(defaultWordCt, sizeof(char*));
    assert(text->words != NULL);

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
        //printf("got %s\n", text->words[wordsCt]);

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
    //printf("resize completed\n");
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

void Set256Word (Text * text) {

    assert(text != NULL);

    size_t wordsCt = text->wordsCt;

    text->_256words = (__m256i**) calloc(wordsCt, sizeof(__m256i*));
    assert(text->_256words != NULL);

    for (size_t i = 0; i < wordsCt; i++) {

        __m256i* _256word = (__m256i*) aligned_alloc(32, sizeof(__m256i));
        assert(_256word != NULL);
        memset(_256word, 0, sizeof(__m256i));

        //printf("alive here\n");
        alignas(32) char tmp_word[32] = {0};
        assert(text->words[i] != NULL);
        strcpy(tmp_word, text->words[i]);
        
        *_256word = _mm256_load_si256 ((__m256i*) tmp_word);

        text->_256words[i] = _256word;

        //printf("%s to 256: %s\n", text->words[i], (char*)text->_256words[i]);
    }
}
