#include "list.h"

/**
 * Struct node
 * @param value the value of the node
 * @param next points to the the next node
 */
struct node
{
    int value;
    struct node *next;
};

/**
 * Struct list
 * @param number_nodes numbers of nodes in the list
 * @param head points to the first node in the list
 * @param tail points to the last node in the list
 */
struct list
{
    size_t number_nodes;
    struct node *head;
    struct node *tail;
};

/**
 * This function creates new linked list and returns a pointer to it.
 * @return a pointer to the struct list that is allocated on the heap
 */
struct list *list_init(void)
{
    struct list *new_list = malloc(sizeof(struct list));
    //! return NULL if malloc failed
    if (new_list == NULL)
    {
        return NULL;
    }
    new_list->head = NULL;
    new_list->tail = NULL;
    new_list->number_nodes = 0;
    return new_list;
}

/**
 * This creates a new node that contains the number num and returns a pointer to
 * it.
 * @param num the value of the node
 * @return a pointer to the node that is allocated on the heap
 */
struct node *list_new_node(int num)
{
    //! the value has to be positive number
    if (num < 0)
    {
        return NULL;
    }
    //! return NULL if malloc failed
    struct node *new_node = malloc(sizeof(struct node));
    if (new_node == NULL)
    {
        return NULL;
    }
    new_node->value = num;
    new_node->next = NULL;
    return new_node;
}

/**
 * This function returns a pointer to the first node of the list.
 * @param l the list
 * @return a pointer to the first node of the list.
 */
struct node *list_head(struct list *l)
{
    //! return NULL if list is empty or list is NULL.
    if (l->number_nodes == 0 || l == NULL)
    {
        return NULL;
    }
    struct node *first_node = l->head;
    return first_node;
}

/**
 * This function returns a pointer to the node after node N.
 * @param n the node
 * @return a pointer to the node after node N.
 */
struct node *list_next(struct node *n)
{
    //! return NULL if node is NULL or the next node is NULL.
    if (n == NULL || n->next == NULL)
    {
        return NULL;
    }
    struct node *next = n->next;
    return next;
}

/**
 * This function inserts node N at the front of list L.
 * @param l the list
 * @param n the node
 * @return 0 if N was successfully inserted, 1 otherwise
 */
int list_add_front(struct list *l, struct node *n)
{
    //! if list is NULL or node is NULL.
    if (n == NULL || l == NULL)
    {
        return 1;
    }
    if (l->number_nodes == 0)
    {
        l->head = n;
        l->tail = n;
        l->number_nodes++;
        n->next = NULL;
    }
    else
    {
        n->next = l->head;
        l->head = n;
        l->number_nodes++;
    }
    return 0;
}

/**
 * This function returns a pointer to the last node of the list.
 * @param l the list
 * @param a pointer to the last node of the list.
 */
struct node *list_tail(struct list *l)
{
    //! return NULL if list is empty or list is NULL.
    if (l == NULL || l->number_nodes == 0)
    {
        return NULL;
    }
    struct node *last_node = l->tail;
    return last_node;
}

/**
 * This function returns a pointer to the node before node N in the list.
 * @param n the node
 * @param l the list
 * @return a pointer to the the node before node N.
 */
struct node *list_prev(struct list *l, struct node *n)
{
    //! return NULL if list is empty or list is NULL or node is NULL or the
    //! node is not in the list.
    if (list_node_present(l, n) != 1)
    {
        return NULL;
    }
    struct node *previous = l->head;
    while (previous->next != NULL)
    {
        if (previous->next == n)
        {
            return previous;
        }
        previous = previous->next;
    }
    return NULL;
}

/**
 * This function inserts node N at the back of list L.
 * @param l the list
 * @param n the node
 * @return 0 if N was successfully inserted, 1 otherwise
 */
int list_add_back(struct list *l, struct node *n)
{
    //! if list is NULL or node is NULL.
    if (n == NULL || l == NULL)
    {
        return 1;
    }
    if (l->number_nodes == 0)
    {
        l->head = n;
        l->tail = n;
        l->number_nodes++;
    }
    else
    {
        l->tail->next = n;
        l->tail = n;
        l->number_nodes++;
    }
    n->next = NULL;
    return 0;
}

/**
 * This function gets the value stored in the node.
 * @param n the node
 * @return value of the node or -1 if get value failed
 */
int list_node_get_value(struct node *n)
{
    //! if node is NULL
    if (n == NULL)
    {
        return -1;
    }
    return n->value;
}

/**
 * This function sets the value of the node to the input value.
 * @param n the node
 * @return 0 if value was successfully set, 1 otherwise
 */
int list_node_set_value(struct node *n, int value)
{
    //! if node is NULL or value is negative
    if (n == NULL || value < 0)
    {
        return 1;
    }
    n->value = value;
    return 0;
}

/**
 * This function unlinks the node N from the list L, after unlink the node
 * contains no pointers.
 * @param l the list
 * @param n the node
 * @return 0 if node was successfully unlined, 1 otherwise
 */
int list_unlink_node(struct list *l, struct node *n)
{
    //! return NULL if list is empty or list is NULL or node is NULL or the
    //! node is not in the list.
    if (list_node_present(l, n) != 1)
    {
        return 1;
    }
    if (l->head == n)
    {
        l->head = n->next;
        n->next = NULL;
        n = NULL;
        l->number_nodes--;
        return 0;
    }
    //! node that points to the node N, will point to the next node of node N.
    struct node *previous = list_prev(l, n);
    previous->next = n->next;
    n->next = NULL;
    n = NULL;
    l->number_nodes--;
    return 0;
}

/**
 * This function frees the node
 * @param n the node
 */
void list_free_node(struct node *n)
{
    free(n);
}

/**
 * This function frees the list and the nodes in the list.
 * @param l the list
 * @return 0 if list was successful cleaned up, 1 otherwise
 */
int list_cleanup(struct list *l)
{
    //! if list is NULL
    if (l == NULL)
    {
        return 1;
    }
    struct node *tmp;
    //! free all the nodes
    while (l->head != NULL)
    {
        tmp = l->head;
        l->head = l->head->next;
        list_free_node(tmp);
    }
    free(l);
    return 0;
}

/**
 * This function checks if the list L contains the node N.
 * @param l the list
 * @param n the node
 * @return 1 if node is present in list, 0 if N is not present in list and
 * returns -1 if an error occured.
 */
int list_node_present(struct list *l, struct node *n)
{
    //! if list is NULL, list is empty or node is NULL.
    if (l == NULL || l->number_nodes == 0 || n == NULL)
    {
        return -1;
    }
    //! check if list contains the node
    struct node *tmp = l->head;
    while (tmp != NULL)
    {
        if (tmp == n)
        {
            return 1;
        }
        tmp = tmp->next;
    }
    return 0;
}

/**
 * This function inserts the node N after node M in the link list.
 * @param l the list
 * @param n the node
 * @param m the node
 * @returns 0 if N was successfully inserted, or 1 otherwise
 */
int list_insert_after(struct list *l, struct node *n, struct node *m)
{
    //! if N is not in the list and M is in the list
    if (list_node_present(l, n) == 0 && list_node_present(l, m) == 1)
    {
        //! N points to the next node of M and M points to N.
        n->next = m->next;
        m->next = n;
        l->number_nodes++;
        return 0;
    }
    return 1;
}

/**
 * This function inserts the node N before node M in the link list.
 * @param l the list
 * @param n the node
 * @param m the node
 * @returns 0 if N was successfully inserted, or 1 otherwise
 */
int list_insert_before(struct list *l, struct node *n, struct node *m)
{
    //! if N is not in the list and M is in the list
    if (list_node_present(l, n) == 0 && list_node_present(l, m) == 1)
    {
        if (l->head == m)
        {
            n->next = m;
            l->head = n;
            l->number_nodes++;
            return 0;
        }
        //! node before M points to the N and N points to the M
        struct node *previous = list_prev(l, m);
        if (previous == NULL)
        {
            return 1;
        }
        previous->next = n;
        n->next = m;
        l->number_nodes++;
        return 0;
    }
    return 1;
}

/**
 * This function returns the number of nodes in the list.
 * @param l the list
 * @return number of nodes in the list.
 */
size_t list_length(struct list *l)
{
    //! if list is NULL
    if (l == NULL)
    {
        return 0;
    }
    return l->number_nodes;
}

/**
 * THis function returns a pointer to the i^th node of the list.
 * @param l the list
 * @param i the index
 * @return a pointer to the i^th node
 */
struct node *list_get_ith(struct list *l, size_t i)
{
    //! if list is NULL, list is empty or list does not cointain i nodes.
    if (l == NULL || l->number_nodes == 0 || i > l->number_nodes)
    {
        return NULL;
    }
    struct node *node = l->head;
    size_t index = 0;
    while (node != NULL)
    {
        if (index == i)
        {
            return node;
        }
        index++;
        node = node->next;
    }
    return NULL;
}

/**
 * THis function Cuts list L into 2 lists, with node N being the last node in
 * he first half and all nodes after nodes N are part to the second half, in
 * the same order they were in in the original list.
 * @param l the list
 * @param i the node
 * @return a pointer to the second half list
 */
struct list *list_cut_after(struct list *l, struct node *n)
{
    //! if N is not in the list or N is NULL or List is NULL or list length is
    //! less than 2.
    if (list_node_present(l, n) != 1 || l->number_nodes < 2)
    {
        return NULL;
    }
    //! malloc the second list
    struct list *second_list = list_init();
    //! if the malloc failed
    if (second_list == NULL)
    {
        return NULL;
    }
    second_list->head = n->next;
    second_list->tail = l->tail;
    struct node *count = second_list->head;
    while (count != NULL)
    {
        second_list->number_nodes++;
        count = count->next;
    }
    l->tail = n;
    n->next = NULL;
    return second_list;
}
