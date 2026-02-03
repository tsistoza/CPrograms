#include <stdio.h>
#include <stdlib.h>

typedef struct {  // FIFO
    int* first;
    int* last;
    int* data;
    size_t size;
    size_t capacity;
} Queue;

void unit_tests();
int dequeue(Queue* queue);
void enqueue(Queue* queue, int item);
void printQueue(Queue* queue);
void queue_realloc(Queue* queue, size_t newCapacity);
void queue_free(Queue** queue);

int main() {
    unit_tests();
    return 0;
}

void unit_tests() {
    int numTestPassed = 0;

    Queue* queue = malloc(sizeof(Queue));
    queue->capacity = 15;
    queue->size = 0;
    queue->data = malloc(queue->capacity * sizeof(int));
    queue->first = &(queue->data[0]);
    queue->last = &(queue->data[0]);

    // queue_realloc Test
    queue_realloc(queue, 20);
    if (queue->capacity != 20) {
        fprintf(stderr, "Reallocation failed\n\n");
        queue_free(&queue);
        exit(EXIT_FAILURE);
    }
    queue_realloc(queue, 15);

    // Enqueue Test
    for (int i=0; i<10; i++)
        enqueue(queue, i);
    if (*(queue->first) != 0) {
        queue_free(&queue);
        fprintf(stderr, "Enqueue failed\n\n");
        exit(EXIT_FAILURE);
    }
    printQueue(queue);
    numTestPassed++;

    // Dequeue Test
    for (int i=0; i<3; i++) 
        dequeue(queue);
    if (*(queue->first) != 3) {
        queue_free(&queue);
        fprintf(stderr, "Dequeue Failed\n\n");
        exit(EXIT_FAILURE);
    }
    printQueue(queue);
    numTestPassed++;

    // Test Circular
    for (int i=0; i<3; i++) 
        enqueue(queue, i);
    if (*(queue->last) != 2) {
        queue_free(&queue);
        fprintf(stderr, "Circular Queue as failed\n\n");
        exit(EXIT_FAILURE);
    }
    printQueue(queue);
    numTestPassed++;
    
    // queue_free() test
    queue_free(&queue);
    if (queue != NULL) {
        fprintf(stderr, "Queue has not been freed properly\n");
        exit(EXIT_FAILURE);
    }
    numTestPassed++;

    printf("All Test Passed: %d\n\n", numTestPassed);
    return;
}

int dequeue(Queue* queue) {
    if (queue->size <= 0) {
        fprintf(stderr, "Queue is empty.");
        return -1;
    }

    int dequeuedItem = *(queue->first);

    if (queue->first == &(queue->data[queue->capacity-1]))
        queue->first = &(queue->data[0]);
    else
        queue->first++;
    queue->size--;
    return dequeuedItem;
}

void enqueue(Queue* queue, int item) {
    if (queue->size >= queue->capacity)
        queue_realloc(queue, queue->capacity*2);

    if (queue->size == 0) {
        *(queue->last) = item;
        queue->size++;
        return;
    }

    if (queue->last == &(queue->data[queue->capacity-1]) && queue->size < queue->capacity)
        queue->last = &(queue->data[0]);
    else 
        queue->last++;
    
    *(queue->last) = item;
    queue->size++;
    return;
}

void printQueue(Queue* queue) {
    printf("Queue: First ---> ");
    for (int* qPtr = queue->first; qPtr != queue->last+1;) {
        printf("%d ", *qPtr);

        if (qPtr == &(queue->data[queue->capacity-1]))
            qPtr = &(queue->data[0]);
        else
            qPtr++;

    }
    printf(" <--- Last\n\n");
    return;
}

void queue_realloc(Queue* queue, size_t newCapacity) {
    queue->capacity = newCapacity;
    int* tempData = realloc(queue->data, queue->capacity);
    if (tempData == NULL) {
        queue_free(&queue);
        fprintf(stderr, "Queue reallocation failed\n\n");
        exit(EXIT_FAILURE);
    }
    queue->data = tempData;
    return;
}

void queue_free(Queue** queue) {
    free((*queue)->data);
    (*queue)->data = NULL;
    free(*queue);
    *queue = NULL;
    return;
}