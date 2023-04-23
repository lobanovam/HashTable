#include "./include/hashTable.hpp"
#include "./include/log.hpp"
#include "./include/hashFuncs.hpp"
#include "./include/defines.hpp"
#include <time.h>

//sudo perf record -g ./main  --name=test --rw=randread --bs=4k --ioengine=pvsync2  --filename=/dev/nvme0n1 --direct=1 --hipri --filesize=1G
//sudo perf report -g fractal

void SetHashTable(HashTable * hashTable, Text * text);
void wordsTo32bit(Text * text);

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
        wordsTo32bit(&text);
        log("#done wordsTo32bit()\n\n");
    #endif

    HashTable * hashTable = tableCTOR(text.wordsCt/15, rolHash);
    log("#done tableCTOR()\n\n");

    SetHashTable(hashTable, &text);
    log("#done SetHashTable()\n\n");

    Node* node;
    char key[] = "sasha";
    //char key[] = "dismounting";
//-----------------------------------------------
    clock_t start = clock();
    for (size_t i = 0; i < 100000000; i++) {
        node = TableSearch(hashTable, key);
    }
    clock_t end = clock();
//  IndexDump(hashTable, key);
    log("-----\n unoptimized search is %f\n-----\n\n", (double) (end - start) / CLOCKS_PER_SEC);
//-----------------------------------------------
    hashTable->hashFunc = asmInsertRolHash;
    start = clock();
    for (size_t i = 0; i < 100000000; i++) {
        node = TableSearch(hashTable, key);
    }
    end = clock();
//  IndexDump(hashTable, key);
    log("-----\n asm insertion search is %f\n-----\n\n", (double) (end - start) / CLOCKS_PER_SEC);
//-----------------------------------------------
    hashTable->hashFunc = rolHashAsmROL;
    start = clock();
    for (size_t i = 0; i < 100000000; i++) {
        node = TableSearch(hashTable, key);
    }
    end = clock();
//  IndexDump(hashTable, key);
    log("-----\n asm rol search is %f\n-----\n\n", (double) (end - start) / CLOCKS_PER_SEC);
//-----------------------------------------------

    if (node) {
        log("found str\n");
    }
    else {
        log("not found\n");
    }
    
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

void wordsTo32bit(Text * text) {
    assert(text != NULL);

    size_t wordsCt = text->wordsCt;

    for (size_t i = 0; i < wordsCt; i++) {

        char* _32word = (char*) calloc(32, sizeof(char));
        assert(_32word != NULL);
        
        char* str = text->words[i];
        int len = strlen(str);
        memcpy(_32word, str, len);

        text->words[i] = _32word;   
    }

    // for (size_t i = 0; i < wordsCt; i++) {
    //     printf("words[%zu] is %s\n", i, text->words[i]);
    // }

}

