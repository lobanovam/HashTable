#include "hashTable.hpp"

typedef struct HashTable {
    struct Item * tableItems;
    size_t (*hashFunc)(const char * word);
    size_t size;
} HashTable;

typedef struct Item {
    struct Node * node;
    size_t peers;
} Item;

typedef struct Node {
    char * string; 
    struct Node* next;
} Node;

static void ClearList(Item* item);
static Node* CreateNode(const char * str);

HashTable * tableCTOR(size_t size, size_t (*hashFunc)(const char * word)) {

    assert(hashFunc != NULL);

    HashTable * hashTable = (HashTable*)(calloc(1, sizeof(HashTable)));
    assert(hashTable != NULL);

    hashTable->tableItems = (Item*) calloc(size, sizeof(Item));
    hashTable->size = size;

    hashTable->hashFunc = hashFunc;

    return hashTable;
}

void TableInsert(HashTable * hashTable, const char * word) {

    assert(hashTable != NULL);
    assert(word      != NULL);

    int index = hashTable->hashFunc(word) % hashTable->size;

    Node* prevNode = hashTable->tableItems[index].node;
    
    if (prevNode == NULL) {

        hashTable->tableItems[index].peers++;
        hashTable->tableItems[index].node = CreateNode(word);
        return;
    }

    while (prevNode->next != NULL) {

        assert(prevNode->string != NULL);
        if (!strcmp(prevNode->string, word)) {
            return;
        }
        prevNode = prevNode->next;
    }
    prevNode->next = CreateNode(word);
    hashTable->tableItems[index].peers++;
}

Node* TableSearch(HashTable * hashTable, const char* key) {
    
    assert(hashTable != NULL);
    assert(key       != NULL);

    size_t hashVal = hashTable->hashFunc(key);
    size_t index = hashVal % hashTable->size;

    if (hashTable->tableItems[index].peers == 1) {
        return hashTable->tableItems[index].node;
    }
    Node* node =  hashTable->tableItems[index].node;

    while (node) {
        
        if (!strcmp((const char* )node->string, key))
            break;
        node = node->next;
    }

    return node;
}

static Node* CreateNode(const char * str) {

    assert(str != NULL);

    Node* node = (Node*) calloc(1, sizeof(Node));
    node->string = (char*) calloc(strlen(str) + 1, sizeof(char));
    strcpy(node->string, str);
    //printf("created node with str %s\n", node->string);
    node->next = NULL;

    return node;
}

void tableDTOR(HashTable * hashTable) {
    assert(hashTable != NULL);

    for (size_t i = 0; i < hashTable->size; i++) {
        ClearList(&hashTable->tableItems[i]);
        hashTable->tableItems[i].peers = 0;
    }

    free(hashTable->tableItems);
    hashTable->size = 0;

    free(hashTable);
}

static void ClearList(Item* item) {
    assert(item != NULL);

    Node* node = item->node;
    Node* nextNode = NULL;
    if (!node) {
        return;
    }
    
    while (node->next) {
        nextNode = node->next;
        free(node->string);
        free(node);
        node = nextNode;
    }
    free(node->string);
    free(node);
}

void TableToCsv(HashTable * hashTable, FILE * CsvFile) {

    assert(hashTable != NULL);
    assert(CsvFile   != NULL);

    size_t len = hashTable->size;
    for (size_t i = 0; i < len; i++) {
        fprintf(CsvFile, "%zu, %zu\n", i, hashTable->tableItems[i].peers);
    }
}


