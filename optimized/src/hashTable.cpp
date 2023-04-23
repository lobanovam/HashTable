#include "./include/hashTable.hpp"
#include "./include/defines.hpp"
#include <immintrin.h>

typedef struct Item {
    struct Node * node;
    size_t peers;
} Item;


static void ClearList(Item* item);
static Node* CreateNode(const char * str);
int avxCmp(const char* str1, const char* str2);

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
    
    assert(hashTable != NULL);
    assert(key       != NULL);

    //printf("\n\n ### in table search\n");
    size_t hashVal = hashTable->hashFunc(key);
    size_t index = hashVal % hashTable->size;

    Node* node =  hashTable->tableItems[index].node;

    while (node) {

        #ifndef _32WORD
            if (!strcmp((const char* )node->string, key))
                break;
        #else
            if (!avxCmp((const char*)node->string, key))
                break;
        #endif 
        node = node->next;
    }

    return node;
}

static Node* CreateNode(const char * str) {

    assert(str != NULL);

    Node* node = (Node*) calloc(1, sizeof(Node));
    assert(node != NULL);

    #ifndef _32WORD
        node->string = (char*) calloc(strlen(str) + 1, sizeof(char));
        assert(node->string != NULL);
        strcpy(node->string, str);
    #else
        node->string = (char*) calloc(32, sizeof(char));
        assert(node->string != NULL);
        memcpy(node->string, str, 32);
    #endif
    
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

void IndexDump(HashTable * hashTable, const char * key) {

    assert(hashTable != NULL);

    size_t index = hashTable->hashFunc(key);

    Item item = hashTable->tableItems[index % hashTable->size];
    Node * node = item.node;

    for (size_t i = 0; i < item.peers; i++) {
        printf(" %s ->", node->string);
        node = node->next;
    }
    printf("\n");
}

int avxCmp(const char* str1, const char* str2) {

    __m256i ar1 = _mm256_loadu_si256 ((__m256i *) str1);
    __m256i ar2 = _mm256_loadu_si256 ((__m256i *) str2);

    __m256i cmp = _mm256_cmpeq_epi8 (ar1, ar2);
    int mask = _mm256_movemask_epi8 (cmp);
    //printf("mask is %x\n", mask);

    if (mask == 0xffffffff) {
        return 0;
    }

    return 1;
}


