#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "array.h"
#include "hash_func.h"

/**
 * Struct table
 * @param array the 2D array that contains all the nodes in the hash table
 * @param hash_func the function used for computng the hash value
 * @param max_load_factor maximum load factor of a hash table
 * @param capacity capacity of the array used to index the table
 * @param load current number of elements stored in the table
 */
struct table
{
    struct node **array;
    unsigned long (*hash_func)(unsigned char *);
    double max_load_factor;
    unsigned long capacity;
    unsigned long load;
};

/**
 * Struct node
 * @param key the string of characters that is the key for this node
 * @param value a resizing array, containing all the integer values for the key
 * @param next next pointer
 */
struct node
{
    char *key;
    struct array *value;
    struct node *next;
};

/**
 * This function creates new hash table and returns a pointer to it.
 *
 * @return a pointer to the struct table that is allocated on the heap
 */
void resize(struct table *t)
{
    size_t capacity = t->capacity;
    struct node **array = t->array;
    t->array = realloc(t->array, capacity * 2 * sizeof(struct node *));
    t->capacity = 2 * capacity;
    for (size_t i = 0; i < t->capacity; i++)
    {
        t->array[i] = NULL;
    }
    for (size_t i = 0; i < capacity; i++)
    {
        while (array[i] != NULL)
        {
            struct node *tmp = array[i];
            array[i] = array[i]->next;
            tmp->next = NULL;
            unsigned long hash_value = t->hash_func((unsigned char *)tmp->key);
            unsigned long hash_index = hash_value % t->capacity;
            struct node *new_node = malloc(sizeof(struct node));
            new_node->key = tmp->key;
            new_node->value = tmp->value;
            new_node->next = NULL;
            if (t->array[hash_index] == NULL)
            {
                t->array[hash_index] = new_node;
                free(tmp);
            }
            else
            {
                struct node *table_node = t->array[hash_index];
                while (table_node->next != NULL)
                {
                    table_node = table_node->next;
                }
                table_node->next = new_node;
                free(tmp);
            };
        }
    }
}

struct table *table_init(unsigned long capacity,
                         double max_load_factor,
                         unsigned long (*hash_func)(unsigned char *))
{
    //! allocate hash table on the heap
    struct table *new_table = malloc(sizeof(struct table));
    //! return NULL if malloc failed
    if (new_table == NULL)
    {
        return NULL;
    }
    //! allocate 2D array on the heap
    new_table->array = malloc(capacity * sizeof(struct node *));
    //! return NULL if malloc failed and free hash table
    if (new_table->array == NULL)
    {
        free(new_table);
        return NULL;
    }
    //! 2D array points to NULL
    for (size_t i = 0; i < capacity; i++)
    {
        new_table->array[i] = NULL;
    }
    new_table->capacity = capacity;
    new_table->load = 0;
    new_table->max_load_factor = max_load_factor;
    new_table->hash_func = hash_func;
    return new_table;
}
/**
 * This copies and inserts an array of characters as a key into the hash table,
 * together with the value, stored in a resizing integer array. If the key is
 * already present in the table, the value is appended to the existing array
 * instead. Returns 0 if successful and 1 otherwise.
 * table
 *
 * @param t the input hash table
 * @param key input key
 * @param value input value
 * @return 0 if successfull else return 1
 */
int table_insert(struct table *t, char *key, int value)
{
    if (t == NULL || key == NULL || t->array == NULL)
    {
        return 1;
    }
    //! calculate the hash value and index
    if ((double)t->load / (double)t->capacity >= (double)t->max_load_factor)
    {
        resize(t);
    }
    unsigned long hash_value = t->hash_func((unsigned char *)key);
    unsigned long hash_index = hash_value % t->capacity;
    //! if index position of hash table is empty insert the node
    if (t->array[hash_index] == NULL)
    {
        //! malloc a new node on the heap, return NULL on failure
        struct node *new_node = malloc(sizeof(struct node));
        if (new_node == NULL)
        {
            return 1;
        }
        //! malloc space on the heap to store the key
        new_node->key = malloc((strlen(key) * sizeof(char) + 1));
        if (new_node->key == NULL)
        {
            //! return NULL on failure and free node
            free(new_node);
            return 1;
        }
        //! copy the key
        strcpy(new_node->key, key);
        //! make an array and append the value to that array
        new_node->value = array_init(5);
        array_append(new_node->value, value);
        t->array[hash_index] = new_node;
        new_node->next = NULL;
        t->load++;
        return 0;
    }
    struct node *tmp = t->array[hash_index];
    while (tmp != NULL)
    {
        //! if the key already exist, append the value to array of that node
        if (strcmp(tmp->key, key) == 0)
        {
            array_append(tmp->value, value);
            return 0;
        }
        //! if the key does not exist in the hash table
        if (tmp->next == NULL)
        {
            //! malloc a new node on the heap, return NULL on failure
            struct node *new_node = malloc(sizeof(struct node));
            if (new_node == NULL)
            {
                return 1;
            }
            //! malloc space on the heap to store the key
            new_node->key = malloc((strlen(key) * sizeof(char) + 1));
            if (new_node->key == NULL)
            {
                //! return NULL on failure and free node
                free(new_node);
                return 1;
            }
            //! copy the key
            strcpy(new_node->key, key);
            //! make an array and append the value to that array
            new_node->value = array_init(5);
            array_append(new_node->value, value);
            new_node->next = NULL;
            //! add the node to the tail of the "link list"
            tmp->next = new_node;
            t->load++;
            return 0;
        }
        tmp = tmp->next;
    }
    return 1;
}

/**
 * This function returns the array of all inserted integer values for the
 * specified key.
 *
 * @param t the input hash table
 * @param key the input key
 * @return struct array for that specified key
 */
struct array *table_lookup(struct table *t, char *key)
{
    //! if t is NULL or 2D array of hash table is NULL or key is NULL
    if (t == NULL || t->array == NULL || key == NULL)
    {
        return NULL;
    }
    //! calculate the hash value and index
    unsigned long hash_value = t->hash_func((unsigned char *)key);
    unsigned long hash_index = hash_value % t->capacity;
    //! if the index position of hash table is empty return NULL
    if (t->array[hash_index] == NULL)
    {
        return NULL;
    }
    struct node *tmp = t->array[hash_index];
    while (tmp != NULL)
    {
        //! if the key is found, return the array
        if (strcmp(tmp->key, key) == 0)
        {
            return tmp->value;
        }
        tmp = tmp->next;
    }
    //! else return NULL
    return NULL;
}

/**
 * This funcction returns the load factor of the hash table. The load factor is
 * defined as: number of elements stored / size of hash table.
 *
 * @param t the input hash table
 * @return load factor for that hash table
 */
double table_load_factor(struct table *t)
{
    if (t == NULL || t->array == NULL)
    {
        return -1;
    }
    return (double)(t->load / t->capacity);
}

/**
 * This function removes the specified key and associated value from the
 * hash table.
 *
 * @param t the input hash table
 * @param key the input key
 * @return 0 if key was removed, 1 if the key was not present in the hash table
 */
int table_delete(struct table *t, char *key)
{
    //! if t is NULL or 2D array of hash table is NULL or key is NULL
    if (t == NULL || t->array == NULL || key == NULL)
    {
        return 1;
    }
    //! calculate the hash value and index
    unsigned long hash_value = t->hash_func((unsigned char *)key);
    unsigned long hash_index = hash_value % t->capacity;
    //! if the head node has to be deleted
    if (strcmp(t->array[hash_index]->key, key) == 0)
    {
        //! free the head node and let the next node be the head node.
        struct node *tmp = t->array[hash_index];
        t->array[hash_index] = t->array[hash_index]->next;
        tmp->next = NULL;
        free(tmp->key);
        array_cleanup(tmp->value);
        free(tmp);
        t->load--;
        return 0;
    }
    struct node *tmp = t->array[hash_index];
    while (tmp->next != NULL)
    {
        //! if the next node has to be deleted
        if (strcmp(tmp->next->key, key) == 0)
        {
            //! let current node point to the node after the next node
            struct node *delete = tmp->next;
            tmp->next = delete->next;
            delete->next = NULL;
            //! free the next node
            free(delete->key);
            array_cleanup(delete->value);
            free(delete);
            t->load--;
            return 0;
        }
        tmp = tmp->next;
    }
    //! else return 1
    return 1;
}

/**
 * This function cleans up the hash table data structure.
 *
 * @param t input hash table
 */
void table_cleanup(struct table *t)
{
    for (size_t i = 0; i < t->capacity; i++)
    {
        while (t->array[i] != NULL)
        {
            struct node *tmp = t->array[i];
            t->array[i] = tmp->next;
            free(tmp->key);
            array_cleanup(tmp->value);
            free(tmp);
        }
    }
    free(t->array);
    free(t);
}
