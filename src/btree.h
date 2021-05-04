#ifndef BTREE_H
#define BTREE_H

#include <stdlib.h>
#include <stdbool.h>
#include <inttypes.h>
#include <stdio.h>

#define BT(type) type##_bt_t

// Not yet supported!
// #define BT_PTR(type) type##_bt_ptr_t

/** @def DECL_BT(type)
    @param type The type of the binary tree

    Declares a binary tree (BT) type and BT-associated functions.
**/
#define DECL_BT(type) struct type##_bt { \
        type value; \
        struct type##_bt* left; \
        struct type##_bt* right; \
    }; \
    typedef struct type##_bt BT(type); \
    BT(type)* type##_bt_new(type element); \
    size_t type##_bt_size(BT(type)* tree); \
    size_t type##_bt_depth(BT(type)* tree); \
    size_t type##_bt_leaves(BT(type)* tree); \
    BT(type)* type##_bt_connect(BT(type)* left, BT(type)* right, type element); \
    void type##_bt_printf(BT(type)* tree); /* Note: only available if DEF_BT_PRINTF* is used */ \
    void type##_bt_free(BT(type)* tree); \
    bool type##_bt_is_leaf(BT(type)* node); \
    bool type##_bt_is_empty(BT(type)* node); \
    BT(type)* type##_bt_left(BT(type)* node); \
    BT(type)* type##_bt_right(BT(type)* node); \
    BT(type)* type##_bt_get(BT(type)* tree, uintmax_t address); \
    BT(type)* type##_bt_prefix_find(BT(type)* tree, bool (*predicate)(const type, const void*), const void* predicate_data); \
    BT(type)* type##_bt_postfix_find(BT(type)* tree, bool (*predicate)(const type, const void*), const void* predicate_data); \
    BT(type)* type##_bt_clone(BT(type)* node);

/** @def DEF_BT(type)
    @param type The type of the binary tree

    Defines the BT-associated functions.
**/
#define DEF_BT(type) \
    BT(type)* type##_bt_new(type element) { \
        BT(type)* res = (BT(type)*)malloc(sizeof(struct type##_bt)); \
        res->value = element; \
        res->left = NULL; \
        res->right = NULL; \
        return res; \
    } \
    size_t type##_bt_size(BT(type)* tree) { \
        if (tree == NULL) return 0; \
        return 1 + type##_bt_size(tree->left) + type##_bt_size(tree->right); \
    } \
    size_t type##_bt_depth(BT(type)* tree) { \
        if (tree == NULL) return 0; \
        size_t left = type##_bt_depth(tree->left); \
        size_t right = type##_bt_depth(tree->right); \
        if (left > right) return left + 1; \
        else return right + 1; \
    } \
    size_t type##_bt_leaves(BT(type)* tree) { \
        if (tree == NULL) return 0; \
        if (tree->left == NULL && tree->right == NULL) return 1; \
        else return type##_bt_leaves(tree->left) + type##_bt_leaves(tree->right); \
    } \
    BT(type)* type##_bt_connect(BT(type)* left, BT(type)* right, type element) { \
        BT(type)* res = type##_bt_new(element); \
        res->left = left; \
        res->right = right; \
        return res; \
    } \
    void type##_bt_free(BT(type)* tree) { \
        if (tree == NULL) return; \
        type##_bt_free(tree->left); \
        type##_bt_free(tree->right); \
        free(tree); \
    } \
    bool type##_bt_is_leaf(BT(type)* node) { \
        if (node == NULL) return false; \
        else return node->left == NULL && node->right == NULL; \
    } \
    bool type##_bt_is_empty(BT(type)* node) { \
        return node == NULL; \
    } \
    BT(type)* type##_bt_get(BT(type)* tree, uintmax_t address) { \
        if (tree == NULL) return NULL; \
        else if (address % 2) { \
            if (tree->right == NULL) return tree; \
            else return type##_bt_get(tree->right, address >> 1); \
        } else { \
            if (tree->left == NULL) return tree; \
            else return type##_bt_get(tree->left, address >> 1); \
        } \
    } \
    BT(type)* type##_bt_prefix_find(BT(type)* tree, bool (*predicate)(const type, const void*), const void* predicate_data) { \
        if (tree == NULL) return NULL; \
        if (predicate(tree->value, predicate_data)) return tree; \
        BT(type)* left = type##_bt_prefix_find(tree->left, predicate, predicate_data); \
        if (left != NULL) return left; \
        BT(type)* right = type##_bt_prefix_find(tree->right, predicate, predicate_data); \
        return right; \
    } \
    BT(type)* type##_bt_postfix_find(BT(type)* tree, bool (*predicate)(const type, const void*), const void* predicate_data) { \
        if (tree == NULL) return NULL; \
        BT(type)* left = type##_bt_postfix_find(tree->left, predicate, predicate_data); \
        if (left != NULL) return left; \
        BT(type)* right = type##_bt_postfix_find(tree->right, predicate, predicate_data); \
        if (right != NULL) return right; \
        if (predicate(tree->value, predicate_data)) return tree; \
        else return NULL; \
    } \
    BT(type)* type##_bt_left(BT(type)* node) { \
        if (node == NULL) return NULL; \
        return node->left; \
    } \
    BT(type)* type##_bt_right(BT(type)* node) { \
        if (node == NULL) return NULL; \
        return node->right; \
    } \
    BT(type)* type##_bt_clone(BT(type)* node) { \
        if (node == NULL) return NULL; \
        return type##_bt_connect(type##_bt_clone(node->left), type##_bt_clone(node->right), node->value); \
    }

/**
    @def DEF_BT_PRINTF_CUSTOM
    @param type The type of the elements in the binary tree
    @param printf_callback A block of code that prints a value of the binary tree. The value can be accessed through the symbol `value`.

    Defines the `TYPE_bt_printf` function, printing the values of the binary tree *by executing* `printf_callback`.

    ## Example

    ```
    DEF_BT_PRINTF_CUSTOM(char, printf("'%c'", value));
    ```
**/
#define DEF_BT_PRINTF_CUSTOM(type, printf_callback) \
    void type##_bt_printf_rec(BT(type)* tree) { \
        if (tree == NULL) printf("()"); \
        else if (tree->left == NULL && tree->right == NULL) { \
            printf("("); \
            type value = tree->value; \
            printf_callback; \
            printf(")"); \
        } else { \
            printf("("); \
            type##_bt_printf_rec(tree->left); \
            printf(" <- "); \
            type value = tree->value; \
            printf_callback; \
            printf(" -> "); \
            type##_bt_printf_rec(tree->right); \
            printf(")"); \
        } \
    } \
    void type##_bt_printf(BT(type)* tree) { \
        printf("BinaryTree<" #type "> "); \
        type##_bt_printf_rec(tree); \
        printf("\n"); \
    }

/**
    @def DEF_BT_PRINTF_FN
    @param type The type of the elements in the binary tree
    @param printf_callback A function to be called for each value of the binary tree and that prints that value. Must take as only argument `type` and may be of any return type.

    Defines the `TYPE_bt_printf` function, printing the values of the binary tree *by calling* `printf_callback`.

    ## Example

    ```
    void print_my_char(char c) {
        printf("'%c'", c);
    }

    DEF_BT_PRINTF_FN(char, print_my_char);
    ```
**/
#define DEF_BT_PRINTF_FN(type, printf_callback) \
    DEF_BT_PRINTF_CUSTOM(type, orintf_callback(value))

/**
    @def DEF_BT_PRINTF
    @param type The type of the elements in the binary tree
    @param printf_format A format string for printf

    Defines the `TYPE_bt_printf` function, printing the values of the binary tree using `printf_format` *as printf format*.

    ## Example

    ```
    DEF_BT_PRINTF(char, "'%c'");
    ```
**/
#define DEF_BT_PRINTF(type, printf_format) \
    DEF_BT_PRINTF_CUSTOM(type, printf(printf_format, value))

/** @struct TYPE_bt

    A binary tree node with a value of type `TYPE`
**/

/** @typedef TYPE_bt_t

    Resolves to `struct TYPE_bt`; can also be accessed with BT(TYPE)
**/

/** @def BT(TYPE)

    Resolves to `struct TYPE_bt`;
**/

/** @fn TYPE_bt_new(TYPE element)
    @param element The value to put in the root node

    Creates a new tree with only one node, containing `element`.
**/

/** @fn TYPE_bt_free(BT(TYPE)* tree)
    @param tree The tree to free

    Frees all of the memory used by `tree` and its child's memory.
**/

/** @fn TYPE_bt_size(BT(TYPE)* tree)
    @returns size_t
    @param tree The tree whose size is to measure

    Recursively measures the size (number of nodes) of a tree.
**/

/** @fn TYPE_bt_depth(BT(TYPE)* tree)
    @returns size_t
    @param tree The tree whose depth is to measure

    Recursively measures the depth (length of the longest branch) of a tree.
**/

/** @fn TYPE_bt_leaves(BT(TYPE)* tree)
    @returns size_t
    @param tree The tree whose number of leaves is to measure

    Recursively measures the number of leaves in a tree.
**/

/** @fn TYPE_bt_printf(BT(TYPE)* tree)
    @param tree The tree to print out

    Prints a tree; useful for debugging.

    **Note:** only available if DEF_BT_PRINTF* is used.
**/

/** @fn TYPE_bt_connect(BT(TYPE)* left, BT(TYPE)* right, TYPE element)
    @returns BT(TYPE)*
    @param left The left branch of the tree; NULL for no branch
    @param right The right branch of the tree; NULL for no branch
    @param element The value to put in the root node

    Creates a tree out of up to two sub-trees. Will return a new node, whose value is `element`, whose left child is `left` and whose right child is `right`.
**/

/** @fn TYPE_bt_is_leaf(BT(TYPE)* node)
    @returns bool
    @param tree The node to test against leafiness

    Returns whether or not the given node is a leaf
**/

/** @fn TYPE_bt_is_empty(BT(TYPE)* node)
    @returns bool
    @param tree The node to test against emptiness

    Returns whether or not the given node is a an empty tree (ie. NULL)
**/

/** @fn TYPE_bt_get(BT(TYPE)* tree, intmax_t address)
    @returns TYPE*
    @param tree The tree to get the element from
    @param address The address of the element; read from lowest bit to highest bit; `0` means left and `1` means right

    Returns the element at the given address.
    The algorithm recursively chooses its path based on address; the n-th bit (0 = lowest bit) determines which path should be taken at the n-th depth of the tree, where `0` will make it go left and `1` will make it go right.
    Once the path ends, the last accessed element is returned.
    Returns NULL if no leaf was found (ie. the tree is empty).
**/

#endif // BTREE_H
