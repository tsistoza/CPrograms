#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int* top;
    int* data;
    size_t size;
    size_t capacity;
} Stack;

void stack_push(Stack* stack, int item);
void stack_realloc(Stack* stack, size_t newCapacity);
int stack_top(Stack* stack);
int stack_pop(Stack* stack);
void unit_tests();
void stack_free(Stack** stack);
void stack_print(Stack* stack);

int main () {
    unit_tests();
    return 0;
}

void unit_tests() {
    int numTestPassed = 0;
    Stack* stack = malloc(sizeof(Stack));
    stack->capacity = 20;
    stack->size = 0;
    stack->data = malloc(stack->capacity * sizeof(int));
    stack->top = &(stack->data[stack->size]);

    // stack_push() test
    for (int i=0; i<10; i++)
        stack_push(stack, i);
    stack_print(stack);
    if (stack_top(stack) < 9) {
        fprintf(stderr,  "stack_push failed\n\n");
        stack_free(&stack);
        exit(EXIT_FAILURE);
    }
    numTestPassed++;


    // stack_pop test
    for (int i=0; i<3; i++) stack_pop(stack);
    stack_print(stack);
    if (stack_top(stack) > 6) {
        fprintf(stderr, "stack_pop failed\n\n");
        stack_free(&stack);
        exit(EXIT_FAILURE);
    }
    numTestPassed++;

    // stack_realloc() test
    stack_realloc(stack, stack->capacity*2);
    if (stack->capacity <= 20) {
        fprintf(stderr, "stack reallocation failed\n");
        stack_free(&stack);
        exit(EXIT_FAILURE);
    }
    stack_realloc(stack, 10);

    // stack_free() test
    stack_free(&stack);
    if (stack != NULL) {
        fprintf(stderr, "stack has not been freed\n");
        exit(EXIT_FAILURE);
    }
    numTestPassed++;

    printf("Test Passed: %d \n\n", numTestPassed);
}

void stack_push(Stack* stack, int item) {
    if (stack->size >= stack->capacity)
        stack_realloc(stack, stack->capacity*2);
    
    if (stack->size <= 0)
        *(stack->top) = item;
    else {
        stack->top++;
        *(stack->top) = item;
    }

    stack->size++;
    return;
}

void stack_realloc(Stack* stack, size_t newCapacity) {
    int* temp_data = realloc(stack->data, newCapacity);
    if (temp_data == NULL) {
        fprintf(stderr, "reallocation failed \n\n");
        exit(EXIT_FAILURE);
    }
    stack->capacity = newCapacity;
    return;
}

int stack_top(Stack* stack) {
    return *(stack->top);
}

int stack_pop(Stack* stack) {
    if (stack->size <= 0) return -1;

    int item = *(stack->top);
    stack->top--;
    stack->size--;
    return item;
}

void stack_free(Stack** stack) {
    free((*stack)->data);
    (*stack)->data = NULL;
    free(*stack);
    *stack = NULL;
    return;
}

void stack_print(Stack* stack) {
    printf("Curr Stack: \n");
    for (int i=stack->size-1; i>=0; i--)
        printf("  %d  \n", stack->data[i]);
    printf("-------------------------\n");
    return;
}