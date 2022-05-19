/* Do not edit this file. */
#include <stddef.h>

/* Handle to queue */
struct queue;

/* Return a pointer to a queue data structure with a maximum capacity of
 * 'capacity' if successful, otherwise return NULL. */
struct queue *queue_init(size_t capacity);

/* Cleanup queue.
 * Also prints the statistics. */
void queue_cleanup(struct queue *q);

/* Print queue statistics to stderr.
 * The format is: 'stats' num_of_pushes num_of_pops max_elements */
void queue_stats(const struct queue *q);

/* Push item the end of the queue.
 * Return 0 if successful, 1 otherwise. */
int queue_push(struct queue *q, int e);

/* Remove the first item from queue and return it.
 * Return the first item if successful, -1 otherwise. */
int queue_pop(struct queue *q);

/* Return the first item from queue. Leave queue unchanged.
 * Return the first item if successful, -1 otherwise. */
int queue_peek(const struct queue *q);

/* Return 1 if queue is empty, 0 if the queue contains any elements and
 * return -1 if the operation fails. */
int queue_empty(const struct queue *q);

/* Return the number of elements stored in the queue. */
size_t queue_size(const struct queue *q);
