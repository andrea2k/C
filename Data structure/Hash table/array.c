/** Name: Andrea Li
 * Student ID: 13990268
 * Course: Datastructuur
 * Institution: University of Amsterdam
 *
 * array.c:
 * Functions in this API are used to create an array, insert value into the
 * array and more. This API will be used in main.c and hash_table.c
 *
*/

#include <stdlib.h>
#include "array.h"

/**
 * struct array
 * @param array 1D array that contains values
 * @param element numbers of elements stored
 * @param array_size size of array
 */
struct array
{
    int *array;
    size_t elements;
    size_t array_size;
};

/**
 * This function creates a pointer that points to the struct array that is
 * allocated on the heap.
 *
 * @param initial_capacity input of the array size
 * @return pointer that points to the struct
 */
struct array *array_init(unsigned long initial_capacity)
{
    struct array *new_array = malloc(sizeof(struct array));
    //! return NULL if malloc failed
    if (new_array == NULL)
    {
        return NULL;
    }
    new_array->array_size = initial_capacity;
    new_array->array = malloc(initial_capacity * sizeof(size_t));
    //! return NULL if malloc failed and free struct array
    if (new_array->array == NULL)
    {
        free(new_array);
        return NULL;
    }
    new_array->elements = 0;
    return new_array;
}

/**
 * This function frees the struct array and the 1D array in the struct.
 *
 * @param a input array
 */
void array_cleanup(struct array *a)
{
    free(a->array);
    free(a);
}

/**
 * This function returns the element at the index position in the array.
 *
 * @param a input array
 * @param index the index position
 * @return -1 if the index is not a valid position in the array else return the
 * value
 */
int array_get(struct array *a, unsigned long index)
{
    //! if a is NULL or index is bigger than number of elements in the array.
    if (a == NULL || index >= a->elements)
    {
        return -1;
    }
    return a->array[index];
}

/**
 * This function adds the element at the end of the array.
 *
 * @param a input array
 * @param elem input value
 * @return 0 if successfull, 1 otherwise
 */
int array_append(struct array *a, int elem)
{
    //! if input array is NULL
    if (a == NULL)
    {
        return 1;
    }
    //! resize the array if number of elements is equal to size of array
    if (a->elements == a->array_size)
    {
        //! if realloc failed
        a->array = realloc(a->array, 2 * a->array_size * sizeof(size_t));
        if (a->array == NULL)
        {
            return 1;
        }
        a->array_size = a->array_size * 2;
    }
    a->array[a->elements] = elem;
    a->elements++;
    return 0;
}

/**
 * This function returns the number of elements in the array.
 *
 * @param a input array
 * @return 0 if input array is NULL,else return number of elements in the array.
 */
unsigned long array_size(struct array *a)
{
    //! if input array is NULL
    if (a == NULL)
    {
        return 0;
    }
    return a->elements;
}
