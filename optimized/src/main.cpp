#include "./include/hashTable.hpp"
#include "./include/log.hpp"
#include "./include/hashFuncs.hpp"
#include "./include/defines.hpp"
#include <time.h>
#include <stdalign.h>

//sudo perf record -g ./main  --name=test --rw=randread --bs=4k --ioengine=pvsync2  --filename=/dev/nvme0n1 --direct=1 --hipri --filesize=1G
//sudo perf report -g fractal

void SetHashTable(HashTable * hashTable, Text * text);
void TextTo32bit(Text * text);
char* wordTo32bit(char * word);
double TestHashTable(HashTable * hashTable, Text* text, size_t testCt);

//const char* InputFilename = "./textData/inputText.txt";
const char* InputFilename = "./textData/Text.txt";
const char* InputMode = "r";
const char* CsvMode = "w";

const char* CsvFilename  = "./tabulars/rolHash.csv";

int main() {
    log("--------------------START LOGS--------------------\n\n");

    Text text = {};
    FILE* read = NULL;
    read = openFile(InputFilename, InputMode);

    readFile(&text, read);
    fclose(read);
    log("#done readFile()\n\n");

    SplitOnWords(&text);
    log("#done SplitOnWords()\n\n");

    #ifdef _32WORD
        TextTo32bit(&text);
        log("#done wordsTo32bit()\n\n");
    #endif

    HashTable * hashTable = tableCTOR(text.wordsCt/15, rolHash);
    log("#done tableCTOR()\n\n");

    SetHashTable(hashTable, &text);
    log("#done SetHashTable()\n\n");

    Node* node;
    char* key = (char*)calloc(32, sizeof(char));

    //strcpy(key, "dismounting");
    //strcpy(key, "grigoryLeps");
    //strcpy(key, "you");
    
    double meanTime = 0;
//-----------------------------------------------
    meanTime = TestHashTable(hashTable, &text, 10);
    log("---\n ordinary rol time: %f\n---\n", meanTime);
//-----------------------------------------------
    hashTable->hashFunc = rolHashAsmROL;
    meanTime = TestHashTable(hashTable, &text, 10);
    log("---\n with asm rol time: %f\n---\n", meanTime);
//-----------------------------------------------
    hashTable->hashFunc = asmInsertRolHash;
    meanTime = TestHashTable(hashTable, &text, 10);
    log("---\n with asm insertion time: %f\n---\n", meanTime);
//-----------------------------------------------
    hashTable->hashFunc = _RolHash;
    meanTime = TestHashTable(hashTable, &text, 10);
    log("---\n full asm _RolHash time: %f\n---\n", meanTime);
//-----------------------------------------------


    FILE* CsvFile = openFile(CsvFilename, CsvMode);
    TableToCsv(hashTable, CsvFile);
    fclose(CsvFile);
    log("#done TableToCsv()\n\n");

    textDTOR(&text);
    log("#done textDTOR\n\n");

    tableDTOR(hashTable);
    log("#done tableDTOR\n\n");

    log("\n--------------------END LOGS--------------------\n");
    fclose(logFile);

    return 0;
}

void SetHashTable(HashTable * hashTable, Text * text) {

    assert(hashTable != NULL);
    assert(text      != NULL);

    for (size_t i = 0; i < text->wordsCt; i++) {

        char* str = text->words[i];
        //log("will be trying to insert %s\n", str);
        TableInsert(hashTable, str);
    }
}

void TextTo32bit(Text * text) {

    assert(text != NULL);

    size_t wordsCt = text->wordsCt;

    for (size_t i = 0; i < wordsCt; i++) {
        text->words[i] = wordTo32bit(text->words[i]);  
    }
}

char* wordTo32bit(char * word) {

    assert(word != NULL);

    alignas (32) char* _32word = (char*) calloc(32, sizeof(char));
    assert(_32word != NULL);

    int len = strlen(word);
    memcpy(_32word, word, len);

    return _32word;
}

double TestHashTable(HashTable * hashTable, Text* text, size_t testCt) {

    assert(hashTable != NULL);
    assert(text      != NULL);

    Node* node = NULL;

    clock_t meanTime = 0;
    char** wordsArr = text->words;
    size_t wordsCt = text->wordsCt;

    for (size_t count = 0; count < testCt; count++) {

        clock_t start = clock();
        for (size_t i = 0; i < 100000000; i++) {
            node = TableSearch(hashTable, wordsArr[i % wordsCt]);
        }
        clock_t end = clock();

        meanTime += (end - start);
    }

    //IndexDump(hashTable, key);

    return (double) (meanTime) / (double) (testCt * CLOCKS_PER_SEC);
}
