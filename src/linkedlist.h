#ifndef ARRAY_H
#define ARRAY_H

#include <stdlib.h>
#include <stdbool.h>

#define LL(type) type##_ll_t
#define LL_PTR(type) type##_ll_ptr_t

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
    LL(type)* type##_ll_push_tail(LL(type)* list, type element); \
    LL(type)* type##_ll_push_head(LL(type)* list, type element); \
    void type##_ll_printf(LL(type)* list); \
    void type##_ll_free(LL(type)* list);

#define DECL_LL_PTR(type) struct type##_ll_ptr { \
        type* value; \
        struct type##_ll_ptr* next; \
    }; \
    typedef struct type##_ll_ptr LL_PTR(type);

#define DECL_LL_SOURCES(type, printer) \
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
    LL(type)* type##_ll_push_head(LL(type)* list, type element) { \
        LL(type)* res = (LL(type)*)malloc(sizeof(struct type##_ll)); \
        res->value = element; \
        res->next = list; \
        return res; \
    } \
    LL(type)* type##_ll_pop_tail(LL(type)* list) { \
        if (list == NULL) return NULL; \
        else if (list->next == NULL) return NULL; \
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
    void type##_ll_printf(LL(type)* list) { \
        printf("LinkedList<" #type "> ["); \
        while (true) { \
            printf(printer, list->value); \
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
    void type##_ll_free(LL(type)* list) { \
        if (list == NULL) return; \
        LL(type)* next; \
        do { \
            next = list->next; \
            free(list); \
            list = next; \
        } while (next != NULL); \
    }

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

#define LL_PTR_FREE2(type, list) { \
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

#endif // ARRAY_H
