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
int ListPushBack(Node* head, const char* key);

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

    int inserted = ListPushBack(prevNode, word);

    if (inserted) 
        hashTable->tableItems[index].peers++;

}

int ListPushBack(Node* head, const char* key) {

    assert(head != NULL);
    assert(key != NULL);

    while (head->next != NULL) {

        assert(head->string != NULL);
        if (!strcmp(head->string, key)) {
            return 0;
        }
        head = head->next;
    }

    if (strcmp(head->string, key)) {
        head->next = CreateNode(key);

        return 1;
    }

    return 0;
}

Node* TableSearch(HashTable * hashTable, const char* key) {


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




