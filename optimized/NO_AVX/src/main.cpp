#include "./include/hashTable.hpp"
#include "./include/log.hpp"
#include "./include/hashFuncs.hpp"
#include <time.h>
#include <stdalign.h>

// valgrind --tool=callgrind ./main
// kcachegrind

void SetHashTable(HashTable * hashTable, Text * text);
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

    HashTable * hashTable = tableCTOR(text.wordsCt/15, FAQ6Hash);
    log("#done tableCTOR()\n\n");

    SetHashTable(hashTable, &text);
    log("#done SetHashTable()\n\n");
    double meanTime = 0;
//-----------------------------------------------
    meanTime = TestHashTable(hashTable, &text, 10);
    log("---\n ordinary FAQ6 hash time: %f\n---\n", meanTime);
//-----------------------------------------------
    ReplaceHashFunc(hashTable, _FAQ6);
    meanTime = TestHashTable(hashTable, &text, 10);
    log("---\n full asm FAQ6 time: %f\n---\n", meanTime);
//-----------------------------------------------
//     ReplaceHashFunc(hashTable, asmInsertRolHash);
//     meanTime = TestHashTable(hashTable, &text, 10);
//     log("---\n with asm insertion time: %f\n---\n", meanTime);
// //-----------------------------------------------
//     ReplaceHashFunc(hashTable, _RolHash);
//     meanTime = TestHashTable(hashTable, &text, 10);
//     log("---\n full asm _RolHash time: %f\n---\n", meanTime);
// //-----------------------------------------------


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

        //log("%zu) time is %f\n", count, (double) (end - start) / CLOCKS_PER_SEC);
        meanTime += (end - start);
    }

    return (double) (meanTime) / (double) (testCt * CLOCKS_PER_SEC);
}
