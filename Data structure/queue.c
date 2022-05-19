
#include <stdio.h>
#include <stdlib.h>

#include "queue.h"

/**
 * queue data structure
 * @param pushes number of pushes
 * @param pops number of pops
 * @param array 1D array that contains maze locations
 * @param front index of the first item
 * @param rear index of the end of the queue
 * @param max_element maximum numbers of elements stored
 * @param array_size size of array
 */
struct queue
{
    int pushes;
    int pops;
    int *array;
    size_t front;
    size_t rear;
    size_t max_element;
    size_t array_size;
};

/**
 * This function creates a pointer that points to the queue data struct, it also
 * creates a pointer that points to the 1D array in the struct.
 *
 * @param capacity input of the array size
 * @param maze_queue pointer to struct
 * @param rear start index is 0
 * @param array pointer to 1D array
 * @return pointer that points to the struct
 */
struct queue *queue_init(size_t capacity)
{
    if (capacity <= 0)
    {
        capacity = 10000;
    }
    //! heap allocated struct
    struct queue *maze_queue = malloc(sizeof(struct queue));
    //! if malloc failed return NULL
    if (maze_queue == NULL)
        return NULL;
    maze_queue->array_size = capacity;
    maze_queue->front = 0;
    maze_queue->rear = 0;
    maze_queue->pushes = 0;
    maze_queue->pops = 0;
    maze_queue->max_element = 0;
    //! heap allocated array
    maze_queue->array = malloc(capacity * sizeof(size_t));
    //! if malloc failed return NULL
    if (maze_queue->array == 0)
    {
        free(maze_queue);
        return NULL;
    }
    return maze_queue;
}

/**
 * This functie deallocate the memory block that were allocated in function
 * "queue_init"
 *
 * @param s pointer to the struct
 */
void queue_cleanup(struct queue *q)
{
    //! free 1D array
    free(q->array);
    //! free struct
    free(q);
}

/**
 * This function prints the data of the struct.
 *
 * @param s pointer to the struct
 */
void queue_stats(const struct queue *q)
{
    //! print number of pusehs, number of pops and max numbers of element stored
    fprintf(stderr, "stats %d %d %ld\n", q->pushes, q->pops, q->max_element);
}

/**
 * This function push item onto the end of the queue.
 *
 * @param s pointer to the struct
 * @param c maze location
 * @return 0 if successful, 1 otherwise
 */
int queue_push(struct queue *q, int e)
{
    //! maze location must be positive number
    if (q == NULL || e < 0)
    {
        return 1;
    }
    else if (q->rear == q->array_size)
    {
        //! increase array size if array is full
        q->array = realloc(q->array, 2 * q->array_size * sizeof(size_t));
        if (q->array == NULL)
        {
            printf("Memory realloc not available\n");
            return 1;
        }
        q->array_size = q->array_size * 2;
    }
    q->pushes++;
    q->array[q->rear] = e;
    q->rear++;
    //! Remember max numbers of elements stored
    if (q->rear - q->front > q->max_element)
    {
        q->max_element = q->rear - q->front;
    }
    return 0;
}

/**
 * This function remove the first item from queue and return it.
 *
 * @param s pointer to the struct
 * @return maze location if successful , -1 if not successful
 */
int queue_pop(struct queue *q)
{
    //! if array is empty
    if (q == NULL || q->rear == q->front)
    {
        return -1;
    }
    q->pops++;
    return q->array[q->front++];
}

/**
 * This function return the first item from queue. Leave queue unchanged.
 *
 * @param s pointer to the struct
 * @return top item if successful , -1 if not successful
 */
int queue_peek(const struct queue *q)
{
    //! if array is empty
    if (q == NULL || q->rear == q->front)
    {
        return -1;
    }
    return q->array[q->front];
}

/**
 * This function checks if the queue is empty.
 * @param s pointer to the struct
 * @return 1 if queue is empty, 0 if it contains element, -1 if operation fails.
 */
int queue_empty(const struct queue *q)
{
    if (q == NULL)
    {
        return -1;
    }
    //! if array is empty
    else if (q->rear == q->front)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/**
 * This function returns the number of element stored in the queue.
 *
 * @param s pointer to the struct
 * @return numbers of elements stored in the queue.
 */
size_t queue_size(const struct queue *q)
{
    return q->rear - q->front;
}
