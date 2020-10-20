#include <stdlib.h>
#include <stdio.h>
#include "linkedlist.h"

LL_PTR(void)* void_ll_ptr_new(void* element) {
    LL_PTR(void)* res = (LL_PTR(void)*)malloc(sizeof(struct void_ll_ptr));
    res->value = element;
    res->next = NULL;
    return res;
}

void void_ll_ptr_push_tail(LL_PTR(void)* list, void* element) {
    if (list == NULL) return;
    while (list->next != NULL) list = list->next;
    list = list->next = (LL_PTR(void)*)malloc(sizeof(struct void_ll_ptr));
    list->value = element;
    list->next = NULL;
}

LL_PTR(void)* void_ll_ptr_push_head(LL_PTR(void)* list, void* element) {
    LL_PTR(void)* res = (LL_PTR(void)*)malloc(sizeof(struct void_ll_ptr));
    res->value = element;
    res->next = list;
    return res;
}

void void_ll_ptr_printf(LL_PTR(void)* list) {
    LL_PTR_PRINTF(void, list, printf("%p", element));
}

void void_ll_ptr_free(LL_PTR(void)* list) {
    LL_PTR_FREE(void, list, free(element));
}

void* void_ll_ptr_head(LL_PTR(void)* list) {
    if (list == NULL) return NULL;
    else return list->value;
}

void* void_ll_ptr_tail(LL_PTR(void)* list) {
    if (list == NULL) return NULL;
    while (list->next != NULL) list = list->next;
    return list->value;
}

void* void_ll_ptr_get(LL_PTR(void)* list, size_t index) {
    if (list == NULL) return NULL;
    while (list->next != NULL && index != 0) {
        list = list->next;
        index--;
    }
    if (index > 0) return NULL;
    return list->value;
}

size_t void_ll_ptr_length(LL_PTR(void)* list) {
    if (list == NULL) return 0;
    size_t res = 1;
    while (list->next != NULL) {
        list = list->next;
        res++;
    }
    return res;
}
