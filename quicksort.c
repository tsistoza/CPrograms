#include <stdio.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

typedef struct {
    int* data;
    size_t capacity;
    size_t size;
} Array;

int generate_random_number(int min, int max) {
    return (rand() %  (max - min + 1)) + min;
}

void quick_sort_dualpivot(Array* array, int left, int right);
void swap(Array* array, int index1, int index2);
void unit_tests();
void prettyPrint(Array* array);
void prettyPrintSection(Array* array, int low, int high);
void freeArray(Array** array);

int main() {
    srand(time(NULL));
    unit_tests();
    return 0;
}

void quick_sort_dualpivot(Array* array, int low, int high) {
    // dual pivot
    int p1 = low, p2 = high;
    int lowerbound = p1 + 1;
    int higherbound = p2 - 1;

    if (array->data[p1] > array->data[p2]) swap(array, p1, p2);

    for (int pointer=p1+1; pointer<p2;) {
        if (array->data[pointer] < array->data[p1]) {
            swap(array, lowerbound, pointer);
            swap(array, p1, lowerbound);
            p1++;
            lowerbound++;
            continue;
        }
        
        if (array->data[pointer] > array->data[p2]) {
            swap(array, higherbound, pointer);
            swap(array, p2, higherbound);
            p2--;
            higherbound--;
            continue;
        }

        pointer++;
    }

    if (low < p1-1) // zone 1
        quick_sort_dualpivot(array, low, p1-1);
    if (high > p2+1) // zone 3
        quick_sort_dualpivot(array, p2+1, high);
    if (p1+1 < p2-1) // zone 2
        quick_sort_dualpivot(array, p1+1, p2-1);
    
}

void swap(Array* array, int index1, int index2) {
    if (index1 == index2) return;
    int temp = array->data[index1];
    array->data[index1] = array->data[index2];
    array->data[index2] = temp;
    return;
}

void prettyPrint(Array* array) {
    printf("{ ");
    for (int i=0; i<array->size; i++) {
        if (i == array->size-1) {
            printf("%d ", array->data[i]);
            break;
        }
        printf("%d, ", array->data[i]);
    }
    printf("} \n\n");
}

void prettyPrintSection(Array* array, int low, int high) {
    printf("SECTION:\n");
    printf("{ ");
    for (int i=low; i<high+1; i++) {
        if (i == high) {
            printf("%d ", array->data[i]);
            break;
        }
        printf("%d, ", array->data[i]);
    }
    printf("} \n\n");
}

void freeArray(Array** array) {
    free((*array)->data);
    free(*array);
    *array = NULL;
    return;
}

void unit_tests() {
    int numTestPassed = 0;

    // generate_random_number() test
    int low=0,high=30;
    int number = generate_random_number(low, high);
    if (number < low || number > high) {
        fprintf(stderr, "generate_random_number failed\n\n");
        exit(EXIT_FAILURE);
    }
    numTestPassed++;

    Array* array = malloc(sizeof(array));
    array->capacity = 10;
    array->size = 0;
    array->data = malloc(array->capacity * sizeof(int*));
    
    int size=0;
    for (; size<array->capacity; size++)
        array->data[size] = generate_random_number(low, high);
    array->size = size;
    prettyPrint(array);

    // swap() test
    int leftElement = array->data[0];
    int rightElement = array->data[size-1];
    swap(array, 0, size-1);
    if (rightElement != array->data[0] && leftElement != array->data[size-1]) {
        free(array->data);
        free(array);
        fprintf(stderr, "swap failed\n\n");
        exit(EXIT_FAILURE);
    }
    swap(array, 0, size-1);
    numTestPassed++;

    // quick_sort_dualpivot() test
    quick_sort_dualpivot(array, 0, array->size-1);
    for (int i=1; i<array->size; i++) {
        if (array->data[i-1] > array->data[i]){
            free(array->data);
            free(array);
            fprintf(stderr, "SORT FAILED\n");
            exit(EXIT_FAILURE);
        }
    }
    printf("Sorted Array:\n");
    prettyPrint(array);

    // freeArray() test
    freeArray(&array);
    if (array != NULL) {
        fprintf(stderr, "did not dealloc properly \n\n");
        exit(EXIT_FAILURE);
    }
    numTestPassed++;

    printf("Number of Test Passed: %d\n\n", numTestPassed);
    return;
}