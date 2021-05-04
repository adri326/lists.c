#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

#define LL(type) type##_ll_t
#define LL_PTR(type) type##_ll_ptr_t

/** @def DECL_LL(type)
    @param type The type of linked list

    Declares a linked list (LL) type and LL-associated functions.
**/
#define DECL_LL(type) struct type##_ll { \
        type value; \
        struct type##_ll* next; \
    }; \
    typedef struct type##_ll LL(type); \
    /* Here we go: */ \
    LL(type)* type##_ll_new(type element); \
    size_t type##_ll_length(LL(type)* list); \
    type* type##_ll_head(LL(type)* list); \
    type* type##_ll_tail(LL(type)* list); \
    type* type##_ll_get(LL(type)* list, size_t index); \
    LL(type)* type##_ll_next(LL(type)* list); \
    bool type##_ll_is_empty(LL(type)* list); \
    LL(type)* type##_ll_push_tail(LL(type)* list, type element); \
    LL(type)* type##_ll_push_head(LL(type)* list, type element); \
    LL(type)* type##_ll_concat(LL(type)* list_a, LL(type)* list_b); \
    void type##_ll_printf(LL(type)* list); /* Note: only available if DEF_LL_PRINTF* has been called */ \
    void type##_ll_free(LL(type)* list); \
    LL(type)* type##_ll_clone(LL(type)* list);

/** @def DECL_LL_PTR(type)
    @param type The type of linked list

    Declares a linked list (LL) type holding pointers to values of type `type` and LL-associated functions.
**/
#define DECL_LL_PTR(type) struct type##_ll_ptr { \
        type* value; \
        struct type##_ll_ptr* next; \
    }; \
    typedef struct type##_ll_ptr LL_PTR(type);

/** @def DEF_LL(type)
    @param type The type of the linked list

    Defines the LL-associated functions
**/
#define DEF_LL(type) \
    LL(type)* type##_ll_new(type element) { \
        LL(type)* res = (LL(type)*)malloc(sizeof(struct type##_ll)); \
        res->value = element; \
        res->next = NULL; \
        return res; \
    } \
    size_t type##_ll_length(LL(type)* list) { \
        if (list == NULL) return 0; \
        size_t res = 1; \
        while (list->next != NULL) { \
            list = list->next; \
            res++; \
        } \
        return res; \
    } \
    type* type##_ll_head(LL(type)* list) { \
        if (list == NULL) return NULL; \
        else return &list->value; \
    } \
    type* type##_ll_tail(LL(type)* list) { \
         if (list == NULL) return NULL; \
         while (list->next != NULL) list = list->next; \
         return &list->value; \
    } \
    type* type##_ll_get(LL(type)* list, size_t index) { \
         if (list == NULL) return NULL; \
         while (list->next != NULL && index != 0) { \
             list = list->next; \
             index--; \
         } \
         if (index > 0) return NULL; \
         return &list->value; \
    } \
    LL(type)* type##_ll_push_tail(LL(type)* list, type element) { \
        if (list == NULL) return type##_ll_new(element); \
        LL(type)* res = list; \
        while (list->next != NULL) list = list->next; \
        list = list->next = (LL(type)*)malloc(sizeof(struct type##_ll)); \
        list->value = element; \
        list->next = NULL; \
        return res; \
    } \
    LL(type)* type##_ll_concat(LL(type)* list_a, LL(type)* list_b) { \
        if (list_a == NULL) return list_b; \
        LL(type)* res = list_a; \
        while (list_a->next != NULL) list_a = list_a->next; \
        list_a->next = list_b; \
        return res; \
    } \
    LL(type)* type##_ll_push_head(LL(type)* list, type element) { \
        LL(type)* res = (LL(type)*)malloc(sizeof(struct type##_ll)); \
        res->value = element; \
        res->next = list; \
        return res; \
    } \
    LL(type)* type##_ll_pop_tail(LL(type)* list) { \
        if (list == NULL) return NULL; \
        else if (list->next == NULL) { \
            free(list); \
            return NULL; \
        } \
        LL(type)* res = list; \
        while (res->next->next != NULL) res = res->next; \
        free(res->next); \
        res->next = NULL; \
        return list; \
    } \
    LL(type)* type##_ll_pop_head(LL(type)* list) { \
        if (list == NULL) return NULL; \
        return list->next; \
    } \
    void type##_ll_free(LL(type)* list) { \
        if (list == NULL) return; \
        LL(type)* next; \
        do { \
            next = list->next; \
            free(list); \
            list = next; \
        } while (next != NULL); \
    } \
    LL(type)* type##_ll_next(LL(type)* list) { \
        if (list == NULL) return NULL; \
        else return list->next; \
    } \
    bool type##_ll_is_empty(LL(type)* list) { \
        return list == NULL; \
    } \
    LL(type)* type##_ll_clone(LL(type)* list) { \
        if (list == NULL) return NULL; \
        return type##_ll_push_head(type##_ll_clone(list->next), list->value); \
    }

/**
    @def DEF_LL_PRINTF_CUSTOM
    @param type The type of the elements in the linked list
    @param printf_callback A block of code that prints a value of the linked list. The value can be accessed through the symbol `value`.

    Defines the `TYPE_ll_printf` function, printing the values of the linked list *by executing* `printf_callback`.

    ## Example

    ```
    DEF_LL_PRINTF_CUSTOM(char, printf("'%c'", value));
    ```
**/
#define DEF_LL_PRINTF_CUSTOM(type, printf_callback) \
    void type##_ll_printf(LL(type)* list) { \
        printf("LinkedList<" #type "> ["); \
        while (true) { \
            type value = list->value; \
            printf_callback; \
            if (list->next != NULL) { \
                printf(", "); \
                list = list->next; \
            } \
            else { \
                printf("]\n"); \
                break; \
            } \
        } \
    } \

/**
    @def DEF_LL_PRINTF_FN
    @param type The type of the elements in the linked list
    @param printf_callback A function to be called for each value of the linked list and that prints that value. Must take as only argument `type` and may be of any return type.

    Defines the `TYPE_ll_printf` function, printing the values of the linked list *by calling* `printf_callback`.

    ## Example

    ```
    void print_my_char(char c) {
        printf("'%c'", c);
    }

    DEF_LL_PRINTF_FN(char, print_my_char);
    ```
**/
#define DEF_LL_PRINTF_FN(type, printf_callback) \
    DEF_LL_PRINTF(type, printf_callback(value))

/**
    @def DEF_LL_PRINTF
    @param type The type of the elements in the linked list
    @param printf_format A format string for printf

    Defines the `TYPE_ll_printf` function, printing the values of the linked list using `printf_format` *as printf format*.

    ## Example

    ```
    DEF_LL_PRINTF(char, "'%c'");
    ```
**/
#define DEF_LL_PRINTF(type, printf_format) \
    DEF_LL_PRINTF_CUSTOM(type, printf(printf_format, value))

DECL_LL_PTR(void)

LL_PTR(void)* void_ll_ptr_new(void* element);
#define LL_PTR_NEW(type, element) ((LL_PTR(type)*)void_ll_ptr_new((void*)(element)))

void void_ll_ptr_push_tail(LL_PTR(void)* list, void* element);
#define LL_PTR_PUSH_TAIL(type, list, element) void_ll_ptr_push_tail((LL_PTR(void)*)(list), (void*)(element))

LL_PTR(void)* void_ll_ptr_push_head(LL_PTR(void)* list, void* element);
#define LL_PTR_PUSH_HEAD(type, list, element) ((LL_PTR(type)*)void_ll_ptr_push_head((LL_PTR(void)*)(list), (void*)(element)))

void void_ll_ptr_printf(LL_PTR(void)* list);
#define LL_PTR_PRINTF(type, list, print) { \
    LL_PTR(type)* list_ptr = list; \
    printf("LinkedList<" #type "*> ["); \
    while (list_ptr != NULL) { \
        type* element = list_ptr->value; \
        print; \
        list_ptr = list_ptr->next; \
        if (list_ptr == NULL) { \
            printf("]\n"); \
        } else { \
            printf(", "); \
        } \
    } \
}

void void_ll_ptr_free(LL_PTR(void)* list);
#define LL_PTR_FREE(type, list, element_free) { \
    LL_PTR(type)* list_ptr = list; \
    while (list_ptr != NULL) { \
        LL_PTR(type)* next = list_ptr->next; \
        type* element = list_ptr->value; \
        element_free; \
        free(list_ptr); \
        list_ptr = next; \
    } \
}

#define LL_PTR_FREE_AUTO(type, list) { \
    LL_PTR(type)* list_ptr = list; \
    while (list_ptr != NULL) { \
        LL_PTR(type)* next = list_ptr->next; \
        free(list_ptr); \
        list_ptr = next; \
    } \
}

void* void_ll_ptr_head(LL_PTR(void)* list);
#define LL_PTR_HEAD(type, list) (type*)void_ll_ptr_head((LL_PTR(void)*)(list))

void* void_ll_ptr_tail(LL_PTR(void)* list);
#define LL_PTR_TAIL(type, list) (type*)void_ll_ptr_tail((LL_PTR(void)*)(list))

void* void_ll_ptr_get(LL_PTR(void)* list, size_t index);
#define LL_PTR_GET(type, list, index) (type*)void_ll_ptr_get((LL_PTR(void)*)(list), (size_t)(index))

size_t void_ll_ptr_length(LL_PTR(void)* list);
#define LL_PTR_LENGTH(type, list) void_ll_ptr_length((LL_PTR(void)*)(list))

#endif // LINKEDLIST_H
