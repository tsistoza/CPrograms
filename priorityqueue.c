// Min Heap - Priority Queue
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct {
    int value;
} Node;

typedef struct {
    Node** data;
    size_t capacity;
    size_t size;
} PriorityQueue;

int generate_random_number(int min, int max) {
    return (rand() %  (max - min + 1)) + min;
}

void unit_tests();
PriorityQueue* initPriorityQueue(size_t capacity);
int priorityQueue_Dequeue(PriorityQueue* queue);
void priorityQueue_Enqueue(PriorityQueue* queue, Node* newNode);
void priorityQueue_Realloc(PriorityQueue* queue, size_t newCapacity);
int leftChild(int index);
int rightChild(int index);
int parent(int index);
void shiftUp(Node** data, int curr);
void shiftDown(Node** data, int curr, size_t size);
void swap(int index1, int index2, Node** data);
void prettyPrintQueue(PriorityQueue* queue);
void priorityQueueFree(PriorityQueue** queue);

int main() {
    srand(time(NULL));
    unit_tests();
    return 0;
}

void unit_tests() {
    int min = 1, max = 20;
    size_t capacity = 10;
    int numTestPassed = 0;

    // Helper Functions Test
    int index = 5;
    if (leftChild(index) != 11) {
        fprintf(stderr, "LeftChild Function is not implemented properly\n\n");
        exit(EXIT_FAILURE);
    }
    if (rightChild(index) != 12) {
        fprintf(stderr, "RightChild Function is not implemented properly\n\n");
        exit(EXIT_FAILURE);
    }
    if (parent(index) != 2) {
        fprintf(stderr, "Parent Function is not implemented properly\n\n");
        exit(EXIT_FAILURE);
    }
    Node** check;
    check = malloc(2 * sizeof(Node*));

    Node* node1 = malloc(sizeof(Node));
    node1->value = 1;
    Node* node2 = malloc(sizeof(Node));
    node2->value = 0;
    check[0] = node1;
    check[1] = node2;
    swap(0, 1, check);
    for(int i=0; i<2; i++){
        if (check[i]->value != i){
            fprintf(stderr, "Swap Function is not implemented properly\n\n");
            exit(EXIT_FAILURE);
        }
    }
    free(check);
    free(node1);
    free(node2);
    numTestPassed++;

    PriorityQueue* queue = initPriorityQueue(capacity);

    // priorityQueue_Enqueue
    for (int i=0; i<10; i++) {
        Node* newNode = malloc(sizeof(Node));
        newNode->value = generate_random_number(min, max);
        priorityQueue_Enqueue(queue, newNode);
    }
    for (int i=1; i<queue->size; i++) {
        int root = queue->data[parent(i)]->value;
        int currValue = queue->data[i]->value;
        if (currValue < root) {
            priorityQueueFree(&queue);
            fprintf(stderr, "Enqueue Failed\n\n");
            exit(EXIT_FAILURE);
        }
    }
    prettyPrintQueue(queue);
    numTestPassed++;

    // priorityQueue_Dequeue
    for (int i=0, prev=INT_MIN; i<3; i++) {
        int curr = priorityQueue_Dequeue(queue);
        printf("Dequeued %d\n", curr);
        if (prev > curr) {
            priorityQueueFree(&queue);
            fprintf(stderr, "Failed Dequeue\n\n");
            exit(EXIT_FAILURE);
        }
        prev = curr;
    }
    prettyPrintQueue(queue);
    numTestPassed++;

    // Free PriorityQueue
    priorityQueueFree(&queue);
    if (queue != NULL) {
        fprintf(stderr, "FAILED TO DEALLOCATE MEMORY\n\n");
        exit(EXIT_FAILURE);
    }
    numTestPassed++;
    
    printf("Num Test Passed: %d\n\n", numTestPassed);
    return;
}

PriorityQueue* initPriorityQueue(size_t capacity) {
    PriorityQueue* queue = malloc(sizeof(PriorityQueue));
    queue->capacity = capacity;
    queue->size = 0;
    queue->data = malloc(queue->capacity * sizeof(Node*));
    return queue;
}

int priorityQueue_Dequeue(PriorityQueue* queue) {
    if (queue->size == 0) return -1;
    int rootVal = queue->data[0]->value;
    if (queue->size == 1) {
        free(queue->data[0]);
        queue->data[0] = NULL;
        return rootVal;
    }
    
    swap(0, queue->size-1, queue->data);
    free(queue->data[queue->size-1]);
    queue->data[queue->size-1] = NULL;
    queue->size--;
    shiftDown(queue->data, 0, queue->size);
    return rootVal;
}

void priorityQueue_Enqueue(PriorityQueue* queue, Node* newNode) {
    Node** data = queue->data;
    if (queue->size >= queue->capacity)
        priorityQueue_Realloc(queue, queue->capacity*2);
    if (queue->size == 0){
        data[0] = newNode;
        queue->size++;
        return;
    }

    data[queue->size] = newNode;
    shiftUp(data, queue->size);
    queue->size++;
}

void priorityQueue_Realloc(PriorityQueue* queue, size_t newCapacity) {
    queue->capacity = newCapacity;
    Node** data = realloc(queue->data, newCapacity * sizeof(Node*));
    if (data == NULL) {
        fprintf(stderr, "Reallocation Failed\n\n");
        exit(EXIT_FAILURE);
    }

    queue->data = data;
    return;
}

int leftChild(int index){ 
    return 2*index + 1;
}

int rightChild(int index) {
    return 2*index + 2;
}

int parent(int index) {
    return (index - 1) / 2;
}

void swap(int index1, int index2, Node** data) {
    Node* tempNode = data[index1];
    data[index1] = data[index2];
    data[index2] = tempNode;
    return;
}

void shiftUp(Node** data, int curr) {
    int value = data[curr]->value;
    int parentIndex = parent(curr);

    if (parentIndex < 0) return;

    int parentValue = data[parentIndex]->value;
    if (parentValue > value) {
        swap(curr, parentIndex, data);
        shiftUp(data, parentIndex);
    }
    return;
}

void shiftDown(Node** data, int curr, size_t size) {
    if (curr >= size-1) return;

    int leftIndex = leftChild(curr);
    int rightIndex = rightChild(curr);
    
    if (leftIndex >= size) return;

    int leftValue = (leftIndex < size) ? data[leftIndex]->value : INT_MAX;
    int rightValue = (rightIndex < size) ? data[rightIndex]->value : INT_MAX;
    int currValue = data[curr]->value;

    int newIndex = (leftValue < rightValue) ? leftIndex : rightIndex;
    if (newIndex == leftIndex) 
        swap(leftIndex, curr, data);
    if (newIndex == rightIndex)
        swap(rightIndex, curr, data);
    shiftDown(data, newIndex, size);
    return;
}

void prettyPrintQueue(PriorityQueue* queue) {
    printf("PriorityQueue Begin: --> ");
    for (int i=0; i<queue->size; i++)
        printf("%d --> ", queue->data[i]->value);
    printf("End\n\n");
    return;
}

void priorityQueueFree(PriorityQueue** queue) {
    for (int i=0; i<(*queue)->size; i++)
        free((*queue)->data[i]);
    free((*queue)->data);
    free(*queue);
    *queue = NULL;
    return;
}
