#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct {
    int* ptr;
    size_t capacity;
    size_t size;
    int front;
    int back;
} vector;

vector* init_vector(int capacity);
void free_vector(vector* vec);
void resize_vector(vector* vec);
void push_back(int num, vector* vec);
void pop_back(int num, vector* vec);
void prettyPrint(vector* vec);


int main() {
    int capacity = 1, size = 0;
    vector* newVec = init_vector(capacity);
    
    for (int i=0; i<20; i++)
        push_back(i, newVec);
    prettyPrint(newVec);
    
    for (int i=0; i<10; i++)
        pop_back(i, newVec);
    prettyPrint(newVec);
    
    for (int i=newVec->back; i<50; i*=2) 
        push_back(i, newVec);
    prettyPrint(newVec);

    free_vector(newVec);
    return 0;
}

vector* init_vector(int capacity) {
    if (capacity <= 0) {
        fprintf(stderr, "INVALID CAPACITY, CAPACITY > 0");
        exit(EXIT_FAILURE);
    }
    vector* newVec = malloc(sizeof(vector));
    if (newVec == NULL) {
        fprintf(stderr, "new vector failed, memory alloc");
        exit(EXIT_FAILURE);
    }

    newVec->ptr = malloc(capacity * sizeof(int));
    newVec->capacity = capacity;
    newVec->size = 0;
    newVec->front = -1;
    newVec->back = -1;
    return newVec;
}

void free_vector(vector* vec) {
    free(vec->ptr);
    free(vec);
    return;
}

void resize_vector(vector* vec) {
    if (vec==NULL) return;
    vec->capacity *= 2;
    int* temp_ptr = realloc(vec->ptr, vec->capacity*sizeof(int));
    if (temp_ptr != NULL) {
        vec->ptr = temp_ptr;
        return;
    }

    fprintf(stderr, "resize_vector: vector reallocation failed");
    free_vector(vec);
    exit(EXIT_FAILURE);
}

void push_back(int num, vector* vec) {
    if (vec->size >= vec->capacity) // resize
        resize_vector(vec);
    if (vec->size == 0) vec->front = num;
    vec->back = num;
    vec->ptr[vec->size++] = num;
}

void pop_back(int num, vector* vec) {
    if (vec == NULL)
        return;

    if (vec->size == 0) return;
    vec->ptr[vec->size-1] = -1;
    vec->size--;
    vec->back = vec->ptr[vec->size-1];
}

void prettyPrint(vector* vec) {
    printf("vector size = %zu, xs capacity = %zu\n", vec->size, vec->capacity);
    printf("vector front: %d, vector back: %d\n", vec->front, vec->back);
    printf("{ ");
    for (int i=0; i<vec->size; i++)
        (i < vec->size-1) ? printf("%d, ", vec->ptr[i]) : printf("%d ", vec->ptr[i]);
    printf("}\n\n");
    return;
}