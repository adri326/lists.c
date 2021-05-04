#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <check.h>
#include <linkedlist.h>
#include <btree.h>
#include <bstree.h>
#include <vec.h>

DECL_LL(int);
DEF_LL(int);
DECL_LL_PTR(char);

DECL_BT(int);
DEF_BT(int);

DECL_BST(int);
DECL_BST_LL(int);
DEF_BST(int);
DEF_BST_LL(int);

DECL_VEC(int);
DEF_VEC(int);

START_TEST(test_ll_new) {
    LL(int)* int_list = int_ll_new(10);
    ck_assert_int_eq(int_list->value, 10);
    ck_assert(int_list->next == NULL);
    int_ll_free(int_list);

    LL_PTR(char)* string_list = LL_PTR_NEW(char, "Hello, world!");
    ck_assert_str_eq(string_list->value, "Hello, world!");
    ck_assert(string_list->next == NULL);
    LL_PTR_FREE_AUTO(char, string_list);
}
END_TEST

START_TEST(test_ll_push) {
    LL(int)* int_list = int_ll_new(10);
    int_list = int_ll_push_tail(int_list, 15);
    int_list = int_ll_push_head(int_list, 5);
    LL(int)* alias = int_list;
    for (int n = 1; n <= 3; n++) {
        ck_assert_int_eq(alias->value, 5 * n);
        if (n != 3) {
            ck_assert(alias->next != NULL);
        } else {
            ck_assert(alias->next == NULL);
        }
        alias = alias->next;
    }
    int_ll_free(int_list);
}
END_TEST

START_TEST(test_ll_head) {
    LL(int)* int_list = int_ll_new(10);
    int_list = int_ll_push_tail(int_list, 15);
    ck_assert_int_eq(*int_ll_head(int_list), 10);
    int_ll_free(int_list);

    LL_PTR(char)* string_list = LL_PTR_NEW(char, "Hello, world!");
    LL_PTR_PUSH_TAIL(char, string_list, "I am another element.");
    ck_assert_str_eq(LL_PTR_HEAD(char, string_list), "Hello, world!");
    LL_PTR_FREE_AUTO(char, string_list);
}
END_TEST

START_TEST(test_ll_tail) {
    LL(int)* int_list = int_ll_new(10);
    int_list = int_ll_push_tail(int_list, 15);
    ck_assert_int_eq(*int_ll_tail(int_list), 15);
    int_ll_free(int_list);

    LL_PTR(char)* string_list = LL_PTR_NEW(char, "Hello, world!");
    LL_PTR_PUSH_TAIL(char, string_list, "I am another element.");
    ck_assert_str_eq(LL_PTR_TAIL(char, string_list), "I am another element.");
    LL_PTR_FREE_AUTO(char, string_list);
}
END_TEST

START_TEST(test_ll_length) {
    LL(int)* int_list = int_ll_new(0);
    LL_PTR(char)* string_list = LL_PTR_NEW(char, "0");
    for (int n = 2; n < 6; n++) {
        int_list = int_ll_push_tail(int_list, 0);
        LL_PTR_PUSH_TAIL(char, string_list, "0");
        ck_assert_int_eq(int_ll_length(int_list), n);
        ck_assert_int_eq(LL_PTR_LENGTH(char, string_list), n);
    }
    int_ll_free(int_list);
    LL_PTR_FREE_AUTO(char, string_list);
}
END_TEST

START_TEST(test_bt_new) {
    BT(int)* int_btree = int_bt_new(16);

    ck_assert_int_eq(int_btree->value == 16, 1);

    int_bt_free(int_btree);
}
END_TEST

BT(int)* test_bt_size_rec(size_t depth) {
    if (depth == 0) {
        return int_bt_new(depth);
    } else {
        return int_bt_connect(test_bt_size_rec(depth - 1), test_bt_size_rec(depth - 1), depth);
    }
}

START_TEST(test_bt_size) {
    BT(int)* int_btree = int_bt_new(0);
    BT(int)* big_btree = test_bt_size_rec(3);

    ck_assert_int_eq(int_bt_size(int_btree), 1);
    ck_assert_int_eq(int_bt_size(big_btree), 15);

    int_bt_free(int_btree);
    int_bt_free(big_btree);
}
END_TEST

START_TEST(test_bt_depth) {
    BT(int)* int_btree = int_bt_new(0);
    BT(int)* big_btree = test_bt_size_rec(3);

    ck_assert_int_eq(int_bt_depth(int_btree), 1);
    ck_assert_int_eq(int_bt_depth(big_btree), 4);

    int_bt_free(int_btree);
    int_bt_free(big_btree);
}
END_TEST

START_TEST(test_bt_leaves) {
    BT(int)* int_btree = int_bt_new(0);
    BT(int)* big_btree = test_bt_size_rec(3);

    ck_assert_int_eq(int_bt_leaves(int_btree), 1);
    ck_assert_int_eq(int_bt_leaves(big_btree), 8);

    int_bt_free(int_btree);
    int_bt_free(big_btree);
}
END_TEST

START_TEST(test_bt_get) {
    BT(int)* int_btree = int_bt_connect(int_bt_new(2), int_bt_connect(int_bt_new(3), NULL, 1), 0);

    ck_assert_int_eq(int_bt_get(int_btree, 0b111)->value, 1);
    ck_assert_int_eq(int_bt_get(int_btree, 0b11)->value, 1);
    ck_assert_int_eq(int_bt_get(int_btree, 0b01)->value, 3);
    ck_assert_int_eq(int_bt_get(int_btree, 0b10)->value, 2);
    ck_assert_int_eq(int_bt_get(int_btree, 0b00)->value, 2);

    int_bt_free(int_btree);
}
END_TEST

START_TEST(test_vec_new) {
    VEC(int)* int_vec = int_vec_new(4);

    int_vec_free(int_vec);
}
END_TEST

START_TEST(test_vec_length) {
    VEC(int)* int_vec = int_vec_new(4);

    for (int n = 0; n < 4; n++) {
        ck_assert_int_eq((int)int_vec_length(int_vec), n);
        int_vec_push(int_vec, n);
    }

    int_vec_free(int_vec);
}
END_TEST

START_TEST(test_vec_pop) {
    VEC(int)* int_vec = int_vec_new(4);

    for (int n = 0; n < 4; n++) {
        int_vec_push(int_vec, n);
    }

    for (int n = 3; n >= 0; n--) {
        ck_assert_int_eq(int_vec_pop(int_vec), n);
        ck_assert_int_eq(int_vec_length(int_vec), n);
    }

    int_vec_free(int_vec);
}
END_TEST

bool test_vec_find_sub(const int* value, const void* cmp) {
    return *value == *(int*)cmp;
}

START_TEST(test_vec_find) {
    VEC(int)* int_vec = int_vec_new(4);

    for (int n = 0; n < 4; n++) {
        int_vec_push(int_vec, n);
    }

    for (int n = 0; n < 4; n++) {
        ck_assert_int_eq(int_vec_find(int_vec, test_vec_find_sub, &n), n);
    }

    int x = -1;
    ck_assert_int_eq(int_vec_find(int_vec, test_vec_find_sub, &x), -1);

    int_vec_free(int_vec);
}
END_TEST

Suite* ll_suite() {
    Suite* res = suite_create("LinkedList");
    TCase* tc_core = tcase_create("Core");
    tcase_add_test(tc_core, test_ll_new);
    tcase_add_test(tc_core, test_ll_push);
    tcase_add_test(tc_core, test_ll_head);
    tcase_add_test(tc_core, test_ll_tail);
    suite_add_tcase(res, tc_core);
    return res;
}

Suite* bt_suite() {
    Suite* res = suite_create("BinaryTree");
    TCase* tc_core = tcase_create("Core");
    tcase_add_test(tc_core, test_bt_new);
    tcase_add_test(tc_core, test_bt_size);
    tcase_add_test(tc_core, test_bt_depth);
    tcase_add_test(tc_core, test_bt_leaves);
    tcase_add_test(tc_core, test_bt_get);
    suite_add_tcase(res, tc_core);
    return res;
}

Suite* vec_suite() {
    Suite* res = suite_create("Vec");
    TCase* tc_core = tcase_create("Core");
    tcase_add_test(tc_core, test_vec_new);
    tcase_add_test(tc_core, test_vec_length);
    tcase_add_test(tc_core, test_vec_pop);
    tcase_add_test(tc_core, test_vec_find);
    suite_add_tcase(res, tc_core);
    return res;
}

int main(int argc, char* argv[]) {
    Suite* ll_s = ll_suite();
    Suite* bt_s = bt_suite();
    Suite* vec_s = vec_suite();
    SRunner* ll_sr = srunner_create(ll_s);
    SRunner* bt_sr = srunner_create(bt_s);
    SRunner* vec_sr = srunner_create(vec_s);

    srunner_run_all(ll_sr, CK_NORMAL);
    srunner_run_all(bt_sr, CK_NORMAL);
    srunner_run_all(vec_sr, CK_NORMAL);
    int fails = srunner_ntests_failed(ll_sr) + srunner_ntests_failed(bt_sr) + srunner_ntests_failed(vec_sr);
    return (fails == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
