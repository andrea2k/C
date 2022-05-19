#include <stdio.h>
#include <stdlib.h>

#include "stack.h"

/**
 * Stack data structure
 * @param pushes number of pushes
 * @param pops number of pops
 * @param array 1D array that contains maze locations
 * @param top current index of the 1D array
 * @param max_element maximum numbers of elements stored
 * @param array_size size of array
 */
struct stack
{
    int pushes;
    int pops;
    int *array;
    size_t top;
    size_t max_element;
    size_t array_size;
};

/**
 * This function creates a pointer that points to the stack data struct, it also
 * creates a pointer that points to the 1D array in the struct.
 *
 * @param capacity input of the array size
 * @param maze_stack pointer to struct
 * @param top start index is 0
 * @param array pointer to 1D array
 * @return pointer that points to the struct
 */
struct stack *stack_init(size_t capacity)
{
    //! heap allocated struct
    struct stack *maze_stack = malloc(sizeof(struct stack));
    //! if malloc failed return NULL
    if (maze_stack == NULL)
        return NULL;
    maze_stack->array_size = capacity;
    maze_stack->top = 0;
    maze_stack->pushes = 0;
    maze_stack->pops = 0;
    //! heap allocated array
    maze_stack->array = malloc(capacity * sizeof(size_t));
    maze_stack->max_element = 0;
    //! if malloc failed return NULL
    if (maze_stack->array == 0)
    {
        free(maze_stack);
        return NULL;
    }
    return maze_stack;
}

/**
 * This functie deallocate the memory block that were allocated in function
 * "stack_init"
 *
 * @param s pointer to the struct
 */
void stack_cleanup(struct stack *s)
{
    //! free 1D array
    free(s->array);
    //! free struct
    free(s);
}

/**
 * This function prints the data of the struct on s.
 *
 * @param s pointer to the struct
 */
void stack_stats(const struct stack *s)
{
    //! print number of pusehs, number of pops and max numbers of element stored
    fprintf(stderr, "stats %d %d %ld\n", s->pushes, s->pops, s->max_element);
}

/**
 * This function push item onto the stack.
 *
 * @param s pointer to the struct
 * @param c maze location
 * @return 0 if successful, 1 otherwise
 */
int stack_push(struct stack *s, int c)
{
    //! maze location must be positive number
    if (s == NULL || c < 0)
    {
        return 1;
    }
    else if (s->top == s->array_size)
    {
        //! increase array size if array is full
        s->array = realloc(s->array, 2 * s->array_size * sizeof(size_t));
        if (s->array == NULL)
        {
            printf("Memory realloc not available\n");
            return 1;
        }
        s->array_size = s->array_size * 2;
    }
    s->pushes++;
    s->array[s->top] = c;
    s->top++;
    //! Remember max numbers of elements stored
    if (s->top > s->max_element)
    {
        s->max_element = s->top;
    }
    return 0;
}

/**
 * This function pops the last stored item from the stack.
 *
 * @param s pointer to the struct
 * @return maze location if successful , -1 if not successful
 */
int stack_pop(struct stack *s)
{
    //! if array is empty
    if (s == NULL || s->top == 0)
    {
        return -1;
    }
    s->pops++;
    s->top--;
    return s->array[s->top];
}

/**
 * This function returns top of item from stack and leave stack unchanged.
 *
 * @param s pointer to the struct
 * @return top item if successful , -1 if not successful
 */
int stack_peek(const struct stack *s)
{
    //! if array is empty
    if (s == NULL || s->top == 0)
    {
        return -1;
    }
    return s->array[s->top - 1];
}

/**
 * This function checks if the stack is empty.
 * @param s pointer to the struct
 * @return 1 if stack is empty, 0 if it contains element, -1 if operation fails.
 */
int stack_empty(const struct stack *s)
{
    if (s == NULL)
    {
        return -1;
    }
    //! if array is empty
    else if (s->top == 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/**
 * This function returns the number of element stored in the stack.
 *
 * @param s pointer to the struct
 * @return numbers of elements stored in the stack.
 */
size_t stack_size(const struct stack *s)
{
    return s->top;
}
