#include <trie.h>
#include <test.h>
#include <string.h>
#include <stdlib.h>

test_f test_create(void)
{
    Trie *trie;

    trie = trie_create();
    T_ERROR(trie == NULL);
    T_EXPECT(trie_get_num_entries(trie), 0);

    trie_destroy(trie);
}

test_f test_insert(void)
{
    char *words[] = {"Kukos", "Jest", "Masterem", "C", "I", "Kocha", "Makra"};
    int hight[] = {6, 6, 9, 9, 9, 9, 9};
    size_t size = ARRAY_SIZE(words);
    size_t i;

    Trie *trie;

    trie = trie_create();
    T_ERROR(trie == NULL);
    T_EXPECT(trie_get_num_entries(trie), 0);

    for (i = 0; i < size; ++i)
    {
        T_EXPECT(trie_insert(trie, words[i]), 0);
        T_EXPECT(trie_get_hight(trie), hight[i]);
    }

    T_EXPECT(trie_get_num_entries(trie), size);

    trie_destroy(trie);
}

test_f test_insert_the_same(void)
{
    char *words[] = {"Kukos", "Jest", "Masterem", "C", "I", "Kocha", "Makra"};
    size_t size = ARRAY_SIZE(words);
    size_t i;

    Trie *trie;

    trie = trie_create();
    T_ERROR(trie == NULL);
    T_EXPECT(trie_get_num_entries(trie), 0);

    for (i = 0; i < size; ++i)
        T_EXPECT(trie_insert(trie, words[i]), 0);

    T_EXPECT(trie_get_num_entries(trie), size);

    for (i = 0; i < size; ++i)
        T_CHECK(trie_insert(trie, words[i]) != 0);

    T_EXPECT(trie_get_num_entries(trie), size);

    trie_destroy(trie);
}

test_f test_search(void)
{
    char *words[] = {"Kukos", "Jest", "Masterem", "C", "I", "Kocha", "Makra"};
    char *words2[] = {"1", "3", "5", "11", "17", "41"};
    size_t size = ARRAY_SIZE(words);
    size_t size2 = ARRAY_SIZE(words2);
    size_t i;

    Trie *trie;

    trie = trie_create();
    T_ERROR(trie == NULL);
    T_EXPECT(trie_get_num_entries(trie), 0);

    for (i = 0; i < size; ++i)
        T_EXPECT(trie_insert(trie, words[i]), 0);

    T_EXPECT(trie_get_num_entries(trie), size);

    for (i = 0; i < size; ++i)
        T_EXPECT(trie_find(trie, words[i]), true);

    for (i = 0; i < size2; ++i)
        T_EXPECT(trie_find(trie, words2[i]), false);

    T_EXPECT(trie_get_num_entries(trie), size);
    trie_destroy(trie);
}

test_f test_delete(void)
{
    char *words[] = {"Kukos", "Jest", "Masterem", "C", "I", "Kocha", "Makra"};
    size_t size = ARRAY_SIZE(words);
    int hight = 9;
    int hight2 = 6;
    size_t i;

    Trie *trie;

    trie = trie_create();
    T_ERROR(trie == NULL);
    T_EXPECT(trie_get_num_entries(trie), 0);

    for (i = 0; i < size; ++i)
        T_EXPECT(trie_insert(trie, words[i]), 0);

    T_EXPECT(trie_get_num_entries(trie), size);
    T_EXPECT(trie_get_hight(trie), hight);

    for (i = 0; i < size; ++i)
        T_EXPECT(trie_find(trie, words[i]), true);

    for (i = 0; i < (size + 1) >> 1; ++i)
        T_EXPECT(trie_delete(trie, words[i]), 0);

    T_EXPECT(trie_get_num_entries(trie), size >> 1);
    T_EXPECT(trie_get_hight(trie), hight2);

    for (i = 0; i < (size + 1) >> 1; ++i)
        T_EXPECT(trie_find(trie, words[i]), false);

    for (i = (size + 1) >> 1; i < size; ++i)
        T_EXPECT(trie_find(trie, words[i]), true);

    for (i = (size + 1) >> 1; i < size; ++i)
        T_EXPECT(trie_delete(trie, words[i]), 0);

    T_EXPECT(trie_get_num_entries(trie), 0);
    T_EXPECT(trie_get_hight(trie), 1);

    for (i = 0; i < size; ++i)
        T_EXPECT(trie_find(trie, words[i]), false);

    trie_destroy(trie);
}

test_f test_delete_the_same(void)
{
    char *words[] = {"Kukos", "Jest", "Masterem", "C", "I", "Kocha", "Makra"};
    size_t size = ARRAY_SIZE(words);
    size_t i;

    Trie *trie;

    trie = trie_create();
    T_ERROR(trie == NULL);
    T_EXPECT(trie_get_num_entries(trie), 0);

    for (i = 0; i < size; ++i)
        T_EXPECT(trie_insert(trie, words[i]), 0);

    T_EXPECT(trie_get_num_entries(trie), size);

    for (i = 0; i < size; ++i)
        T_EXPECT(trie_find(trie, words[i]), true);

    for (i = 0; i < (size + 1) >> 1; ++i)
        T_EXPECT(trie_delete(trie, words[i]), 0);

    T_EXPECT(trie_get_num_entries(trie), size >> 1);

    for (i = 0; i < (size + 1) >> 1; ++i)
        T_EXPECT(trie_find(trie, words[i]), false);

    for (i = (size + 1) >> 1; i < size; ++i)
        T_EXPECT(trie_find(trie, words[i]), true);

    for (i = 0; i < (size + 1) >> 1; ++i)
        T_CHECK(trie_delete(trie, words[i]) != 0);

    for (i = (size + 1) >> 1; i < size; ++i)
        T_EXPECT(trie_delete(trie, words[i]), 0);

    T_EXPECT(trie_get_num_entries(trie), 0);

    for (i = 0; i < size; ++i)
        T_EXPECT(trie_find(trie, words[i]), false);

    for (i = 0; i < size; ++i)
        T_CHECK(trie_delete(trie, words[i]) != 0);

    trie_destroy(trie);
}

test_f test_insert_delete(void)
{
    char *words[] = {"Kukos", "Jest", "Masterem", "C", "I", "Kocha", "Makra"};
    size_t size = ARRAY_SIZE(words);
    size_t i;

    Trie *trie;

    trie = trie_create();
    T_ERROR(trie == NULL);
    T_EXPECT(trie_get_num_entries(trie), 0);

    for (i = 0; i < size; ++i)
        T_EXPECT(trie_insert(trie, words[i]), 0);

    T_EXPECT(trie_get_num_entries(trie), size);

    for (i = 0; i < size; ++i)
        T_EXPECT(trie_find(trie, words[i]), true);

    for (i = 0; i < (size + 1) >> 1; ++i)
        T_EXPECT(trie_delete(trie, words[i]), 0);

    T_EXPECT(trie_get_num_entries(trie), size >> 1);

    for (i = 0; i < (size + 1) >> 1; ++i)
        T_EXPECT(trie_find(trie, words[i]), false);

    for (i = (size + 1) >> 1; i < size; ++i)
        T_EXPECT(trie_find(trie, words[i]), true);

    for (i = (size + 1) >> 1; i < size; ++i)
        T_EXPECT(trie_delete(trie, words[i]), 0);

    T_EXPECT(trie_get_num_entries(trie), 0);

    for (i = 0; i < size; ++i)
        T_EXPECT(trie_find(trie, words[i]), false);

    for (i = 0; i < size; ++i)
        T_EXPECT(trie_insert(trie, words[i]), 0);

    T_EXPECT(trie_get_num_entries(trie), size);

    for (i = 0; i < size; ++i)
        T_EXPECT(trie_find(trie, words[i]), true);

    trie_destroy(trie);
}

test_f test_convert_to_array(void)
{
    char *words[] = {"Kukos", "Jest", "Masterem", "C", "I", "Kocha", "Makra"};
    char *wexp[] = {"C", "I", "Jest", "Kocha", "Kukos", "Makra", "Masterem"};
    char **ret;
    size_t rsize;
    size_t size = ARRAY_SIZE(words);
    size_t i;

    Trie *trie;

    trie = trie_create();
    T_ERROR(trie == NULL);
    T_EXPECT(trie_get_num_entries(trie), 0);

    for (i = 0; i < size; ++i)
        T_EXPECT(trie_insert(trie, words[i]), 0);

    T_EXPECT(trie_get_num_entries(trie), size);

    ret = trie_to_array(trie, &rsize);
    T_ERROR(ret == NULL);
    T_ASSERT(rsize, size);

    for (i = 0; i < size; ++i)
        T_EXPECT(strcmp(wexp[i], ret[i]), 0);

    for (i = 0; i < size; ++i)
        FREE(ret[i]);

    FREE(ret);
    trie_destroy(trie);
}

test_f test_empty(void)
{
    Trie *trie;
    size_t size;

    trie = trie_create();
    T_ERROR(trie == NULL);
    T_EXPECT(trie_get_num_entries(trie), 0);
    T_CHECK(trie_delete(trie, "Kukos") != 0);
    T_EXPECT(trie_find(trie, "Kukos"), false);
    T_EXPECT(trie_to_array(trie, &size), NULL);
    T_EXPECT(trie_get_hight(trie), 1);

    trie_destroy(trie);
}

test_f test_for_each(void)
{
    char *words[] = {"Kukos", "Jest", "Masterem", "C", "I", "Kocha", "Makra"};
    char *wexp[] = {"C", "I", "Jest", "Kocha", "Kukos", "Makra", "Masterem"};
    size_t size = ARRAY_SIZE(words);
    size_t i;
    Trie_node *node;
    char *data;

    Trie *trie;

    trie = trie_create();
    T_ERROR(trie == NULL);
    T_EXPECT(trie_get_num_entries(trie), 0);

    for (i = 0; i < size; ++i)
        T_EXPECT(trie_insert(trie, words[i]), 0);

    T_EXPECT(trie_get_num_entries(trie), size);

    i = 0;
    for_each(trie, Trie, node, data)
    {
        T_CHECK(node != NULL);
        T_EXPECT(strcmp(wexp[i], data), 0);
        ++i;
    }

    i = size - 1;
    for_each_prev(trie, Trie, node, data)
    {
        T_CHECK(node != NULL);
        T_EXPECT(strcmp(wexp[i], data), 0);
        --i;
    }

    i = 0;
    for_each_data(trie, Trie, data)
         T_EXPECT(strcmp(wexp[i++], data), 0);

    i = size - 1;
    for_each_data_prev(trie, Trie, data)
         T_EXPECT(strcmp(wexp[i--], data), 0);

    for_each_node(trie, Trie, node)
        T_CHECK(node != NULL);

    for_each_node_prev(trie, Trie, node)
        T_CHECK(node != NULL);

    trie_destroy(trie);
}

test_f test_empty_for_each(void)
{
    char *words[] = {"", "", ""};
    char *wexp[] = {"", "", ""};
    size_t size = ARRAY_SIZE(words);
    size_t i;
    Trie_node *node;
    char *data;

    Trie *trie;

    trie = trie_create();
    T_ERROR(trie == NULL);
    T_EXPECT(trie_get_num_entries(trie), 0);

    i = 0;
    for_each(trie, Trie, node, data)
    {
        T_CHECK(node != NULL);
        T_EXPECT(strcmp(wexp[i], data), 0);
        ++i;
    }

    i = size - 1;
    for_each_prev(trie, Trie, node, data)
    {
        T_CHECK(node != NULL);
        T_EXPECT(strcmp(wexp[i], data), 0);
        --i;
    }

    i = 0;
    for_each_data(trie, Trie, data)
         T_EXPECT(strcmp(wexp[i++], data), 0);

    i = size - 1;
    for_each_data_prev(trie, Trie, data)
         T_EXPECT(strcmp(wexp[i--], data), 0);

    for_each_node(trie, Trie, node)
        T_CHECK(node != NULL);

    for_each_node_prev(trie, Trie, node)
        T_CHECK(node != NULL);

    trie_destroy(trie);
}

void test(void)
{
    TEST(test_create());
    TEST(test_insert());
    TEST(test_insert_the_same());
    TEST(test_search());
    TEST(test_delete());
    TEST(test_delete_the_same());
    TEST(test_insert_delete());
    TEST(test_convert_to_array());
    TEST(test_empty());
    TEST(test_for_each());
    TEST(test_empty_for_each());
}

int main(void)
{
    TEST_INIT("TRIE");
    test();
    TEST_SUMMARY();
}