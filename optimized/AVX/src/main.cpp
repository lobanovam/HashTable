#include <time.h>
#include <stdalign.h>

#include "./include/hashTable.hpp"
#include "./include/log.hpp"
#include "./include/hashFuncs.hpp"

void SetHashTable(HashTable * hashTable, Text * text);
double TestHashTable(HashTable * hashTable, Text* text, size_t testCt);

//const char* InputFilename = "./textData/inputText.txt";
const char* InputFilename = "./textData/Text.txt";
const char* InputMode = "r";
const char* CsvMode = "w";

const char* CsvFilename  = "./tabulars/CRC32Hash.csv";

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

    Set256Word(&text);
    log("#done Set256Word()\n\n");

    HashTable * hashTable = tableCTOR(text.wordsCt / 10 , _crc32);
    log("#done tableCTOR()\n\n");

    SetHashTable(hashTable, &text);
    log("#done SetHashTable()\n\n");

    double meanTime = 0;
//-----------------------------------------------
    meanTime = TestHashTable(hashTable, &text, 10);
    log("---\n ordinary ROL hash time: %f\n---\n", meanTime);
// //-----------------------------------------------
//     ReplaceHashFunc(hashTable, rolHashAsmROL);
//     meanTime = TestHashTable(hashTable, &text, 10);
//     log("---\n rolHashAsmROL time: %f\n---\n", meanTime);
// //-----------------------------------------------
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

        __m256i* str = text->_256words[i];
        //log("will be trying to insert %s\n", (char*)str);
        TableInsert(hashTable, str);
    }
}

double TestHashTable(HashTable * hashTable, Text* text, size_t testCt) {

    assert(hashTable != NULL);
    assert(text      != NULL);

    volatile Node* node = NULL;

    clock_t meanTime = 0;
    __m256i** _256wordsArr = text->_256words;
    size_t wordsCt = text->wordsCt;

    for (size_t count = 0; count < testCt; count++) {

        clock_t start = clock();
        for (size_t i = 0; i < 100000000; i++) {
            node = TableSearch(hashTable, _256wordsArr[i % wordsCt]);
        }
        clock_t end = clock();

        //log("%zu), time is %f\n", count, (double) (end - start) / CLOCKS_PER_SEC);
        meanTime += (end - start);
    }

    return (double) (meanTime) / (double) (testCt * CLOCKS_PER_SEC);
}
