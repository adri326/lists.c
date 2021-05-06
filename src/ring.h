#ifndef RING_H
#define RING_H

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

#define _RING_MIN(a, b) ((a) < (b) ? (a) : (b))

#define RING(type) type##_ring_t

/** @def DECL_RING(type)
    @param type The type of the circular vector

    Declares a circular vector (RING) type and RING-associated functions.
**/
#define DECL_RING(type) struct type##_ring { \
        type* data; \
        size_t capacity; \
        size_t start; \
        size_t length; \
    }; \
    typedef struct type##_ring RING(type); \
    RING(type)* type##_ring_new(const size_t capacity); \
    void type##_ring_free(RING(type)* ring); \
    size_t type##_ring_resize(RING(type)* ring, size_t new_capacity); \
    size_t type##_ring_push_head(RING(type)* ring, type value); \
    size_t type##_ring_push_tail(RING(type)* ring, type value); \
    type type##_ring_pop_head(RING(type)* ring); \
    type type##_ring_pop_tail(RING(type)* ring); \
    size_t type##_ring_length(const RING(type)* ring); \
    size_t type##_ring_capacity(const RING(type)* ring); \
    type* type##_ring_get(RING(type)* ring, const size_t index); \
    RING(type)* type##_ring_clone(const RING(type)* ring); \
    void type##_ring_printf(RING(type)* ring); /* Note: only available if DEF_RING_PRINTF* is used */ \
    int type##_ring_find(const RING(type)* ring, bool (*predicate)(const type*, const void*), const void* predicate_data);

/** @def DEF_RING(type)
    @param type The type of the circular vector

    Defines the RING-associated functions.
**/
#define DEF_RING(type) \
    RING(type)* type##_ring_new(const size_t capacity) { \
        RING(type)* res = (RING(type)*)malloc(sizeof(struct type##_ring)); \
        if (res == NULL) return NULL; \
        res->capacity = capacity; \
        res->length = 0; \
        res->start = 0; \
        res->data = (type*)malloc(sizeof(type) * capacity); \
        if (res->data == NULL) return NULL; \
        return res; \
    } \
    void type##_ring_free(RING(type)* ring) { \
        if (ring == NULL) return; \
        free(ring->data); \
        free(ring); \
    } \
    size_t type##_ring_resize(RING(type)* ring, size_t new_capacity) { \
        if (ring == NULL) return 0; \
        if (new_capacity == 0) return ring->capacity; \
        if (new_capacity < ring->length) new_capacity = ring->length; \
        if (new_capacity == 0) return 0; /* zero reallocation are deprecated as of C11 DR 400; see https://en.cppreference.com/w/c/memory/realloc */ \
        if (ring->start + ring->length <= ring->capacity && ring->start + ring->length <= new_capacity) { \
            /* No overlapping or memory to move: simply use realloc */ \
            type* new_data = (type*)realloc(ring->data, sizeof(type) * new_capacity); \
            if (new_data == NULL) return 0; \
            ring->data = new_data; \
            ring->capacity = new_capacity; \
        } else { \
            /* Allocating a new region because I can't be bothered to do all of the overlapping copies. You're welcome to contribute if you dare. */ \
            type* new_data = (type*)malloc(sizeof(type) * new_capacity); \
            if (new_data == NULL) return 0; \
            /* [BB..AAA], [........] -> [AAA.....] */ \
            memcpy(new_data, ring->data + ring->start, sizeof(type) * _RING_MIN(ring->length, (ring->capacity - ring->start))); \
            if (ring->start + ring->length > ring->capacity) { \
                /* [BB..AAA], [AAA.....] -> [AAABB...] */ \
                memcpy(new_data + ring->capacity - ring->start, ring->data, sizeof(type) * (ring->start + ring->length - ring->capacity)); \
            } \
            free(ring->data); \
            ring->data = new_data; \
            ring->capacity = new_capacity; \
            ring->start = 0; \
        } \
        return new_capacity; \
    } \
    size_t type##_ring_push_head(RING(type)* ring, type value) { \
        if (ring->length >= ring->capacity) { \
            if (type##_ring_resize(ring, ring->capacity * 2) == 0) return 0; \
        } \
        ring->data[(ring->start + ring->length) % ring->capacity] = value; \
        ring->length += 1; \
        return ring->length; \
    } \
    size_t type##_ring_push_tail(RING(type)* ring, type value) { \
        if (ring->length >= ring->capacity) { \
            if (type##_ring_resize(ring, ring->capacity * 2) == 0) return 0; \
        } \
        ring->start = (ring->start + ring->capacity - 1) % ring->capacity; \
        ring->data[ring->start] = value; \
        ring->length += 1; \
        return ring->length; \
    } \
    type type##_ring_pop_head(RING(type)* ring) { \
        type res; \
        if (ring->length == 0) return res; \
        res = ring->data[ring->start + ring->length - 1]; \
        ring->length -= 1; \
        return res; \
    } \
    type type##_ring_pop_tail(RING(type)* ring) { \
        type res; \
        if (ring->length == 0) return res; \
        res = ring->data[ring->start]; \
        ring->start += 1; \
        ring->length -= 1; \
        return res; \
    } \
    size_t type##_ring_length(const RING(type)* ring) { \
        if (ring == NULL) return 0; \
        return ring->length; \
    } \
    size_t type##_ring_capacity(const RING(type)* ring) { \
        if (ring == NULL) return 0; \
        return ring->capacity; \
    } \
    type* type##_ring_get(RING(type)* ring, const size_t index) { \
        if (ring == NULL) return NULL; \
        if (index >= ring->length) return NULL; \
        return &ring->data[(ring->start + index) % ring->capacity]; \
    } \
    RING(type)* type##_ring_clone(const RING(type)* ring) { \
        RING(type)* res = type##_ring_new(ring->length); \
        if (!res) return NULL; \
        for (size_t n = 0; n < ring->length; n++) { \
            type##_ring_push_head(res, ring->data[(ring->start + n) % ring->capacity]); \
        } \
        return res; \
    } \
    int type##_ring_find(const RING(type)* ring, bool (*predicate)(const type*, const void*), const void* predicate_data) { \
        if (ring == NULL) return -1; \
        for (size_t n = 0; n < ring->length; n++) { \
            if (predicate(&ring->data[(n + ring->start) % ring->capacity], predicate_data)) return (int)n; \
        } \
        return -1; \
    }

/**
    @def DEF_RING_PRINTF_CUSTOM
    @param type The type of the elements in the circular vector
    @param printf_callback A block of code that prints a value of the circular vector. The value can be accessed through the symbol `value`.

    Defines the `TYPE_ring_printf` function, printing the values of the circular vector *by executing* `printf_callback`.

    ## Example

    ```
    DEF_RING_PRINTF_CUSTOM(char, printf("'%c'", value));
    ```
**/
#define DEF_RING_PRINTF_CUSTOM(type, printf_callback) \
    void type##_ring_printf(RING(type)* ring) { \
        printf("Vec<" #type "> ["); \
        for (size_t n = 0; n < ring->length; n++) { \
            type value = ring->data[(ring->start + n) % ring->capacity]; \
            printf_callback; \
            if (n < ring->length - 1) printf(", "); \
        } \
        printf("]\n"); \
    }

/**
    @def DEF_RING_PRINTF_FN
    @param type The type of the elements in the circular vector
    @param printf_callback A function to be called for each value of the circular vector and that prints that value. Must take as only argument `type` and may be of any return type.

    Defines the `TYPE_ring_printf` function, printing the values of the circular vector *by calling* `printf_callback`.

    ## Example

    ```
    void print_my_char(char c) {
        printf("'%c'", c);
    }

    DEF_RING_PRINTF_FN(char, print_my_char);
    ```
**/
#define DEF_RING_PRINTF_FN(type, printf_callback) \
    DEF_RING_PRINTF_CUSTOM(type, printf_callback(value))

/**
    @def DEF_RING_PRINTF
    @param type The type of the elements in the circular vector
    @param printf_format A format string for printf

    Defines the `TYPE_ring_printf` function, printing the values of the circular vector using `printf_format` *as printf format*.

    ## Example

    ```
    DEF_RING_PRINTF(char, "'%c'");
    ```
**/
#define DEF_RING_PRINTF(type, printf_format) \
    DEF_RING_PRINTF_CUSTOM(type, printf(printf_format, value))

/** @struct TYPE_ring

    A dual-ended scalable array or "circular vector" with values of type `TYPE`
**/

/** @typedef TYPE_ring_t

    Resolves to `struct TYPE_ring`; can also be accessed with RING(TYPE)
**/

/** @def RING(TYPE)

    Resolves to `struct TYPE_ring`;
**/

/** @fn TYPE_ring_new(const size_t capacity)
    @param capacity The capacity in number of elements
    @returns The new vector

    Creates a new scalable array with values of type `TYPE` and initial capacity `capacity`.
    Note that if `capacity = 0`, pushing anything to the array will not work (as the capacity is doubled when a necessary resize is triggered).
    It is, however, possible to escape such state by using TYPE_ring_resize(capacity > 0)
**/

/** @fn TYPE_ring_free(RING(TYPE)* ring)
    @param vector The scalable array to free

    Frees all of the memory owned by `ring`.
**/

/** @fn TYPE_ring_length(RING(TYPE)* ring)
    @param vector The scalable array whose length is to get
    @returns The length of `ring`, 0 if NULL

    Returns the length of `ring`, or 0 if that pointer is NULL.
**/

/** @fn TYPE_ring_capacity(RING(TYPE)* ring)
    @param vector The scalable array whose capacity is to get
    @returns The capacity of `ring`, 0 if NULL

    Returns the capacity of `ring`, or 0 if that pointer is NULL.
**/

/** @fn TYPE_ring_resize(RING(TYPE)* ring, size_t new_capacity)
    @param vector The scalable array whose capacity is to change
    @param new_capacity The new capacity of the scalable array. Must be non-null, or else nothing is done.
    @returns The new capacity (adapted based on `vector->length`)

    Resizes the circular vector's allocated space. The values contained by `ring` are left unchanged, although previous pointers to these might become invalidated.
    If `new_capacity < ring->capacity`, then the allocated space will be shrunk to the length of `ring`, as to not loose any element.
    Thus, if you wish to shrink the circular vector, you may call `TYPE_ring_resize(vector, 1);`
**/

/** @fn TYPE_ring_push(RING(TYPE)* ring, TYPE value)
    @param vector The scalable array to push the value to
    @param value The value to push
    @returns The new length

    Pushes a value at the end of `ring`.
    A resize may be triggered during the process; if that resize fails (lack of hardware space or null initial capacity), then the element won't be pushed and the function will return 0.
**/

/** @fn TYPE_ring_pop(RING(TYPE)* ring)
    @param vector The circular vector to pop a value from
    @returns The popped value, or 0 if the circular vector is NULL or empty

    Pops the last value of `ring`; returns 0 on failure.
**/

/** @fn TYPE_ring_printf(RING(TYPE)* vec)
    @param vec The circular vector to print out

    Prints a vector; useful for debugging.
    **Note:** only available if DEF_RING_PRINTF* is used.
**/

/** @fn TYPE_ring_find(const RING(TYPE)* ring, bool (*predicate)(const TYPE*, const void*), const void* predicate_data)
    @param vector The circular vector to search through
    @param predicate The predicate; a function that takes as input a pointer to a value of `ring` and a user-defined void pointer
    @param predicate_data A user-defined void pointer to data that the predicate can use
    @returns -1 on failure or if predicate never returned true, the index of the first value found otherwise

    Finds a value of `ring` based on a `predicate` function.
    Returns the index of the first value for which `predicate(value)` yields true.

    `predicate` may be given an arbitrary piece of information through `predicate_data`.

    ## Example

    ```c
    DECL_RING(int);
    DEF_RING(int);

    bool int_eq(int* a, void* b) {
        return *a == *(int*)b;
    }

    int main() {
        RING(int)* int_vec = int_vec_new(4);

        int_vec_push(int_vec, 1);
        int_vec_push(int_vec, 2);
        int_vec_push(int_vec, 3);
        int_vec_push(int_vec, 4);

        int x = 3;
        printf("%d", int_vec_find(int_vec, &int_eq, &x)); // prints 2
    }
    ```
**/

#endif // RING_H
