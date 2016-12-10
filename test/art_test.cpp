#include <fcntl.h>
#include <inttypes.h>
#include <stdio.h>
#include <string.h>
#include <cmath>
#include <gtest/gtest.h>
#include <art.h>

art_document get_document(uint32_t id) {
    art_document document;
    document.score = (uint16_t) id;
    document.id = id;
    document.offsets = new uint32_t[1]{0};
    document.offsets_len = 1;

    return document;
}

TEST(ArtTest, test_art_init_and_destroy) {
    art_tree t;
    int res = art_tree_init(&t);
    ASSERT_TRUE(res == 0);

    ASSERT_TRUE(art_size(&t) == 0);

    res = art_tree_destroy(&t);
    ASSERT_TRUE(res == 0);
}

TEST(ArtTest, test_art_insert) {
    art_tree t;
    int res = art_tree_init(&t);
    ASSERT_TRUE(res == 0);

    size_t len;
    char buf[512];
    FILE *f;
    char *path_to_resources = std::getenv("TEST_RESOURCES");
    f = fopen("/tmp/typesense_test/words.txt", "r");

    uintptr_t line = 1;
    while (fgets(buf, sizeof buf, f)) {
        len = strlen(buf);
        buf[len-1] = '\0';
        art_document document = get_document(line);
        ASSERT_TRUE(NULL == art_insert(&t, (unsigned char*)buf, len, &document, 1));
        ASSERT_TRUE(art_size(&t) == line);
        line++;

        delete document.offsets;
    }

    res = art_tree_destroy(&t);
    ASSERT_TRUE(res == 0);
}

TEST(ArtTest, test_art_insert_verylong) {
    art_tree t;
    int res = art_tree_init(&t);
    ASSERT_TRUE(res == 0);

    unsigned char key1[300] = {16,0,0,0,7,10,0,0,0,2,17,10,0,0,0,120,10,0,0,0,120,10,0,
                               0,0,216,10,0,0,0,202,10,0,0,0,194,10,0,0,0,224,10,0,0,0,
                               230,10,0,0,0,210,10,0,0,0,206,10,0,0,0,208,10,0,0,0,232,
                               10,0,0,0,124,10,0,0,0,124,2,16,0,0,0,2,12,185,89,44,213,
                               251,173,202,211,95,185,89,110,118,251,173,202,199,101,0,
                               8,18,182,92,236,147,171,101,150,195,112,185,218,108,246,
                               139,164,234,195,58,177,0,8,16,0,0,0,2,12,185,89,44,213,
                               251,173,202,211,95,185,89,110,118,251,173,202,199,101,0,
                               8,18,180,93,46,151,9,212,190,95,102,178,217,44,178,235,
                               29,190,218,8,16,0,0,0,2,12,185,89,44,213,251,173,202,
                               211,95,185,89,110,118,251,173,202,199,101,0,8,18,180,93,
                               46,151,9,212,190,95,102,183,219,229,214,59,125,182,71,
                               108,180,220,238,150,91,117,150,201,84,183,128,8,16,0,0,
                               0,2,12,185,89,44,213,251,173,202,211,95,185,89,110,118,
                               251,173,202,199,101,0,8,18,180,93,46,151,9,212,190,95,
                               108,176,217,47,50,219,61,134,207,97,151,88,237,246,208,
                               8,18,255,255,255,219,191,198,134,5,223,212,72,44,208,
                               250,180,14,1,0,0,8, '\0'};
    unsigned char key2[303] = {16,0,0,0,7,10,0,0,0,2,17,10,0,0,0,120,10,0,0,0,120,10,0,
                               0,0,216,10,0,0,0,202,10,0,0,0,194,10,0,0,0,224,10,0,0,0,
                               230,10,0,0,0,210,10,0,0,0,206,10,0,0,0,208,10,0,0,0,232,
                               10,0,0,0,124,10,0,0,0,124,2,16,0,0,0,2,12,185,89,44,213,
                               251,173,202,211,95,185,89,110,118,251,173,202,199,101,0,
                               8,18,182,92,236,147,171,101,150,195,112,185,218,108,246,
                               139,164,234,195,58,177,0,8,16,0,0,0,2,12,185,89,44,213,
                               251,173,202,211,95,185,89,110,118,251,173,202,199,101,0,
                               8,18,180,93,46,151,9,212,190,95,102,178,217,44,178,235,
                               29,190,218,8,16,0,0,0,2,12,185,89,44,213,251,173,202,
                               211,95,185,89,110,118,251,173,202,199,101,0,8,18,180,93,
                               46,151,9,212,190,95,102,183,219,229,214,59,125,182,71,
                               108,180,220,238,150,91,117,150,201,84,183,128,8,16,0,0,
                               0,3,12,185,89,44,213,251,133,178,195,105,183,87,237,150,
                               155,165,150,229,97,182,0,8,18,161,91,239,50,10,61,150,
                               223,114,179,217,64,8,12,186,219,172,150,91,53,166,221,
                               101,178,0,8,18,255,255,255,219,191,198,134,5,208,212,72,
                               44,208,250,180,14,1,0,0,8, '\0'};


    art_document doc1 = get_document(1);
    art_document doc2 = get_document(2);

    ASSERT_TRUE(NULL == art_insert(&t, key1, 299, &doc1, 1));
    ASSERT_TRUE(NULL == art_insert(&t, key2, 302, &doc2, 2));
    art_insert(&t, key2, 302, &doc2, 2);
    EXPECT_EQ(art_size(&t), 2);

    res = art_tree_destroy(&t);
    ASSERT_TRUE(res == 0);
}

TEST(ArtTest, test_art_insert_search) {
    art_tree t;
    int res = art_tree_init(&t);
    ASSERT_TRUE(res == 0);

    int len;
    char buf[512];
    FILE *f = fopen("/tmp/typesense_test/words.txt", "r");

    uintptr_t line = 1;
    while (fgets(buf, sizeof buf, f)) {
        len = strlen(buf);
        buf[len-1] = '\0';
        art_document doc = get_document((uint32_t) line);
        ASSERT_TRUE(NULL == art_insert(&t, (unsigned char*)buf, len, &doc, 1));
        line++;
    }

    // Seek back to the start
    fseek(f, 0, SEEK_SET);

    // Search for each line
    line = 1;
    while (fgets(buf, sizeof buf, f)) {
        len = strlen(buf);
        buf[len-1] = '\0';

        art_leaf* l = (art_leaf *) art_search(&t, (unsigned char*)buf, len);
        EXPECT_EQ(line, l->values->ids.at(0));
        line++;
    }

    // Check the minimum
    art_leaf *l = art_minimum(&t);
    ASSERT_TRUE(l && strcmp((char*)l->key, "A") == 0);

    // Check the maximum
    l = art_maximum(&t);
    ASSERT_TRUE(l && strcmp((char*)l->key, "zythum") == 0);

    res = art_tree_destroy(&t);
    ASSERT_TRUE(res == 0);
}


TEST(ArtTest, test_art_insert_delete) {
    art_tree t;
    int res = art_tree_init(&t);
    ASSERT_TRUE(res == 0);

    int len;
    char buf[512];
    FILE *f = fopen("/tmp/typesense_test/words.txt", "r");

    uintptr_t line = 1, nlines;
    while (fgets(buf, sizeof buf, f)) {
        len = strlen(buf);
        buf[len-1] = '\0';
        art_document doc = get_document((uint32_t) line);
        ASSERT_TRUE(NULL == art_insert(&t, (unsigned char*)buf, len, &doc, 1));
        line++;
    }

    nlines = line - 1;

    // Seek back to the start
    fseek(f, 0, SEEK_SET);

    // Search for each line
    line = 1;
    while (fgets(buf, sizeof buf, f)) {
        len = strlen(buf);
        buf[len-1] = '\0';

        // Search first, ensure all entries still
        // visible
        art_leaf* l = (art_leaf *) art_search(&t, (unsigned char*)buf, len);
        EXPECT_EQ(line, l->values->ids.at(0));

        // Delete, should get lineno back
        art_values* values = (art_values*)art_delete(&t, (unsigned char*)buf, len);
        EXPECT_EQ(line, values->ids.at(0));
        free(values);

        // Check the size
        ASSERT_TRUE(art_size(&t) == nlines - line);
        line++;
    }

    // Check the minimum and maximum
    ASSERT_TRUE(!art_minimum(&t));
    ASSERT_TRUE(!art_maximum(&t));
    ASSERT_TRUE(art_size(&t) == 0);

    res = art_tree_destroy(&t);
    ASSERT_TRUE(res == 0);
}

int iter_cb(void *data, const unsigned char* key, uint32_t key_len, void *val) {
    uint64_t *out = (uint64_t*)data;
    uintptr_t line = ((art_values*)val)->ids.at(0);
    uint64_t mask = (line * (key[0] + key_len));
    out[0]++;
    out[1] ^= mask;
    return 0;
}

TEST(ArtTest, test_art_insert_iter) {
                art_tree t;
        int res = art_tree_init(&t);
        ASSERT_TRUE(res == 0);

        int len;
        char buf[512];
        FILE *f = fopen("/tmp/typesense_test/words.txt", "r");

        uint64_t xor_mask = 0;
        uintptr_t line = 1, nlines;
        while (fgets(buf, sizeof buf, f)) {
            len = strlen(buf);
            buf[len-1] = '\0';
            art_document doc = get_document((uint32_t) line);
            ASSERT_TRUE(NULL == art_insert(&t, (unsigned char*)buf, len, &doc, 1));

            xor_mask ^= (line * (buf[0] + len));
            line++;
        }
        nlines = line - 1;

        uint64_t out[] = {0, 0};
        ASSERT_TRUE(art_iter(&t, iter_cb, &out) == 0);

        ASSERT_TRUE(out[0] == nlines);
        ASSERT_TRUE(out[1] == xor_mask);

        res = art_tree_destroy(&t);
        ASSERT_TRUE(res == 0);
}


typedef struct {
    int count;
    int max_count;
    const char **expected;
} prefix_data;

static int test_prefix_cb(void *data, const unsigned char *k, uint32_t k_len, void *val) {
    prefix_data *p = (prefix_data*)data;
    assert(p->count < p->max_count);
    assert(memcmp(k, p->expected[p->count], k_len) == 0);
    p->count++;
    return 0;
}

TEST(ArtTest, test_art_iter_prefix) {
        art_tree t;
        int res = art_tree_init(&t);
        ASSERT_TRUE(res == 0);

        const char *s = "api.foo.bar";
        art_document doc = get_document((uint32_t) 1);
        ASSERT_TRUE(NULL == art_insert(&t, (unsigned char*)s, strlen(s)+1, &doc, 1));

        s = "api.foo.baz";
        doc = get_document((uint32_t) 2);
        ASSERT_TRUE(NULL == art_insert(&t, (unsigned char*)s, strlen(s)+1, &doc, 1));

        s = "api.foe.fum";
        doc = get_document((uint32_t) 3);
        ASSERT_TRUE(NULL == art_insert(&t, (unsigned char*)s, strlen(s)+1, &doc, 1));

        s = "abc.123.456";
        doc = get_document((uint32_t) 4);
        ASSERT_TRUE(NULL == art_insert(&t, (unsigned char*)s, strlen(s)+1, &doc, 1));

        s = "api.foo";
        doc = get_document((uint32_t) 5);
        ASSERT_TRUE(NULL == art_insert(&t, (unsigned char*)s, strlen(s)+1, &doc, 1));

        s = "api";
        doc = get_document((uint32_t) 6);
        ASSERT_TRUE(NULL == art_insert(&t, (unsigned char*)s, strlen(s)+1, &doc, 1));

        // Iterate over api
        const char *expected[] = {"api", "api.foe.fum", "api.foo", "api.foo.bar", "api.foo.baz"};
        prefix_data p = { 0, 5, expected };
        ASSERT_TRUE(!art_iter_prefix(&t, (unsigned char*)"api", 3, test_prefix_cb, &p));
        ASSERT_TRUE(p.count == p.max_count);

        // Iterate over 'a'
        const char *expected2[] = {"abc.123.456", "api", "api.foe.fum", "api.foo", "api.foo.bar", "api.foo.baz"};
        prefix_data p2 = { 0, 6, expected2 };
        ASSERT_TRUE(!art_iter_prefix(&t, (unsigned char*)"a", 1, test_prefix_cb, &p2));
        ASSERT_TRUE(p2.count == p2.max_count);

        // Check a failed iteration
        prefix_data p3 = { 0, 0, NULL };
        ASSERT_TRUE(!art_iter_prefix(&t, (unsigned char*)"b", 1, test_prefix_cb, &p3));
        ASSERT_TRUE(p3.count == 0);

        // Iterate over api.
        const char *expected4[] = {"api.foe.fum", "api.foo", "api.foo.bar", "api.foo.baz"};
        prefix_data p4 = { 0, 4, expected4 };
        ASSERT_TRUE(!art_iter_prefix(&t, (unsigned char*)"api.", 4, test_prefix_cb, &p4));
        ASSERT_TRUE(p4.count == p4.max_count);

        // Iterate over api.foo.ba
        const char *expected5[] = {"api.foo.bar"};
        prefix_data p5 = { 0, 1, expected5 };
        ASSERT_TRUE(!art_iter_prefix(&t, (unsigned char*)"api.foo.bar", 11, test_prefix_cb, &p5));
        ASSERT_TRUE(p5.count == p5.max_count);

        // Check a failed iteration on api.end
        prefix_data p6 = { 0, 0, NULL };
        ASSERT_TRUE(!art_iter_prefix(&t, (unsigned char*)"api.end", 7, test_prefix_cb, &p6));
        ASSERT_TRUE(p6.count == 0);

        // Iterate over empty prefix
        prefix_data p7 = { 0, 6, expected2 };
        ASSERT_TRUE(!art_iter_prefix(&t, (unsigned char*)"", 0, test_prefix_cb, &p7));
        ASSERT_TRUE(p7.count == p7.max_count);

        res = art_tree_destroy(&t);
        ASSERT_TRUE(res == 0);
}


TEST(ArtTest, test_art_long_prefix) {
    art_tree t;
    int res = art_tree_init(&t);
    ASSERT_TRUE(res == 0);

    uintptr_t id;
    const char *s;

    s = "this:key:has:a:long:prefix:3";
    id = 3;
    art_document doc = get_document((uint32_t) id);
    ASSERT_TRUE(NULL == art_insert(&t, (unsigned char*)s, strlen(s)+1, &doc, 1));

    s = "this:key:has:a:long:common:prefix:2";
    id = 2;
    doc = get_document((uint32_t) id);
    ASSERT_TRUE(NULL == art_insert(&t, (unsigned char*)s, strlen(s)+1, &doc, 1));

    s = "this:key:has:a:long:common:prefix:1";
    id = 1;
    doc = get_document((uint32_t) id);
    ASSERT_TRUE(NULL == art_insert(&t, (unsigned char*)s, strlen(s)+1, &doc, 1));

    // Search for the keys
    s = "this:key:has:a:long:common:prefix:1";
    EXPECT_EQ(1, (((art_leaf *)art_search(&t, (unsigned char*)s, strlen(s)+1))->values->ids.at(0)));

    s = "this:key:has:a:long:common:prefix:2";
    EXPECT_EQ(2, (((art_leaf *)art_search(&t, (unsigned char*)s, strlen(s)+1))->values->ids.at(0)));

    s = "this:key:has:a:long:prefix:3";
    EXPECT_EQ(3, (((art_leaf *)art_search(&t, (unsigned char*)s, strlen(s)+1))->values->ids.at(0)));

    const char *expected[] = {
            "this:key:has:a:long:common:prefix:1",
            "this:key:has:a:long:common:prefix:2",
            "this:key:has:a:long:prefix:3",
    };
    prefix_data p = { 0, 3, expected };
    ASSERT_TRUE(!art_iter_prefix(&t, (unsigned char*)"this:key:has", 12, test_prefix_cb, &p));
    ASSERT_TRUE(p.count == p.max_count);

    res = art_tree_destroy(&t);
    ASSERT_TRUE(res == 0);
}

TEST(ArtTest, test_art_insert_search_uuid) {
    art_tree t;
    int res = art_tree_init(&t);
    ASSERT_TRUE(res == 0);

    int len;
    char buf[512];
    FILE *f = fopen("/tmp/typesense_test/uuid.txt", "r");

    uintptr_t line = 1;
    while (fgets(buf, sizeof buf, f)) {
        len = strlen(buf);
        buf[len-1] = '\0';
        art_document doc = get_document((uint32_t) line);
        ASSERT_TRUE(NULL == art_insert(&t, (unsigned char*)buf, len, &doc, 1));
        line++;
    }

    // Seek back to the start
    fseek(f, 0, SEEK_SET);

    // Search for each line
    line = 1;
    while (fgets(buf, sizeof buf, f)) {
        len = strlen(buf);
        buf[len-1] = '\0';

        uintptr_t id = ((art_leaf*)art_search(&t, (unsigned char*)buf, len))->values->ids.at(0);
        ASSERT_TRUE(line == id);
        line++;
    }

    // Check the minimum
    art_leaf *l = art_minimum(&t);
    ASSERT_TRUE(l && strcmp((char*)l->key, "00026bda-e0ea-4cda-8245-522764e9f325") == 0);

    // Check the maximum
    l = art_maximum(&t);
    ASSERT_TRUE(l && strcmp((char*)l->key, "ffffcb46-a92e-4822-82af-a7190f9c1ec5") == 0);

    res = art_tree_destroy(&t);
    ASSERT_TRUE(res == 0);
}

TEST(ArtTest, test_art_max_prefix_len_scan_prefix) {
    art_tree t;
    int res = art_tree_init(&t);
    ASSERT_TRUE(res == 0);

    const char* key1 = "foobarbaz1-test1-foo";
    art_document doc = get_document((uint32_t) 1);
    ASSERT_TRUE(NULL == art_insert(&t, (unsigned char*)key1, strlen(key1)+1, &doc, 1));

    const char *key2 = "foobarbaz1-test1-bar";
    doc = get_document((uint32_t) 2);
    ASSERT_TRUE(NULL == art_insert(&t, (unsigned char*)key2, strlen(key2)+1, &doc, 1));

    const char *key3 = "foobarbaz1-test2-foo";
    doc = get_document((uint32_t) 3);
    ASSERT_TRUE(NULL == art_insert(&t, (unsigned char*)key3, strlen(key3)+1, &doc, 2));

    ASSERT_TRUE(art_size(&t) == 3);

    // Iterate over api
    const char *expected[] = {key2, key1};
    prefix_data p = { 0, 2, expected };
    const char *prefix = "foobarbaz1-test1";
    ASSERT_TRUE(!art_iter_prefix(&t, (unsigned char*)prefix, strlen(prefix), test_prefix_cb, &p));
    ASSERT_TRUE(p.count == p.max_count);

    res = art_tree_destroy(&t);
    ASSERT_TRUE(res == 0);
}

TEST(ArtTest, test_art_prefix_iter_out_of_bounds) {
    // Regression: ensures that `assert(depth < key_len);` is not invoked
    art_tree t;
    int res = art_tree_init(&t);
    ASSERT_TRUE(res == 0);

    const char* key1 = "foobarbaz1-long-test1-foo";
    art_document doc = get_document((uint32_t) 1);
    ASSERT_TRUE(NULL == art_insert(&t, (unsigned char*)key1, strlen(key1)+1, &doc, 1));

    const char *key2 = "foobarbaz1-long-test1-bar";
    doc = get_document((uint32_t) 2);
    ASSERT_TRUE(NULL == art_insert(&t, (unsigned char*)key2, strlen(key2)+1, &doc, 1));

    const char *key3 = "foobarbaz1-long-test2-foo";
    doc = get_document((uint32_t) 3);
    ASSERT_TRUE(NULL == art_insert(&t, (unsigned char*)key3, strlen(key3)+1, &doc, 2));

    ASSERT_TRUE(art_size(&t) == 3);

    // Iterate over api
    const char *expected[] = {key2, key1};
    prefix_data p = { 0, 0, expected };
    const char *prefix = "f2oobar";
    ASSERT_TRUE(!art_iter_prefix(&t, (unsigned char*)prefix, strlen(prefix), test_prefix_cb, &p));
    ASSERT_TRUE(p.count == p.max_count);

    res = art_tree_destroy(&t);
    ASSERT_TRUE(res == 0);
}

TEST(ArtTest, test_art_search_out_of_bounds) {
    // Regression: ensures that `assert(depth < key_len);` is not invoked
    art_tree t;
    int res = art_tree_init(&t);
    ASSERT_TRUE(res == 0);

    const char* key1 = "foobarbaz1-long-test1-foo";
    art_document doc = get_document((uint32_t) 1);
    ASSERT_TRUE(NULL == art_insert(&t, (unsigned char*)key1, strlen(key1)+1, &doc, 1));

    const char *key2 = "foobarbaz1-long-test1-bar";
    doc = get_document((uint32_t) 2);
    ASSERT_TRUE(NULL == art_insert(&t, (unsigned char*)key2, strlen(key2)+1, &doc, 1));

    const char *key3 = "foobarbaz1-long-test2-foo";
    doc = get_document((uint32_t) 3);
    ASSERT_TRUE(NULL == art_insert(&t, (unsigned char*)key3, strlen(key3)+1, &doc, 2));

    ASSERT_TRUE(art_size(&t) == 3);

    // Search for a non-existing key
    const char *prefix = "foobarbaz1-long-";
    art_leaf* l = (art_leaf *) art_search(&t, (const unsigned char *) prefix, strlen(prefix));
    ASSERT_EQ(NULL, l);

    res = art_tree_destroy(&t);
    ASSERT_TRUE(res == 0);
}

TEST(ArtTest, test_art_delete_out_of_bounds) {
    // Regression: ensures that `assert(depth < key_len);` is not invoked
    art_tree t;
    int res = art_tree_init(&t);
    ASSERT_TRUE(res == 0);

    const char* key1 = "foobarbaz1-long-test1-foo";
    art_document doc = get_document((uint32_t) 1);
    ASSERT_TRUE(NULL == art_insert(&t, (unsigned char*)key1, strlen(key1)+1, &doc, 1));

    const char *key2 = "foobarbaz1-long-test1-bar";
    doc = get_document((uint32_t) 2);
    ASSERT_TRUE(NULL == art_insert(&t, (unsigned char*)key2, strlen(key2)+1, &doc, 1));

    const char *key3 = "foobarbaz1-long-test2-foo";
    doc = get_document((uint32_t) 3);
    ASSERT_TRUE(NULL == art_insert(&t, (unsigned char*)key3, strlen(key3)+1, &doc, 2));

    ASSERT_TRUE(art_size(&t) == 3);

    // Try to delete a non-existing key
    const char *prefix = "foobarbaz1-long-";
    art_values* values = (art_values *) art_delete(&t, (const unsigned char *) prefix, strlen(prefix));
    ASSERT_EQ(NULL, values);

    res = art_tree_destroy(&t);
    ASSERT_TRUE(res == 0);
}

TEST(ArtTest, test_art_insert_multiple_ids_for_same_token) {
    art_tree t;
    int res = art_tree_init(&t);
    ASSERT_TRUE(res == 0);

    const char* key1 = "implement";
    art_document doc = get_document((uint32_t) 1);
    ASSERT_TRUE(NULL == art_insert(&t, (unsigned char*)key1, strlen(key1)+1, &doc, 1));

    doc = get_document((uint32_t) 2);
    art_values* value = (art_values*) art_insert(&t, (unsigned char*)key1, strlen(key1) + 1, &doc, 2);
    ASSERT_TRUE(value != NULL);

    doc = get_document((uint32_t) 3);
    ASSERT_TRUE(value == (art_values*) art_insert(&t, (unsigned char*)key1, strlen(key1)+1, &doc, 3));

    ASSERT_TRUE(art_size(&t) == 1);
    ASSERT_EQ(value->ids.getLength(), 3);
    ASSERT_EQ(value->ids.at(0), 1);
    ASSERT_EQ(value->ids.at(1), 2);
    ASSERT_EQ(value->ids.at(2), 3);

    res = art_tree_destroy(&t);
    ASSERT_TRUE(res == 0);
}

TEST(ArtTest, test_art_fuzzy_search_single_leaf) {
    art_tree t;
    int res = art_tree_init(&t);
    ASSERT_TRUE(res == 0);

    const char* implement_key = "implement";
    art_document doc = get_document((uint32_t) 1);
    ASSERT_TRUE(NULL == art_insert(&t, (unsigned char*)implement_key, strlen(implement_key)+1, &doc, 1));

    art_leaf* l = (art_leaf *) art_search(&t, (const unsigned char *)implement_key, strlen(implement_key)+1);
    EXPECT_EQ(1, l->values->ids.at(0));

    std::vector<art_leaf*> leaves;
    art_fuzzy_search(&t, (const unsigned char *) implement_key, strlen(implement_key) + 1, 0, 10, FREQUENCY, false, leaves);
    ASSERT_EQ(1, leaves.size());

    const char* implement_key_typo1 = "implment";
    const char* implement_key_typo2 = "implwnent";

    leaves.clear();
    art_fuzzy_search(&t, (const unsigned char *) implement_key_typo1, strlen(implement_key_typo1) + 1, 0, 10, FREQUENCY, false, leaves);
    ASSERT_EQ(0, leaves.size());

    leaves.clear();
    art_fuzzy_search(&t, (const unsigned char *) implement_key_typo1, strlen(implement_key_typo1) + 1, 1, 10, FREQUENCY, false, leaves);
    ASSERT_EQ(1, leaves.size());

    leaves.clear();
    art_fuzzy_search(&t, (const unsigned char *) implement_key_typo2, strlen(implement_key_typo2) + 1, 2, 10, FREQUENCY, false, leaves);
    ASSERT_EQ(1, leaves.size());

    res = art_tree_destroy(&t);
    ASSERT_TRUE(res == 0);
}

TEST(ArtTest, test_art_fuzzy_search) {
    art_tree t;
    int res = art_tree_init(&t);
    ASSERT_TRUE(res == 0);

    int len;
    char buf[512];
    FILE *f = fopen("/tmp/typesense_test/words.txt", "r");

    uint64_t xor_mask = 0;
    uintptr_t line = 1, nlines;
    while (fgets(buf, sizeof buf, f)) {
        len = strlen(buf);
        buf[len-1] = '\0';
        art_document doc = get_document((uint32_t) line);
        ASSERT_TRUE(NULL == art_insert(&t, (unsigned char*)buf, len, &doc, 1));

        xor_mask ^= (line * (buf[0] + len));
        line++;
    }

    nlines = line - 1;

    std::vector<art_leaf*> leaves;

    // transpose
    art_fuzzy_search(&t, (const unsigned char *) "zymosthneic", strlen("zymosthneic") + 1, 1, 10, FREQUENCY, false, leaves);
    ASSERT_EQ(1, leaves.size());
    ASSERT_STREQ("zymosthenic", (const char *)leaves.at(0)->key);

    // transpose + missing
    leaves.clear();
    art_fuzzy_search(&t, (const unsigned char *) "dacrcyystlgia", strlen("dacrcyystlgia") + 1, 2, 10, FREQUENCY, false, leaves);
    ASSERT_EQ(1, leaves.size());
    ASSERT_STREQ("dacrycystalgia", (const char *)leaves.at(0)->key);

    // missing char
    leaves.clear();
    art_fuzzy_search(&t, (const unsigned char *) "gaberlunze", strlen("gaberlunze") + 1, 1, 10, FREQUENCY, false, leaves);
    ASSERT_EQ(1, leaves.size());
    ASSERT_STREQ("gaberlunzie", (const char *)leaves.at(0)->key);

    // extra char
    leaves.clear();
    art_fuzzy_search(&t, (const unsigned char *) "higghliving", strlen("higghliving") + 1, 1, 10, FREQUENCY, false, leaves);
    ASSERT_EQ(1, leaves.size());
    ASSERT_STREQ("highliving", (const char *)leaves.at(0)->key);

    // substituted char
    leaves.clear();
    art_fuzzy_search(&t, (const unsigned char *) "eacemiferous", strlen("eacemiferous") + 1, 1, 10, FREQUENCY, false, leaves);
    ASSERT_EQ(1, leaves.size());
    ASSERT_STREQ("racemiferous", (const char *)leaves.at(0)->key);

    // missing char + extra char
    leaves.clear();
    art_fuzzy_search(&t, (const unsigned char *) "Sarbruckken", strlen("Sarbruckken") + 1, 2, 10, FREQUENCY, false, leaves);
    ASSERT_EQ(1, leaves.size());
    ASSERT_STREQ("Saarbrucken", (const char *)leaves.at(0)->key);

    // multiple matching results
    leaves.clear();
    art_fuzzy_search(&t, (const unsigned char *) "hown", strlen("hown") + 1, 1, 10, FREQUENCY, false, leaves);
    ASSERT_EQ(10, leaves.size());

    std::vector<const char*> words = {"town", "sown", "shown", "own", "mown", "lown", "howl", "howk", "howe", "how"};
    for(auto leaf_index = 0; leaf_index < leaves.size(); leaf_index++) {
        ASSERT_STREQ(words.at(leaf_index), (const char *)leaves.at(leaf_index)->key);
    }

    res = art_tree_destroy(&t);
    ASSERT_TRUE(res == 0);
}

TEST(ArtTest, test_encode_int) {
    unsigned char chars[9];

    // 175 => 0000,0000,0000,0000,0000,0000,1010,1111,\0
    unsigned char chars_175[9] = {0, 0, 0, 0, 0, 0, 10, 15, 46};
    encode_int(175, chars);
    for(uint32_t i = 0; i < 9; i++) {
        ASSERT_EQ(chars_175[i], chars[i]);
    }

    // 0 => 0000,0000,0000,0000,0000,0000,0000,0000,\0
    unsigned char chars_0[9] = {0, 0, 0, 0, 0, 0, 0, 0, 46};
    encode_int(0, chars);
    for(uint32_t i = 0; i < 9; i++) {
        ASSERT_EQ(chars_0[i], chars[i]);
    }

    // 255 => 0000,0000,0000,0000,0000,0000,1111,1111,\0
    unsigned char chars_255[9] = {0, 0, 0, 0, 0, 0, 15, 15, 46};
    encode_int(255, chars);
    for(uint32_t i = 0; i < 9; i++) {
        ASSERT_EQ(chars_255[i], chars[i]);
    }

    // 4531 => 0000,0000,0000,0000,0001,0001,1011,0011,\0
    unsigned char chars_4531[9] = {0, 0, 0, 0, 1, 1, 11, 3, 46};
    encode_int(4531, chars);
    for(uint32_t i = 0; i < 9; i++) {
        ASSERT_EQ(chars_4531[i], chars[i]);
    }

    // 1200000 => 0000,0000,0001,0010,0100,1111,1000,0000,\0
    unsigned char chars_1M[9] = {0, 0, 1, 2, 4, 15, 8, 0, 46};
    encode_int(1200000, chars);
    for(uint32_t i = 0; i < 9; i++) {
        ASSERT_EQ(chars_1M[i], chars[i]);
    }
}

TEST(ArtTest, test_int_range_hundreds) {
    art_tree t;
    art_tree_init(&t);

    art_document doc = get_document(1);
    const int CHAR_LEN = 9;
    unsigned char chars[CHAR_LEN];

    for(uint32_t i = 100; i < 110; i++) {
        encode_int(i, chars);
        ASSERT_TRUE(NULL == art_insert(&t, (unsigned char*)chars, CHAR_LEN, &doc, 1));
    }

    encode_int(106, chars);

    std::vector<const art_leaf*> results;

    int res = art_int_search(&t, 106, 0, results);
    ASSERT_TRUE(res == 0);
    ASSERT_EQ(1, results.size());
    results.clear();

    res = art_int_search(&t, 106, 1, results);
    ASSERT_TRUE(res == 0);
    ASSERT_EQ(4, results.size());
    results.clear();

    res = art_int_search(&t, 106, -1, results);
    ASSERT_TRUE(res == 0);
    ASSERT_EQ(7, results.size());

    res = art_tree_destroy(&t);
    ASSERT_TRUE(res == 0);
}

TEST(ArtTest, test_int_range_millions) {
    art_tree t;
    art_tree_init(&t);

    art_document doc = get_document(1);
    
    const int CHAR_LEN = 9;
    unsigned char chars[CHAR_LEN];

    for(uint32_t i = 0; i < 1000000; i++) {
        encode_int(i, chars);
        ASSERT_TRUE(NULL == art_insert(&t, (unsigned char*)chars, CHAR_LEN, &doc, 1));
    }

    encode_int(5, chars);
    /*std::cout << std::endl;
    for(uint32_t i = 0; i < CHAR_LEN; i++) {
        std::cout << (int)chars[i] << ", ";
    }
    std::cout << std::endl;*/

    std::vector<const art_leaf*> results;

    // ==
    for(uint32_t i = 0; i < 6; i++) {
        results.clear();
        art_int_search(&t, (uint32_t) pow(10, i), 0, results);
        ASSERT_EQ(1, results.size());

        results.clear();
        art_int_search(&t, (uint32_t) (pow(10, i) + 7), 0, results);
        ASSERT_EQ(1, results.size());
    }

    results.clear();
    art_int_search(&t, 1000000-1, 0, results);
    ASSERT_EQ(1, results.size());

    // >=
    results.clear();
    art_int_search(&t, 1000000-5, 1, results);
    ASSERT_EQ(5, results.size());

    results.clear();
    art_int_search(&t, 1000000-1, 1, results);
    ASSERT_EQ(1, results.size());

    results.clear();
    art_int_search(&t, 1000000, 1, results);
    ASSERT_EQ(0, results.size());

    results.clear();
    art_int_search(&t, 5, 1, results);
    ASSERT_EQ(1000000-5, results.size());

    // <=
    results.clear();
    art_int_search(&t, 1000000-5, -1, results);
    ASSERT_EQ(1000000-5+1, results.size());

    results.clear();
    art_int_search(&t, 1000000-1, -1, results);
    ASSERT_EQ(1000000, results.size());

    results.clear();
    art_int_search(&t, 1000000, -1, results);
    ASSERT_EQ(1000000, results.size());

    results.clear();
    art_int_search(&t, 5, -1, results);
    ASSERT_EQ(5+1, results.size());
}

TEST(ArtTest, test_int_range_byte_boundary) {
    art_tree t;
    art_tree_init(&t);

    art_document doc = get_document(1);

    const int CHAR_LEN = 9;
    unsigned char chars[CHAR_LEN];

    for(uint32_t i = 200; i < 300; i++) {
        encode_int(i, chars);
        ASSERT_TRUE(NULL == art_insert(&t, (unsigned char*)chars, CHAR_LEN, &doc, 1));
    }

    encode_int(255, chars);
    std::vector<const art_leaf*> results;

    results.clear();
    art_int_search(&t, 255, 1, results);
    ASSERT_EQ(45, results.size());

    /*std::cout << std::endl;
    for(auto i = 0; i < 1; i++) {
        auto result = results[i];
        for(auto j = 0; j < result->key_len; j++) {
            std::cout << (int) result->key[j] << ", ";
        }

        std::cout << std::endl;
    }*/
}