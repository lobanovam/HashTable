#include "./include/hashTable.hpp"
#include <immintrin.h>
#include <time.h>

typedef struct Item {
    struct Node * node;
    size_t peers;
} Item;

typedef struct HashTable {
    struct Item * tableItems;
    size_t (*hashFunc)(const char * word);
    size_t size;
} HashTable;

static void ClearList(Item* item);
static Node* CreateNode(const char * str);

HashTable * tableCTOR(size_t size, size_t (*hashFunc)(const char * word)) {

    assert(hashFunc != NULL);

    HashTable * hashTable = (HashTable*)(calloc(1, sizeof(HashTable)));
    assert(hashTable != NULL);

    hashTable->tableItems = (Item*) calloc(size, sizeof(Item));
    assert(hashTable->tableItems != NULL);
    hashTable->size = size;

    hashTable->hashFunc = hashFunc;

    return hashTable;
}

void TableInsert(HashTable * hashTable, const char * word) {

    assert(hashTable != NULL);
    assert(word      != NULL);

    size_t index = hashTable->hashFunc(word) % hashTable->size;

    // printf("\n----------------------");
    // printf("%s value: %zu\n", (char*)word,hashTable->hashFunc((char*) word));

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
    if (strcmp(prevNode->string, word)) {
        prevNode->next = CreateNode(word);
        hashTable->tableItems[index].peers++;
    }
}

Node* TableSearch(HashTable * hashTable, const char* key) {

    // clock_t meanTime = 0;

    // for (size_t count = 0; count < 5; count++) {

    //     clock_t start = clock();
    //     for (size_t i = 0; i < 1000000000; i++) {
    //         size_t index = hashTable->hashFunc(key) % hashTable->size;  
    //     }
    //     clock_t end = clock();
    //     meanTime += (end - start);
    //     printf("%zu), time is %f\n", count+1, (double) (end - start) / CLOCKS_PER_SEC);
    // }

    // printf("mean time: %f\n", (double) (meanTime) / (5 * CLOCKS_PER_SEC));

  
    size_t index = hashTable->hashFunc(key) % hashTable->size;

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
    assert(node != NULL);

    int len = strlen(str);

    node->string = (char*) calloc(len + 1, sizeof(char));
    assert(node->string != NULL);
    strcpy(node->string, str);

    //printf("created node: %s\n", node->string);
    
    node->next = NULL;

    return node;
}

void tableDTOR(HashTable * hashTable) {

    assert(hashTable != NULL);

    for (size_t i = 0; i < hashTable->size; i++) {
        ClearList(&hashTable->tableItems[i]);
    }

    free(hashTable->tableItems);

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

void IndexDump(HashTable * hashTable, const char * key) {

    assert(hashTable != NULL);

    size_t index = hashTable->hashFunc(key);
    printf("hash val is %zu\n", index);

    Item item = hashTable->tableItems[index % hashTable->size];
    Node * node = item.node;

    for (size_t i = 0; i < item.peers; i++) {
        printf(" %s ->", node->string);
        node = node->next;
    }
    printf("\n");
}

void ReplaceHashFunc(HashTable * hashTable, size_t (*hashFunc)(const char * word)) {

    assert(hashTable != NULL);
    assert(hashFunc != NULL);

    hashTable->hashFunc = hashFunc;
}




