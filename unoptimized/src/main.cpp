#include "./include/hashTable.hpp"
#include "./include/log.hpp"
#include "./include/hashFuncs.hpp"


void SetHashTable(HashTable * hashTable, Text * text);

//const char* InputFilename = "./textData/inputText.txt";
const char* InputFilename = "./textData/Text.txt";
const char* InputMode = "rb";
const char* CsvMode = "w";

enum HashFuncsEnum {
    ONE_HASH,
    ASCII_HASH,
    STRLEN_HASH,
    ASCIISUM_HASH,
    ROL_HASH,
    ROR_HASH,
    FAQ6_HASH
};

const char* CsvFilenames[] = {
    "./tabulars/oneHash.csv",
    "./tabulars/asciiHash.csv",
    "./tabulars/strlenHash.csv",
    "./tabulars/asciiSumHash.csv",
    "./tabulars/rolHash.csv",
    "./tabulars/rorHash.csv",
    "./tabulars/FAQ6Hash.csv",
};

size_t (*HashFuncsArr[])(const char * word) = {
    OneHash,
    AsciiHash,
    StrlenHash,
    AsciiSumHash,
    rolHash,
    rorHash,
    FAQ6Hash
};

const int HASH_FUNC = ROL_HASH;

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

    HashTable * hashTable = tableCTOR(text.wordsCt / 15, HashFuncsArr[HASH_FUNC]);
    log("#done tableCTOR()\n\n");

    SetHashTable(hashTable, &text);
    log("#done SetHashTable()\n\n");

    
    Node* node = TableSearch(hashTable, "sasha");

    // if (node) {
    //     //log("found str is %s\n", node->string);
    // }
    // else 
    //     log("not found\n");
    
    FILE* CsvFile = openFile(CsvFilenames[HASH_FUNC], CsvMode);
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

