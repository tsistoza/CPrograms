#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define DEFAULT 100

typedef struct Node {
    int key;
    int value;
    struct Node* next;
} Node;

typedef struct {
    Node** data;
    size_t capacity;
    size_t size;
    bool empty;
    Node* front;
    Node* back;
} unordered_hashmap;

unordered_hashmap* init_map();
void clear_map(unordered_hashmap* dict);
int hash_function(int key, int capacity);
bool insert_map(int key, int val, unordered_hashmap* dict);
void erase(int key, unordered_hashmap* dict);
bool contains(int num, unordered_hashmap* dict);
Node* get(int key, unordered_hashmap* dict);
void resize_map(unordered_hashmap* dict);
void printMap(unordered_hashmap* dict);
void unitTests();

int main() {
    unitTests(); // Verify
    return 0;
}

unordered_hashmap* init_map() {
    printf("CREATING HASHMAP\n");
    unordered_hashmap* newMap = malloc(sizeof(unordered_hashmap));
    newMap->capacity = DEFAULT;
    newMap->data = malloc(newMap->capacity * sizeof(Node*));
    for (int i=0; i<newMap->capacity; i++)
        newMap->data[i] = NULL;

    newMap->size = 0;
    newMap->empty = true;
    newMap->front = NULL;
    newMap->back = NULL;
    return newMap;
}

void clear_map(unordered_hashmap* dict) {
    Node* curr = dict->front;
    Node* next = NULL;
    while (curr != NULL) {
        next = curr->next;
        curr->next = NULL;
        free(curr);
        curr = next;
    }

    dict->capacity = DEFAULT;
    dict->size = 0;
    dict->empty = true;
    return;
}

int hash_function(int key, int capacity) {
    key ^= key >> capacity;
    key *= 0x45d9f3bU;
    key ^= key >> capacity;
    key *= 0x45d9f3bU;
    key ^= key >> capacity;
    return key % capacity;
}

bool insert_map(int key, int val, unordered_hashmap* dict) {
    printf("INSERTING MAP\n");
    if (contains(key, dict)) return false;
    
    int index = hash_function(key, dict->capacity); // Collision Check
    if (dict->data[index] != NULL) {
        printf("COLLISION");
        return false;
    }
    printf("index = %d\n", index);
    dict->data[index] = malloc(sizeof(Node));
    dict->data[index]->key = key;
    dict->data[index]->value = val;
    dict->data[index]->next = NULL;
    if (dict->size == 0)
        dict->front = dict->data[index];
    if (dict->back != NULL)
        dict->back->next = dict->data[index];
    dict->back = dict->data[index];
    dict->size++;
    return true;
}

void erase(int key, unordered_hashmap* dict) {
    int index = hash_function(key, dict->capacity);
    if (dict->data[index] == NULL)
        return;
    Node* curr = dict->front;
    Node* prev = NULL;
    while (curr->key != key) {
        prev = curr;
        curr = curr->next;
    }
    
    if (prev == curr) { // FRONT
        dict->front = curr->next;
        curr->next = NULL;
    }
    if (curr->key == key) { // Between front and back
        prev->next = curr->next;
        curr->next = NULL;
    }
    if (curr->next == NULL) { // BACK
        dict->back = prev;
        prev->next = NULL;
        curr->next = NULL;
    }

    free(dict->data[index]);
    dict->data[index] = NULL;
    dict->size--;
    return;
}

Node* get(int key, unordered_hashmap* dict) {
    int index = hash_function(key, dict->capacity);
    return dict->data[index];
}

bool contains(int key, unordered_hashmap* dict) {
    int index = hash_function(key, dict->capacity);
    if (dict->data[index] == NULL) return false;
    if (dict->data[index]->key == key) return true;
    return false;
}

void printMap(unordered_hashmap* dict) {
    printf("\n\n");
    printf("{ key --> value }\n\n");
    printf("START OF MAP\n");
    printf("-----------------\n");
    Node* currPtr = dict->front;
    while (currPtr != NULL) {
        printf("{ %d --> %d }\n", currPtr->key, currPtr->value);
        currPtr = currPtr->next;
    }
    printf("-----------------\n");
    printf("END OF MAP\n\n");
    return;
}

void unitTests() {
    // Unit Tests
    int numTestPassed = 0;

    // TEST 1
    unordered_hashmap* map = init_map();
    if (map == NULL) {
        fprintf(stderr, "MAP FAILED INIT. TEST 1 FAILED\n");
        fprintf(stdout, "Num Tests Passed: %d\n", numTestPassed);
        exit(EXIT_FAILURE);
    }
    numTestPassed++;
    
    // TEST 2
    for (int i=0; i<5; i++)
        insert_map(i, 1, map);
    if (map->size < 4) {
        fprintf(stderr, "ERROR: map->size is not correct. TEST 2 FAILED\n");
        fprintf(stdout, "Num Tests Passed: %d\n", numTestPassed);
        exit(EXIT_FAILURE);
    }
    numTestPassed++;

    // TEST 3
    int i=0;
    for (Node* curr = map->front; curr!=NULL; curr=curr->next, i++) {
        if (i == curr->key) continue;
        fprintf(stderr, "ERROR: map->key is not correct. TEST 3 FAILED\n");
        fprintf(stdout, "Num Tests Passed: %d\n", numTestPassed);
        exit(EXIT_FAILURE);
    }
    numTestPassed++;

    // TEST 4
    for (int i=0; i<5; i++)
        if (contains(i, map)) get(i, map)->value++;
    for (Node* curr = map->front; curr!=NULL; curr=curr->next) {
        if (curr->value == 2) continue;
        fprintf(stderr, "ERROR: map->values is not correct. TEST 4 FAILED\n");
        fprintf(stdout, "Num Tests Passed: %d\n", numTestPassed);
        exit(EXIT_FAILURE);
    }
    numTestPassed++;

    // TEST 5
    erase(4, map);
    erase(3, map);
    for (Node* curr = map->front; curr!=NULL; curr=curr->next) {
        if (curr->key != 3 && curr->key != 4) continue;
        fprintf(stderr, "ERROR: map->values is not correct. TEST 5 FAILED\n");
        fprintf(stdout, "Num Tests Passed: %d\n", numTestPassed);
        exit(EXIT_FAILURE);
    }
    numTestPassed++;

    printMap(map);
    clear_map(map);
    free(map);
    printf("TESTS 5/5 PASSED\n");
}
