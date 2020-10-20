#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <check.h>
#include <linkedlist.h>

DECL_LL(int);
DECL_LL_SOURCES(int, "%d");
DECL_LL_PTR(char);

START_TEST(test_ll_new) {
    LL(int)* int_list = int_ll_new(10);
    ck_assert_int_eq(int_list->value, 10);
    ck_assert(int_list->next == NULL);
    int_ll_free(int_list);

    LL_PTR(char)* string_list = LL_PTR_NEW(char, "Hello, world!");
    ck_assert_str_eq(string_list->value, "Hello, world!");
    ck_assert(string_list->next == NULL);
    LL_PTR_FREE2(char, string_list);
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
    LL_PTR_FREE2(char, string_list);
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
    LL_PTR_FREE2(char, string_list);
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
    LL_PTR_FREE2(char, string_list);
}

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

int main(int argc, char* argv[]) {
    Suite* ll_s = ll_suite();
    SRunner* sr = srunner_create(ll_s);

    srunner_run_all(sr, CK_NORMAL);
    int fails = srunner_ntests_failed(sr);
    return (fails == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
