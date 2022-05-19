#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tree.h"

/**
 * Binary search tree.
 * @param root the root of the tree
 */
struct tree
{
    struct node *root;
};

/**
 * Node that contains the data,left child and right child.
 */
struct node
{
    int data;
    struct node *lhs;
    struct node *rhs;
};
typedef struct node node;

/* Unique id's for numbering nodes in dot format. */
static int global_node_counter = 0;

/* Helper function: Allocate a new tree node and initialise it with
 * the given parameters. Return a pointer to the new node or NULL on
 * failure. */
static node *make_node(int data)
{
    //! if malloc failed return NULL
    struct node *new_node = malloc(sizeof(struct node));
    if (new_node == NULL)
    {
        return NULL;
    }
    new_node->data = data;
    new_node->lhs = NULL;
    new_node->rhs = NULL;
    return new_node;
}

static int print_tree_dot_r(node *root, FILE *dotf)
{
    int left_id, right_id, my_id = global_node_counter++;

    if (root == NULL)
    {
        fprintf(dotf, "    %d [shape=point];\n", my_id);
        return my_id;
    }

    fprintf(dotf, "    %d [color=%s label=\"%d\"]\n", my_id, "black", root->data);

    left_id = print_tree_dot_r(root->lhs, dotf);
    fprintf(dotf, "    %d -> %d [label=\"l\"]\n", my_id, left_id);

    right_id = print_tree_dot_r(root->rhs, dotf);
    fprintf(dotf, "    %d -> %d [label=\"r\"]\n", my_id, right_id);

    return my_id;
}

void tree_dot(struct tree *tree, char *filename)
{
    node *root = tree->root;
    global_node_counter = 0;
    FILE *dotf = fopen(filename, "w");
    if (!dotf)
    {
        printf("error opening file: %s\n", filename);
        exit(1);
    }
    fprintf(dotf, "digraph {\n");
    if (root)
    {
        print_tree_dot_r(root, dotf);
    }
    fprintf(dotf, "}\n");
    fclose(dotf);
}

/**
 * This function points to the parent of the node that containst the given data.
 *
 * @param tree input tree
 * @param data input data
 * @return the parent of the node
 */
struct node *previous(struct tree *tree, int data)
{
    //! begins with the root
    struct node *tmp = tree->root;
    while (tmp != NULL)
    {
        //! if data is less than the node, search left node
        if (data < tmp->data)
        {
            //! return the node if the node contains the given data
            if (tmp->lhs->data == data)
            {
                return tmp;
            }
            else
            {
                tmp = tmp->lhs;
            }
        }
        //! if data is less than the node, search right node
        else if (data > tmp->data)
        {
            //! return the node if the node contains the given data
            if (tmp->rhs->data == data)
            {
                return tmp;
            }
            else
            {
                tmp = tmp->rhs;
            }
        }
        else
            break;
    }
    return NULL;
}

/**
 * This function deletes the root of the given tree.
 *
 * @param tree input tree
 */
void delete_root(struct tree *tree)
{
    struct node *tmp = tree->root;
    //! if root has no child, free the node and root points to NULL
    if (tmp->lhs == NULL && tmp->rhs == NULL)
    {
        tree->root = NULL;
        free(tmp);
    }
    //! if root has only right child
    else if (tmp->lhs == NULL && tmp->rhs != NULL)
    {
        //! free the node and let right child be the root
        tree->root = tmp->rhs;
        tmp->rhs = NULL;
        free(tmp);
    }
    //! if root has only left child
    else if (tmp->rhs == NULL && tmp->lhs != NULL)
    {
        //! free the node and let left child be the root
        tree->root = tmp->lhs;
        tmp->lhs = NULL;
        free(tmp);
    }
    //! else root has two childs
    else
    {
        //! else the swap node is the most right child of the left subtree
        struct node *swap_node = tmp->lhs;
        while (swap_node->rhs != NULL)
        {
            swap_node = swap_node->rhs;
        }
        //! parent of the swap node
        struct node *parent = previous(tree, swap_node->data);
        if (swap_node->lhs == NULL)
        {
            //! parent of swap node points to NULL if swap node has no child
            parent->rhs = NULL;
        }
        else
        {
            //! else points to the left child
            parent->rhs = swap_node->lhs;
        }
        //! replace the node that has to be deleted with swap node
        swap_node->lhs = tmp->lhs;
        swap_node->rhs = tmp->rhs;
        tree->root = swap_node;
        tmp->lhs = NULL;
        tmp->lhs = NULL;
        //! free the node that has to be deleted
        free(tmp);
    }
}

/**
 * This function deletes the node that containst the data.
 *
 * @param tree input tree
 */
void delete_node(struct tree *tree, int data)
{
    //! the parent of the node that has to be deleted
    struct node *parent = previous(tree, data);
    struct node *delete;
    int i = 0;
    //! if i = 0 it means the left child of the parent has to deleted
    if (parent->lhs != NULL && parent->lhs->data == data)
    {
        delete = parent->lhs;
    }
    else
    //! if i = 1 it means the right child of the parent has to deleted
    {
        delete = parent->rhs;
        i = 1;
    }
    //! if the node that has to be deleted has no child
    if (delete->lhs == NULL && delete->rhs == NULL)
    {
        //! if left child has to be deleted, left child points to NULL
        if (i == 0)
        {
            parent->lhs = NULL;
        }
        //! else right child has to be deleted, right child points to NULL
        else
        {
            parent->rhs = NULL;
        }
    }
    //! if the node that has to be deleted has only right child
    else if (delete->lhs == NULL && delete->rhs != NULL)
    {
        //! if left child has to be deleted
        if (i == 0)
        {
            //! left child of parent points to the right child of the child
            parent->lhs = delete->rhs;
        }
        //! else right child has to be deleted
        else
        {
            //! right child of parent points to the right child of the child
            parent->rhs = delete->rhs;
        }
        delete->rhs = NULL;
    }
    //! if the node that has to be deleted has only left child
    else if (delete->rhs == NULL && delete->lhs != NULL)
    {
        //! if left child has to be deleted
        if (i == 0)
        {
            //! left child of parent points to the left child of the child
            parent->lhs = delete->lhs;
        }
        //! else right child has to be deleted
        else
        {
            //! right child of parent points to the right child of the child
            parent->rhs = delete->lhs;
        }
        delete->lhs = NULL;
    }
    //! else node that has to be deleted has two childs
    else
    {
        //! select the most right child of the left subtree
        struct node *swap = delete->lhs;
        while (swap->rhs != NULL)
        {
            swap = swap->rhs;
        }
        //! parent of the swap node
        struct node *swap_parent = previous(tree, swap->data);
        //! parent of swap node points to NULL if swap node has no child
        if (swap->lhs == NULL)
        {
            swap_parent->rhs = NULL;
        }
        //! else points to the left child
        else
        {
            swap_parent->rhs = swap->lhs;
        }
        //! replace the node that has to be deleted with swap node
        swap->lhs = delete->lhs;
        swap->rhs = delete->rhs;
        delete->lhs = NULL;
        delete->rhs = NULL;
        //! if left child of parent has to be deleted
        if (i == 0)
        {
            //! left child of parent points to the swap node
            parent->lhs = swap;
        }
        else
        {
            //! else right child of parent points to the swap node
            parent->rhs = swap;
        }
    }
    //! free the node that has to be deleted
    free(delete);
}

/**
 * This function print the data of the input node and nodes that belong to that
 * subtree in sorted order to stdout.
 *
 * @param node
 */
void node_print(struct node *node)
{
    if (node != NULL)
    {
        node_print(node->lhs);
        printf("%d\n", node->data);
        node_print(node->rhs);
    }
}
/**
 * This function free the input node and the nodes belong to that subtree
 *
 * @param node
 */
void node_cleanup(struct node *node)
{
    //! just free the node if it has no child
    if (node->lhs == NULL && node->rhs == NULL)
    {
        free(node);
    }
    //! free the node and node belong to that subtree
    else if (node->lhs == NULL && node->rhs != NULL)
    {
        node_cleanup(node->rhs);
        free(node);
    }
    //! free the node and node belong to that subtree
    else if (node->rhs == NULL && node->lhs != NULL)
    {
        node_cleanup(node->lhs);
        free(node);
    }
    //! free the node and node belong to that subtree
    else
    {
        node_cleanup(node->lhs);
        node_cleanup(node->rhs);
        free(node);
    }
}

int tree_check(struct tree *tree)
{
    return 0;
}

/* Return a pointer to a new tree. Return NULL on failure.
 * If the turbo flag is 0 the tree is initialized as a regular
 * binary search tree. If the turbo flag is 1 the tree is initialized
 * as a fast balanced binary search tree. */
struct tree *tree_init(int turbo)
{
    if (turbo == 0)
    {
        //! return NULL if malloc failed
        struct tree *new_tree = malloc(sizeof(struct tree));
        if (new_tree == NULL)
        {
            return NULL;
        }
        new_tree->root = NULL;
        //! return the new tree
        return new_tree;
    }
    return NULL;
}

/* Find data in the tree. Return 1 if the tree contains the data or
 * 0 if the data is not found. */
int tree_find(struct tree *tree, int data)
{
    //! if tree is NULL or the root is NULL
    if (tree == NULL || tree->root == NULL)
    {
        return 0;
    }
    struct node *tmp = tree->root;
    //! search the node that contains the input data
    while (tmp != NULL)
    {
        if (tmp->data == data)
        {
            return 1;
        }
        else if (data < tmp->data)
        {
            tmp = tmp->lhs;
        }
        else
        {
            tmp = tmp->rhs;
        }
    }
    return 0;
}

/* Insert data into the tree. Return 0 if successful and -1 for
 * failure. If the data is already in the tree it remains unchanged
 * and 1 is returned. */
int tree_insert(struct tree *tree, int data)
{
    //! if tree is NULL
    if (tree == NULL)
    {
        return -1;
    }
    //! if the the tree already containst the data
    else if (tree_find(tree, data) == 1)
    {
        return 1;
    }
    //! make new node, return -1 on failure
    struct node *new_node = make_node(data);
    if (new_node == NULL)
    {
        return -1;
    }
    //! if the tree is empty, new node is the root
    if (tree->root == NULL)
    {
        tree->root = new_node;
        return 0;
    }
    struct node *tmp = tree->root;
    //! find the right position to place the new node
    while (1)
    {
        //! search to left if data is less than the node
        if (data < tmp->data)
        {
            //! place the node if left child is NULL
            if (tmp->lhs == NULL)
            {
                tmp->lhs = new_node;
                break;
            }
            else
            {
                tmp = tmp->lhs;
            }
        }
        //! else search to right
        else
        {
            //! place the node if right child is NULL
            if (tmp->rhs == NULL)
            {
                tmp->rhs = new_node;
                break;
            }
            else
            {
                tmp = tmp->rhs;
            }
        }
    }
    return 0;
}

/* Remove data from the tree. Return 0 if successful.  If the data is
 * not found 1 is returned. */
int tree_remove(struct tree *tree, int data)
{
    //! if tree is NULL
    if (tree == NULL)
    {
        return 1;
    }
    //! if data is not found return 1
    else if (tree_find(tree, data) == 0)
    {
        return 1;
    }
    //! if root has to be removed, remove the root
    if (tree->root->data == data)
    {
        delete_root(tree);
    }
    //! else remove the node
    else
    {
        delete_node(tree, data);
    }
    return 0;
}

/* Print the contents of the tree in sorted order to stdout. Print one
 * item per line. */
void tree_print(struct tree *tree)
{
    node_print(tree->root);
}

/* Free all memory associated with the tree. */
void tree_cleanup(struct tree *tree)
{
    if (tree != NULL)
    {
        if (tree->root != NULL)
        {
            node_cleanup(tree->root);
        }
        free(tree);
    }
}
