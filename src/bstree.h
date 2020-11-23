#ifndef BSTREE_H
#define BSTREE_H

#include <stdlib.h>
#include <stdbool.h>
#include "linkedlist.h"

#define BST(type) type##_bst_t

// requires the user to have defined BT(type) beforehand and that the ordinality operators can be applied on type

#define DECL_BST(type) typedef struct type##_bt BST(type); \
    BST(type)* type##_bst_insert(BST(type)* tree, type value);

// requires the user to have defined LL(type) beforehand
#define DECL_BST_LL(type) \
    BST(type)* type##_bst_from_ll(LL(type)* list); \
    LL(type)* type##_bst_into_ll(BST(type)* tree); \
    LL(type)* type##_bst_sort_ll(LL(type)* list);

#define DECL_BST_SOURCES(type, printf_format) \
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
#define DECL_BST_LL_SOURCES(type, printf_format) \
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

#endif // BSTREE_H
