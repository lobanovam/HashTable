#include "hashTable.hpp"
#include "log.hpp"

void tableCTOR(HashTable * hashTable, size_t size);
void tableDTOR(HashTable * hashTable);
void SetHashTable(HashTable * hashTable, Text * text, size_t (*hashFunc)(const char * word));
Node* CreateNode(const char * str);
void TableInsert(HashTable * hashTable, char * str, int index);
Node* TableSearch(HashTable * hashTable, char* key, size_t (*hashFunc)(const char * word));
void TableToCsv(HashTable * hashTable, FILE * CsvFile);
void ClearList(Item* item);

//const char* InputFilename = "./data/inputText.txt";
const char* InputFilename = "./data/Text.txt";
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

const int HASH_FUNC = FAQ6_HASH;

int main() {
    log("--------------------START LOGS--------------------\n\n");

    Text text = {};
    FILE* read = NULL;
    read = openFile(InputFilename, InputMode);

    readFile(&text, read);
    log("#done readFile()\n\n");

    SplitOnWords(&text);
    log("#done SplitOnWords()\n\n");

    HashTable hashTable = {};
    tableCTOR(&hashTable, text.wordsCt/15 );
    log("#done tableCTOR()\n\n");

    
    size_t (*hashFunc)(const char* word) = HashFuncsArr[HASH_FUNC];

    SetHashTable(&hashTable, &text, hashFunc);
    log("#done SetHashTable()\n\n");

    Node* node = TableSearch(&hashTable, "misha", hashFunc);
    log("#done TableSearch()\n\n");

    // if (node) {
    //     //log("found str is %s\n", node->string);
    // }
    // else 
    //     log("not found\n");
    
    FILE* CsvFile = openFile(CsvFilenames[HASH_FUNC], CsvMode);
    TableToCsv(&hashTable, CsvFile);
    log("#done TableToCsv()\n\n");


    textDTOR(&text);
    log("#done textDTOR\n\n");

    tableDTOR(&hashTable);
    log("#done tableDTOR\n\n");

    log("\n--------------------END LOGS--------------------\n");

    return 0;
}


void tableCTOR(HashTable * hashTable, size_t size) {

    assert(hashTable != NULL);

    hashTable->tableItems = (Item*) calloc(size, sizeof(Item));
    hashTable->size = size;

    for (int i = 0; i < size; i++) {
        hashTable->tableItems[i].node = NULL;
        hashTable->tableItems[i].peers = 0;
    }
}

void SetHashTable(HashTable * hashTable, Text * text, size_t (*hashFunc)(const char * word)) {

    assert(hashTable != NULL);
    assert(text      != NULL);
    assert(hashFunc  != NULL);

    for (size_t i = 0; i < text->wordsCt; i++) {

        char* str = text->words[i];
        size_t hashVal = hashFunc(str);
        size_t index = hashVal % hashTable->size;
        TableInsert(hashTable, str, index);
    }
}

Node* CreateNode(const char * str) {

    assert(str != NULL);

    Node* node = (Node*) calloc(1, sizeof(Node));
    node->string = (char*) str;
    node->next = NULL;

    return node;
}

void TableInsert(HashTable * hashTable, char * str, int index) {

    assert(hashTable != NULL);
    assert(str       != NULL);

    Node* node = CreateNode(str);
    Node* prevNode = hashTable->tableItems[index].node;

    
    if (prevNode == NULL) {

        hashTable->tableItems[index].peers++;
        hashTable->tableItems[index].node = node;
        return;
    }

    while (prevNode->next != NULL) {
        if (!strcmp(prevNode->string, str)) {
            //log("------in TableInsert got repeat word %s\n\n", str);
            return;
        }
        prevNode = prevNode->next;
    }
    prevNode->next = node;
    hashTable->tableItems[index].peers++;
}

Node* TableSearch(HashTable * hashTable, char* key, size_t (*hashFunc)(const char * word)) {
    
    assert(hashTable != NULL);
    assert(key       != NULL);
    assert(hashFunc  != NULL);

    size_t hashVal = hashFunc(key);
    size_t index = hashVal % hashTable->size;

    if (hashTable->tableItems[index].peers == 1) {
        return hashTable->tableItems[index].node;
    }
    Node* node =  hashTable->tableItems[index].node;

    while (node) {
        char* str = node->string;
        if (!strcmp(str, key))
            break;
        node = node->next;
    }

    return node;
}

void TableToCsv(HashTable * hashTable, FILE * CsvFile) {

    assert(hashTable != NULL);
    assert(CsvFile   != NULL);

    size_t len = hashTable->size;
    for (size_t i = 0; i < len; i++) {
        fprintf(CsvFile, "%zu, %zu\n", i, hashTable->tableItems[i].peers);
    }
}

void tableDTOR(HashTable * hashTable) {
    assert(hashTable != NULL);

    for (size_t i = 0; i < hashTable->size; i++) {
        ClearList(&hashTable->tableItems[i]);
        hashTable->tableItems->peers = 0;
    }

    free(hashTable->tableItems);
    hashTable->size = 0;

}

void ClearList(Item* item) {
    assert(item != NULL);

    Node* node = item->node;
    Node* nextNode = NULL;
    if (!node) 
        return;
    
    while (node->next) {
        nextNode = node->next;
        free(node);
        node = nextNode;
    }
    free(node);
}



