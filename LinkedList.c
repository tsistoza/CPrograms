#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int value;
    struct Node* next;
} node_t;

typedef struct {
    struct Node* front;
    struct Node* back;
    size_t size;
} LinkedList;

void unit_tests();
LinkedList* initLinkedList();
void linkedListAppend(LinkedList* list, int value);
void linkedListRemove(LinkedList* list, int value);
void printLinkedList(LinkedList* list);
void freeLinkedList(LinkedList** list);

int main() {
    unit_tests();
    return 0;
}

void unit_tests() {
    int numTestPassed = 0;

    LinkedList* list = initLinkedList();
    
    // linkedlistappend test
    for(int i=0; i<10; i++)
       linkedListAppend(list, i);
    struct Node* currPtr = list->front;
    int i=0;
    while (currPtr != NULL) {
        if (currPtr->value != i) {
            freeLinkedList(&list);
            fprintf(stderr, "LinkedListAppend Error\n\n");
            exit(EXIT_FAILURE);
        }
        currPtr = currPtr->next;
        i++;
    }
    printLinkedList(list);
    numTestPassed++;

    // linkedlistremove test
    for (int i=0; i<10; i+=2)
        linkedListRemove(list, i);
    currPtr = list->front;
    while (currPtr != NULL) {
        if (currPtr->value % 2 == 0) {
            freeLinkedList(&list);
            fprintf(stderr, "LinkedListRemove Error\n\n");
            exit(EXIT_FAILURE);
        }
        currPtr = currPtr->next;
    }
    printLinkedList(list);
    numTestPassed++;

    // freeLinkedList test
    freeLinkedList(&list);
    if (list != NULL) {
        fprintf(stderr,"Failed to deallocate LinkedList\n\n");
        exit(EXIT_FAILURE);
    }
    numTestPassed++;

    printf("Number of Tests Passed: %d\n\n", numTestPassed);
    return;
}

LinkedList* initLinkedList() {
    LinkedList* list = malloc(sizeof(LinkedList));
    list->front = NULL;
    list->back = NULL;
    list->size = 0;
    return list;
}

void linkedListAppend(LinkedList* list, int value) {
    if (list->size == 0) {
        list->front = malloc(sizeof(struct Node));
        list->front->value = value;
        list->front->next = NULL;
        list->back = list->front;
        list->size++;
        return;
    }

    struct Node* newNode = malloc(sizeof(struct Node));
    newNode->value = value;
    newNode->next = NULL;
    list->back->next = newNode;
    list->back = newNode;
    list->size++;
    return;
}

void linkedListRemove(LinkedList* list, int value) {
    struct Node* currPtr = list->front;
    struct Node* prevPtr = NULL;
    while (currPtr != NULL) {
        if (currPtr->value == value) break;
        prevPtr = currPtr;
        currPtr = currPtr->next;
    }
    if (currPtr == NULL) return;

    if (list->front == currPtr) { // Front of the list
        list->front = list->front->next;
        currPtr->next = NULL;
        free(currPtr);
    } else if (list->back == currPtr) { // back of list
        prevPtr->next = NULL;
        free(currPtr);
    } else { // between front and end
        prevPtr->next = currPtr->next;
        currPtr->next = NULL;
        free(currPtr);
    }

    return;
}

void printLinkedList(LinkedList* list) {
    printf("LinkedList Begin: --> ");
    struct Node* currPtr = list->front;
    while (currPtr != NULL) {
        printf("%d --> ", currPtr->value);
        currPtr = currPtr->next;
    }
    printf("NULL \n\n");
    return;
}

void freeLinkedList(LinkedList** list) {
    struct Node* prev = (*list)->front;
    struct Node* currPtr = (prev == NULL) ? NULL : prev->next;
    while (currPtr != NULL) {
        free(prev);
        prev = currPtr;
        currPtr = currPtr->next;
    }
    free(prev);
    free(*list);
    *list = NULL;
    return;
}