#ifndef VEC_H
#define VEC_H

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

#define VEC(type) type##_vec_t

#define DECL_VEC(type) struct type##_vec { \
        type* data; \
        size_t capacity; \
        size_t length; \
    }; \
    typedef struct type##_vec VEC(type); \
    VEC(type)* type##_vec_new(const size_t capacity); \
    void type##_vec_free(VEC(type)* vec); \
    size_t type##_vec_resize(VEC(type)* vec, size_t new_capacity); \
    size_t type##_vec_push(VEC(type)* vec, type value); \
    type type##_vec_pop(VEC(type)* vec); \
    size_t type##_vec_length(const VEC(type)* vec); \
    size_t type##_vec_capacity(const VEC(type)* vec); \
    type* type##_vec_get(VEC(type)* vec, const size_t index); \
    int type##_vec_find(const VEC(type)* vec, bool (*predicate)(const type*, const void*), const void* predicate_data);

#define DECL_VEC_SOURCES(type) \
    VEC(type)* type##_vec_new(const size_t capacity) { \
        VEC(type)* res = (VEC(type)*)malloc(sizeof(struct type##_vec)); \
        if (res == NULL) return NULL; \
        res->capacity = capacity; \
        res->length = 0; \
        res->data = (type*)malloc(sizeof(type) * capacity); \
        if (res->data == NULL) return NULL; \
        return res; \
    } \
    void type##_vec_free(VEC(type)* vec) { \
        if (vec == NULL) return; \
        free(vec->data); \
        free(vec); \
    } \
    size_t type##_vec_resize(VEC(type)* vec, size_t new_capacity) { \
        if (vec == NULL) return 0; \
        if (new_capacity == 0) return vec->capacity; \
        if (new_capacity < vec->length) new_capacity = vec->length; \
        if (new_capacity == 0) return 0; /* zero reallocation are deprecated as of C11 DR 400; see https://en.cppreference.com/w/c/memory/realloc */ \
        type* new_data = (type*)realloc((void*)vec->data, sizeof(type) * new_capacity); \
        if (new_data == NULL) return 0; \
        vec->data = new_data; \
        vec->capacity = new_capacity; \
        return new_capacity; \
    } \
    size_t type##_vec_push(VEC(type)* vec, type value) { \
        if (vec == NULL) return 0; \
        if (vec->length == vec->capacity) { \
            if (type##_vec_resize(vec, vec->capacity * 2) == 0) return 0; \
        } \
        vec->data[vec->length] = value; \
        vec->length += 1; \
        return vec->length; \
    } \
    type type##_vec_pop(VEC(type)* vec) { \
        type res; /* Initialized to zero */ \
        if (vec->length == 0) return res; \
        res = vec->data[vec->length - 1]; \
        vec->length -= 1; \
        return res; \
    } \
    size_t type##_vec_length(const VEC(type)* vec) { \
        if (vec == NULL) return 0; \
        else return vec->length; \
    } \
    size_t type##_vec_capacity(const VEC(type)* vec) { \
        if (vec == NULL) return 0; \
        else return vec->capacity; \
    } \
    type* type##_vec_get(VEC(type)* vec, const size_t index) { \
        if (vec == NULL) return NULL; \
        if (index >= vec->length) return NULL; \
        return &vec->data[index]; \
    } \
    int type##_vec_find(const VEC(type)* vec, bool (*predicate)(const type*, const void*), const void* predicate_data) { \
        if (vec == NULL) return -1; \
        for (size_t n = 0; n < vec->length; n++) { \
            if (predicate(&vec->data[n], predicate_data)) return (int)n; \
        } \
        return -1; \
    }

/** @struct TYPE_vec

    A scalable array or "vector" with values of type `TYPE`
**/

/** @typedef TYPE_vec_t

    Resolves to `struct TYPE_vec`; can also be accessed with VEC(TYPE)
**/

/** @def VEC(TYPE)

    Resolves to `struct TYPE_vec`;
**/

/** @fn TYPE_vec_new(const size_t capacity)
    @param capacity The capacity in number of elements
    @returns The new vector

    Creates a new scalable array with values of type `TYPE` and initial capacity `capacity`.
    Note that if `capacity = 0`, pushing anything to the array will not work (as the capacity is doubled when a necessary resize is triggered).
    It is, however, possible to escape such state by using TYPE_vec_resize(capacity > 0)
**/

/** @fn TYPE_vec_free(VEC(TYPE)* vector)
    @param vector The scalable array to free

    Frees all of the memory owned by `vector`.
**/

/** @fn TYPE_vec_length(VEC(TYPE)* vector)
    @param vector The scalable array whose length is to get
    @returns The length of `vector`, 0 if NULL

    Returns the length of vector, or 0 if that pointer is NULL.
**/

/** @fn TYPE_vec_capacity(VEC(TYPE)* vector)
    @param vector The scalable array whose capacity is to get
    @returns The capacity of `vector`, 0 if NULL

    Returns the capacity of vector, or 0 if that pointer is NULL.
**/

/** @fn TYPE_vec_resize(VEC(TYPE)* vector, size_t new_capacity)
    @param vector The scalable array whose capacity is to change
    @param new_capacity The new capacity of the scalable array. Must be non-null, or else nothing is done.
    @returns The new capacity (adapted based on `vector->length`)

    Resizes the vector's allocated space. The values contained by `vector` are left unchanged, although previous pointers to these might become invalidated.
    If `new_capacity < vector`, then the allocated space will be shrunk to the length of vector, as to not loose any element.
    Thus, if you wish to shrink the vector, you may call `TYPE_vec_resize(vector, 1);`
**/

/** @fn TYPE_vec_push(VEC(TYPE)* vector, TYPE value)
    @param vector The scalable array to push the value to
    @param value The value to push
    @returns The new length

    Pushes a value at the end of `vector`.
    A resize may be triggered during the process; if that resize fails (lack of hardware space or null initial capacity), then the element won't be pushed and the function will return 0.
**/

/** @fn TYPE_vec_pop(VEC(TYPE)* vector)
    @param vector The vector to pop a value from
    @returns The popped value, or 0 if the vector is NULL or empty

    Pops the last value of `vector`; returns 0 on failure.
**/

/** @fn TYPE_vec_find(const VEC(TYPE)* vector, bool (*predicate)(const TYPE*, const void*), const void* predicate_data)
    @param vector The vector to search through
    @param predicate The predicate; a function that takes as input a pointer to a value of `vector` and a user-defined void pointer
    @param predicate_data A user-defined void pointer to data that the predicate can use
    @returns -1 on failure or if predicate never returned true, the index of the first value found otherwise

    Finds a value of `vector` based on a `predicate` function.
    Returns the index of the first value for which `predicate(value)` yields true.

    `predicate` may be given an arbitrary piece of information through `predicate_data`.

    ## Example

    ```c
    DECL_VEC(int);
    DECL_VEC_SOURCES(int, "%d");

    bool int_eq(int* a, void* b) {
        return *a == *(int*)b;
    }

    int main() {
        VEC(int)* int_vec = int_vec_new(4);

        int_vec_push(int_vec, 1);
        int_vec_push(int_vec, 2);
        int_vec_push(int_vec, 3);
        int_vec_push(int_vec, 4);

        int x = 3;
        printf("%d", int_vec_find(int_vec, &int_eq, &x)); // prints 2
    }
    ```
**/

#endif // VEC_H
