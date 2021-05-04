#ifndef BSTREE_H
#define BSTREE_H

#include <stdlib.h>
#include <stdbool.h>
#include "linkedlist.h"

#define BST(type) type##_bst_t

/**
    @def DECL_BST(type)
    @param type The type of the BST

    Declares a binary search tree (BST) type and the BST-associated functions.
    This macro requires `DECL_BT(type)` to have been called beforehand.

    The new `BST(type)` type is effectively a typedef on `struct TYPE_bt`, and thus it can be used interchangeably with `BT(type)` and BT-associated functions.
**/
#define DECL_BST(type) typedef struct type##_bt BST(type); \
    BST(type)* type##_bst_insert(BST(type)* tree, type value);

/**
    @def DECL_BST_LL
    @param type The type of the BST

    Declares the BST-associated functions that allow the usage of BSTs with linked lists of `type` (LL).
    This macro requires `DECL_BT(type)`, `DECL_BST(type)` and `DECL_LL(type)` to have been called beforehand.
**/
#define DECL_BST_LL(type) \
    BST(type)* type##_bst_from_ll(LL(type)* list); \
    LL(type)* type##_bst_into_ll(BST(type)* tree); \
    LL(type)* type##_bst_sort_ll(LL(type)* list);

/**
    @def DEF_BST
    @param type The type of the BST

    Defines the BST-associated functions.
    Requires `DECL_BT(type)`, `DECL_BST(type)` and `DEF_BT(type)` to have been called beforehand.
**/
#define DEF_BST(type) \
    BST(type)* type##_bst_insert(BST(type)* tree, type value) { \
        if (tree == NULL) return type##_bt_new(value); \
        else { \
            if (value < tree->value) { \
                tree->left = type##_bst_insert(tree->left, value); \
            } else { \
                tree->right = type##_bst_insert(tree->right, value); \
            } \
            return tree; \
        } \
    }

/**
    @def DEF_BST
    @param type The type of the BST

    Defines the BST-associated functions that allow conversion to/from linked lists (LL).
    Requires `DECL_BT(type)`, `DECL_BST(type)`, `DECL_LL(type)`, `DECL_BST_LL(type)`, `DEF_BT(type)` and `DEF_LL(type)` to have been called beforehand.
**/
#define DEF_BST_LL(type) \
    BST(type)* type##_bst_from_ll(LL(type)* list) { \
        BST(type)* res = NULL; \
        while (!type##_ll_is_empty(list)) { \
            res = type##_bst_insert(res, *type##_ll_head(list)); \
            list = type##_ll_next(list); \
        } \
        return res; \
    } \
    LL(type)* type##_bst_into_ll(BST(type)* tree) { \
        if (tree == NULL) return NULL; \
        LL(type)* res = type##_bst_into_ll(tree->left); \
        res = type##_ll_push_tail(res, tree->value); \
        res = type##_ll_concat(res, type##_bst_into_ll(tree->right)); \
        return res; \
    } \
    LL(type)* type##_bst_sort_ll(LL(type)* list) { \
        BST(type)* bst = type##_bst_from_ll(list); \
        LL(type)* res = type##_bst_into_ll(bst); \
        type##_bt_free(bst); \
        return res; \
    }

/** @typedef TYPE_bst_t

    Resolves to `struct TYPE_bt`; can also be accessed with `BST(TYPE)`.
**/

/** @def BST(TYPE)

    Resolves to `struct TYPE_bt`.
**/

/** @fn TYPE_bst_insert(BST(TYPE)* tree, TYPE value)
    @returns A pointer to the new, modified tree

    Inserts a value of type `TYPE` into a binary search tree, inserting it such that for any node, `left(node)->value ≤ node->value ≤ right(node)->value`.

    Does not self-balance.

    Behavior is undefined if the BST has been modified to contain a closed loop or if the node ordering invariant has not been respected.
**/

/** @fn TYPE_bst_from_ll(LL(TYPE)* list)
    @returns A pointer to the new BST

    Turns a linked list of type `LL(TYPE)*` into a binary search tree, by repeatidly inserting its elements.

    (Currently) does not self-balance.

    Behavior is undefined if the linked list has been modified to contain a closed loop.

    **Note:** only available if `DECL_BST_LL(TYPE)` and `DEF_BST_LL(TYPE)` have been called.
**/

/** @fn TYPE_bst_into_ll(BST(TYPE)* bst)
    @returns A pointer to the new, sorted linked list

    Turns a binary search tree into a sorted linked list.

    Behavior is undefined if the binary search tree has been altered to contain a closed loop or if the node ordering invariant has not been respected.

    **Note:** only available if `DECL_BST_LL(TYPE)` and `DEF_BST_LL(TYPE)` have been called.
**/

/** @fn TYPE_bst_sort_ll(LL(TYPE)* list)
    @returns A sorted version of `list`

    Calls `tree = TYPE_bst_from_ll(list)` and `TYPE_bst_into_ll(tree)`.

    Behavior is undefined if `list` contains a closed loop.

    Average time complexity of this sorting method is `O(logn)`

    **Note:** only available if `DECL_BST_LL(TYPE)` and `DEF_BST_LL(TYPE)` have been called.
**/

#endif // BSTREE_H
