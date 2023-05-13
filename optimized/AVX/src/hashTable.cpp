#include "./include/hashTable.hpp"
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
static Node* CreateNode(__m256i* str);
int avxCmp(__m256i* str1, __m256i* str2);
int ListPushBack(Node* head, __m256i* key);

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

void TableInsert(HashTable * hashTable, __m256i* key) {

    assert(hashTable != NULL);
    assert(key       != NULL);

    size_t index = hashTable->hashFunc((const char*) key) % hashTable->size;

    Node* prevNode = hashTable->tableItems[index].node;
    
    if (prevNode == NULL) {

        hashTable->tableItems[index].peers++;
        hashTable->tableItems[index].node = CreateNode(key);
        return;
    }

    int inserted = ListPushBack(prevNode, key);

    if (inserted) 
        hashTable->tableItems[index].peers++;

}

int ListPushBack(Node* head, __m256i* key) {

    assert(head != NULL);
    assert(key != NULL);

    while (head->next != NULL) {

        assert(head->str != NULL);
        if (!avxCmp(head->str, key)) {
            return 0;
        }
        head = head->next;
    }

    if (avxCmp(head->str, key)) {
        head->next = CreateNode(key);

        return 1;
    }

    return 0;
}

Node* TableSearch(HashTable * hashTable, __m256i* key) {

    size_t index = hashTable->hashFunc((const char*)key) % hashTable->size;

    Node* node =  hashTable->tableItems[index].node;

    while (node) {

        if (!avxCmp(node->str, key)) {
            //printf("found %s\n", (char*)node->str);
            break;
        }

        node = node->next;
    }

    return node;
}

static Node* CreateNode(__m256i* _256word) {

    assert(_256word != NULL);

    Node* node = (Node*) calloc(1, sizeof(Node));
    assert(node != NULL);

    node->str = _256word;

    //printf("created node: %s\n", node->str);

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
        free(node->str);
        free(node);
        node = nextNode;
    }
    free(node->str);
    free(node);
}

void TableToCsv(HashTable * hashTable, FILE * CsvFile) {

    assert(hashTable != NULL);
    assert(CsvFile   != NULL);

    fprintf(CsvFile, "index,list_len\n");

    size_t len = hashTable->size;
    for (size_t i = 0; i < len; i++) {
        fprintf(CsvFile, "%zu,%zu\n", i, hashTable->tableItems[i].peers);
    }
}

void IndexDump(HashTable * hashTable, const char * key) {

    assert(hashTable != NULL);

    size_t index = hashTable->hashFunc(key);
    printf("hash val is %zu\n", index);

    Item item = hashTable->tableItems[index % hashTable->size];
    Node * node = item.node;

    
    for (size_t i = 0; i < item.peers; i++) {

        printf(" %s ->", (char*)(node->str));
        node = node->next;
    }
    printf("\n");
}

int __attribute__ ((noinline)) avxCmp(__m256i* str1, __m256i* str2) {

    __m256i first = *str1;
    __m256i second = *str2;
    
    __m256i cmp = _mm256_cmpeq_epi8 (first, second);
    int mask = _mm256_movemask_epi8 (cmp);

    return (mask - 0xffffffff);
}

void ReplaceHashFunc(HashTable * hashTable, size_t (*hashFunc)(const char * word)) {

    assert(hashTable != NULL);
    assert(hashFunc != NULL);

    hashTable->hashFunc = hashFunc;
}


