#include <iostream>
#include <cstring>
#include <climits>
#include <cassert>
#include <unistd.h>
#include "gtest/gtest.h"

void *smalloc(size_t size);

void *scalloc(size_t num, size_t size);

void sfree(void *p);

void *srealloc(void *oldp, size_t size);

size_t _num_free_blocks();

size_t _num_free_bytes();

size_t _num_allocated_blocks();

size_t _num_allocated_bytes();

size_t _num_meta_data_bytes();

size_t _size_meta_data();

/**
 * This function is used to reset all the counters.
 * It is used before each test.
 */
void _reset_all();


void resrt_to_init_config() {
    _reset_all();
    size_t metadata_size = _size_meta_data();
    EXPECT_GE(metadata_size, 0);
    EXPECT_EQ(_num_allocated_blocks(), 0);
    EXPECT_EQ(_num_allocated_bytes(), 0);
    EXPECT_EQ(_num_free_blocks(), 0);
    EXPECT_EQ(_num_free_bytes(), 0);
    EXPECT_EQ(_num_meta_data_bytes(), 0);
}

TEST(test_part2, test_init_config) {
    size_t metadata_size = _size_meta_data();
    EXPECT_GE(metadata_size, 0);
    EXPECT_EQ(_num_allocated_blocks(), 0);
    EXPECT_EQ(_num_allocated_bytes(), 0);
    EXPECT_EQ(_num_free_blocks(), 0);
    EXPECT_EQ(_num_free_bytes(), 0);
    EXPECT_EQ(_num_meta_data_bytes(), 0);
    resrt_to_init_config();
//    GTEST_FLAG_SET(death_test_style, "fast");
//    EXPECT_EXIT(resrt_to_init_config(), testing::ExitedWithCode(0), "");
}

void test_basic_malloc() {
    resrt_to_init_config();
    size_t metadata_size = _size_meta_data();
    void *p1 = smalloc(10);
    EXPECT_EQ(_num_allocated_blocks(), 1);
    EXPECT_EQ(_num_allocated_bytes(), 10);
    EXPECT_EQ(_num_free_blocks(), 0);
    EXPECT_EQ(_num_free_bytes(), 0);
    EXPECT_EQ(_num_meta_data_bytes(), metadata_size);
    EXPECT_EQ(_num_allocated_blocks(), 1);
    EXPECT_TRUE(p1 != nullptr);
    memccpy(p1, "hello", 5, 5);
    EXPECT_STREQ((char *) p1, "hello");
//    exit(0);
}

TEST(test_part2, test_basic_malloc) {
    test_basic_malloc();
//    GTEST_FLAG_SET(death_test_style, "fast");
//    EXPECT_EXIT(test_basic_malloc(), testing::ExitedWithCode(0), "");
}

void test_basic_calloc() {
    resrt_to_init_config();
    size_t metadata_size = _size_meta_data();
    void *p1 = scalloc(10, 10);
    EXPECT_EQ(_num_allocated_blocks(), 1);
    EXPECT_EQ(_num_allocated_bytes(), 100);
    EXPECT_EQ(_num_free_blocks(), 0);
    EXPECT_EQ(_num_free_bytes(), 0);
    EXPECT_EQ(_num_meta_data_bytes(), metadata_size);
    EXPECT_EQ(_num_allocated_blocks(), 1);
    EXPECT_TRUE(p1 != nullptr);
    for (int i = 0; i < 100; i++) {
        EXPECT_TRUE(((char *) p1)[i] == 0);
    }
    memccpy(p1, "https://www.youtube.com/watch?v=dQw4w9WgXcQ&pp=ygUIcmlja3JvbGw%3D", 66, 66);
    EXPECT_STREQ((char *) p1, "https://www.youtube.com/watch?v=dQw4w9WgXcQ&pp=ygUIcmlja3JvbGw%3D");
    for (int i = 66; i < 100; i++) {
        EXPECT_TRUE(((char *) p1)[i] == 0);
    }
//    exit(0);
}

TEST(test_part2, test_basic_calloc) {
    test_basic_calloc();
//    GTEST_FLAG_SET(death_test_style, "fast");
//    EXPECT_EXIT(test_basic_calloc(), testing::ExitedWithCode(0), "");
}

void test_basic_realloc_with_size_not_sufficient() {
    resrt_to_init_config();
    size_t metadata_size = _size_meta_data();
    void *p1 = smalloc(10);
    EXPECT_EQ(_num_allocated_blocks(), 1);
    EXPECT_EQ(_num_allocated_bytes(), 10);
    EXPECT_EQ(_num_free_blocks(), 0);
    EXPECT_EQ(_num_free_bytes(), 0);
    EXPECT_EQ(_num_meta_data_bytes(), metadata_size);
    EXPECT_EQ(_num_allocated_blocks(), 1);
    EXPECT_TRUE(p1 != nullptr);
    memccpy(p1, "hello", 5, 5);
    EXPECT_STREQ((char *) p1, "hello");
    void *p2 = srealloc(p1, 20);
    EXPECT_EQ(_num_allocated_blocks(), 2);
    EXPECT_EQ(_num_allocated_bytes(), 30);
    EXPECT_EQ(_num_free_blocks(), 1);
    EXPECT_EQ(_num_free_bytes(), 10);
    EXPECT_EQ(_num_meta_data_bytes(), metadata_size * 2);
    EXPECT_EQ(_num_allocated_blocks(), 2);
    EXPECT_TRUE(p2 != nullptr);
    EXPECT_TRUE(p1 != p2);
    EXPECT_STREQ((char *) p2, "hello");
    memccpy(p2, "hello world", 11, 11);
    EXPECT_STREQ((char *) p2, "hello world");
//    exit(0);
}

TEST(test_part2, test_basic_realloc_with_size_not_sufficient) {
    test_basic_realloc_with_size_not_sufficient();
//    GTEST_FLAG_SET(death_test_style, "fast");
//    EXPECT_EXIT(test_basic_realloc_with_size_not_sufficient(), testing::ExitedWithCode(0), "");
}

void test_basic_realloc_with_sufficient_size() {
    resrt_to_init_config();
    size_t metadata_size = _size_meta_data();
    void *p1 = smalloc(30);
    EXPECT_EQ(_num_allocated_blocks(), 1);
    EXPECT_EQ(_num_allocated_bytes(), 30);
    EXPECT_EQ(_num_free_blocks(), 0);
    EXPECT_EQ(_num_free_bytes(), 0);
    EXPECT_EQ(_num_meta_data_bytes(), metadata_size);
    EXPECT_EQ(_num_allocated_blocks(), 1);
    EXPECT_TRUE(p1 != nullptr);
    memccpy(p1, "hello", 5, 5);
    EXPECT_STREQ((char *) p1, "hello");
    void *p2 = srealloc(p1, 20);
    EXPECT_EQ(_num_allocated_blocks(), 1);
    EXPECT_EQ(_num_allocated_bytes(), 30);
    EXPECT_EQ(_num_free_blocks(), 0);
    EXPECT_EQ(_num_free_bytes(), 0);
    EXPECT_EQ(_num_meta_data_bytes(), metadata_size);
    EXPECT_EQ(_num_allocated_blocks(), 1);
    EXPECT_TRUE(p2 != nullptr);
    EXPECT_TRUE(p1 == p2);
    EXPECT_STREQ((char *) p2, "hello");
    memccpy(p2, "hello world", 11, 11);
    EXPECT_STREQ((char *) p2, "hello world");
    EXPECT_STREQ((char *) p1, "hello world"); // since p1 == p2
//    exit(0);
}

TEST(test_part2, test_basic_realloc_with_sufficient_size) {
    test_basic_realloc_with_sufficient_size();
//    GTEST_FLAG_SET(death_test_style, "fast");
//    EXPECT_EXIT(test_basic_realloc_with_sufficient_size(), testing::ExitedWithCode(0), "");
}

void test_zero_calls() {
    resrt_to_init_config();
    EXPECT_EQ(_num_allocated_blocks(), 0);
    EXPECT_EQ(_num_allocated_bytes(), 0);
    EXPECT_EQ(_num_free_blocks(), 0);
    EXPECT_EQ(_num_free_bytes(), 0);
    EXPECT_EQ(_num_meta_data_bytes(), 0);
    EXPECT_EQ(_num_allocated_blocks(), 0);
    void *p = smalloc(0);
    EXPECT_TRUE(p == nullptr);
    p = scalloc(0, 0);
    EXPECT_TRUE(p == nullptr);
    p = scalloc(5, 0);
    EXPECT_TRUE(p == nullptr);
    p = scalloc(0, 5);
    EXPECT_TRUE(p == nullptr);
    EXPECT_NO_THROW(sfree(nullptr));
//    exit(0);
}

TEST(test_part2, test_zero_calls) {
    test_zero_calls();
//    GTEST_FLAG_SET(death_test_style, "fast");
//    EXPECT_EXIT(test_zero_calls(), testing::ExitedWithCode(0), "");
}

void test_over_max_calls() {
    resrt_to_init_config();
    size_t size = (1e8) + 1;
    EXPECT_EQ(_num_allocated_blocks(), 0);
    EXPECT_EQ(_num_allocated_bytes(), 0);
    EXPECT_EQ(_num_free_blocks(), 0);
    EXPECT_EQ(_num_free_bytes(), 0);
    EXPECT_EQ(_num_meta_data_bytes(), 0);
    EXPECT_EQ(_num_allocated_blocks(), 0);
    void *p = smalloc(size);
    EXPECT_TRUE(p == nullptr);
    p = scalloc(size, 1);
    EXPECT_TRUE(p == nullptr);
    p = scalloc(1, size);
    EXPECT_TRUE(p == nullptr);
    p = scalloc(size, size);
    EXPECT_TRUE(p == nullptr);
    p = smalloc(LLONG_MAX);
    EXPECT_TRUE(p == nullptr);
    p = scalloc(LLONG_MAX, 1);
    EXPECT_TRUE(p == nullptr);
    p = scalloc(1, LLONG_MAX);
    EXPECT_TRUE(p == nullptr);
    p = scalloc(LLONG_MAX, LLONG_MAX);
    EXPECT_TRUE(p == nullptr);
//    exit(0);
}

TEST(test_part2, test_over_max_calls) {
    test_over_max_calls();
//    GTEST_FLAG_SET(death_test_style, "fast");
//    EXPECT_EXIT(test_over_max_calls(), testing::ExitedWithCode(0), "");
}

void test_free_in_different_spots() {
    resrt_to_init_config();
    size_t base_size = 10;
    size_t num_of_blocks = 10;
    void **p = new void *[num_of_blocks];
    size_t metadata_size = _size_meta_data();
    for (size_t i = 0; i < num_of_blocks; ++i) {
        size_t size_to_alloc = base_size * (i + 1);
        p[i] = smalloc(size_to_alloc);
        EXPECT_TRUE(p[i] != nullptr);
        EXPECT_EQ(_num_allocated_blocks(), i + 1);
        // 0.5*(i+1)*(size_to_alloc + base_size) = sum of arithmetic series
        EXPECT_EQ(_num_allocated_bytes(), 0.5 * (i + 1) * (size_to_alloc + base_size));
        EXPECT_EQ(_num_free_blocks(), 0);
        EXPECT_EQ(_num_free_bytes(), 0);
        EXPECT_EQ(_num_meta_data_bytes(), (i + 1) * metadata_size);
    }
    size_t total_size_allocated = _num_allocated_bytes(); // it's ok since we checked it in the loop
    size_t current_free_blocks = _num_free_blocks();
    size_t current_free_bytes = _num_free_bytes();
    size_t current_meta_data_bytes = _num_meta_data_bytes();
    size_t current_allocated_blocks = _num_allocated_blocks();
    for (size_t i = 0; i < num_of_blocks; i += 2) {
        sfree(p[i]);
        current_free_blocks++;
        current_free_bytes += base_size * (i + 1);
        EXPECT_EQ(_num_free_blocks(), current_free_blocks);
        EXPECT_EQ(_num_free_bytes(), current_free_bytes);
        EXPECT_EQ(_num_meta_data_bytes(), current_meta_data_bytes); // no change
        EXPECT_EQ(_num_allocated_blocks(), current_allocated_blocks); // no change
        EXPECT_EQ(_num_allocated_bytes(), total_size_allocated); // no change
    }


    for (size_t i = 0; i < num_of_blocks; ++i) {
        EXPECT_NO_THROW(sfree(p[i]));
    }
    delete[] p;
//    exit(0);
}

TEST(test_part2, test_free_in_different_spots) {
    test_free_in_different_spots();
//    GTEST_FLAG_SET(death_test_style, "fast");
//    EXPECT_EXIT(test_free_in_different_spots(), testing::ExitedWithCode(0), "");
}

void test_block_reuse() {
    resrt_to_init_config();
    size_t base_size = 10;
    size_t num_of_blocks = 10;
    void **p = new void *[num_of_blocks];
    size_t metadata_size = _size_meta_data();
    for (size_t i = 0; i < num_of_blocks; ++i) {
        size_t size_to_alloc = base_size;
        p[i] = smalloc(size_to_alloc);
        EXPECT_TRUE(p[i] != nullptr);
        EXPECT_EQ(_num_allocated_blocks(), i + 1);
        // 0.5*(i+1)*(size_to_alloc + base_size) = sum of arithmetic series
        EXPECT_EQ(_num_allocated_bytes(), (i + 1) * base_size);
        EXPECT_EQ(_num_free_blocks(), 0);
        EXPECT_EQ(_num_free_bytes(), 0);
        EXPECT_EQ(_num_meta_data_bytes(), (i + 1) * metadata_size);
    }
    size_t total_size_allocated = _num_allocated_bytes(); // it's ok since we checked it in the loop
    size_t current_free_blocks = _num_free_blocks();
    size_t current_free_bytes = _num_free_bytes();
    size_t current_meta_data_bytes = _num_meta_data_bytes();
    size_t current_allocated_blocks = _num_allocated_blocks();

    for (size_t i = 0; i < num_of_blocks; i += 2) {
        sfree(p[i]);
        current_free_blocks++;
        current_free_bytes += base_size;
        EXPECT_EQ(_num_free_blocks(), current_free_blocks);
        EXPECT_EQ(_num_free_bytes(), current_free_bytes);
        EXPECT_EQ(_num_meta_data_bytes(), current_meta_data_bytes); // no change
        EXPECT_EQ(_num_allocated_blocks(), current_allocated_blocks); // no change
        EXPECT_EQ(_num_allocated_bytes(), total_size_allocated); // no change
    }
    for (size_t i = 0; i < num_of_blocks; i += 2) {
        void *ptr = smalloc(base_size / 2);
        EXPECT_TRUE(ptr != nullptr);
        current_free_blocks--;
        current_free_bytes -= base_size; // since we use all the prv block and not only the half
        EXPECT_EQ(_num_free_blocks(), current_free_blocks);
        EXPECT_EQ(_num_free_bytes(), current_free_bytes);
        EXPECT_EQ(_num_meta_data_bytes(), current_meta_data_bytes); // no change
        EXPECT_EQ(_num_allocated_blocks(), current_allocated_blocks); // no change
        EXPECT_EQ(_num_allocated_bytes(), total_size_allocated); // no change
    }
    for (size_t i = 0; i < num_of_blocks; ++i) {
        EXPECT_NO_THROW(sfree(p[i]));
    }
    delete[] p;
//    exit(0);
}

TEST(test_part2, test_block_reuse) {
    test_block_reuse();
//    GTEST_FLAG_SET(death_test_style, "fast");
//    EXPECT_EXIT(test_block_reuse(), testing::ExitedWithCode(0), "");
}

void test_block_not_reuse_when_not_big_enough() {
    resrt_to_init_config();
    size_t base_size = 10;
    size_t num_of_blocks = 10;
    void **p = new void *[num_of_blocks];
    for (size_t i = 0; i < num_of_blocks; ++i) {
        size_t size_to_alloc = base_size;
        p[i] = smalloc(size_to_alloc);
        EXPECT_TRUE(p[i] != nullptr);
        EXPECT_EQ(_num_allocated_blocks(), i + 1);
    }
    size_t current_free_blocks = 0;
    size_t current_free_bytes = 0;
    for (size_t i = 0; i < num_of_blocks; i += 2) {
        sfree(p[i]);
        current_free_blocks++;
        current_free_bytes += base_size;
        EXPECT_EQ(_num_free_blocks(), current_free_blocks);
        EXPECT_EQ(_num_free_bytes(), current_free_bytes);
    }
    // this data is trusted, since it is tested in a different test
    size_t current_meta_data_bytes = _num_meta_data_bytes();
    size_t current_allocated_blocks = _num_allocated_blocks();
    size_t total_size_allocated = _num_allocated_bytes();
    for (size_t i = 0; i < num_of_blocks / 2; ++i) {
        smalloc(base_size * 2);
        current_meta_data_bytes += _size_meta_data();
        current_allocated_blocks++;
        total_size_allocated += base_size * 2;
        EXPECT_EQ(_num_free_blocks(), current_free_blocks); // no change, since we cant reuse the blocks
        EXPECT_EQ(_num_free_bytes(), current_free_bytes); // no change, since we cant reuse the blocks
        EXPECT_EQ(_num_meta_data_bytes(), current_meta_data_bytes);
        EXPECT_EQ(_num_allocated_blocks(), current_allocated_blocks);
        EXPECT_EQ(_num_allocated_bytes(), total_size_allocated);
    }
    for (size_t i = 0; i < num_of_blocks; ++i) {
        EXPECT_NO_THROW(sfree(p[i]));
    }
    delete[] p;
}

TEST(test_part2, test_block_not_reuse_when_not_big_enough) {
    test_block_not_reuse_when_not_big_enough();
}

void test_search_in_asc_order() {
    resrt_to_init_config();
    size_t base_size = 10;
    void *p1 = smalloc(base_size);
    void *p2 = smalloc(base_size * 2);
    void *p3 = smalloc(base_size * 3);
    EXPECT_NO_THROW(sfree(p2));
    EXPECT_NO_THROW(sfree(p3));
    EXPECT_EQ(_num_free_blocks(), 2);
    EXPECT_EQ(_num_free_bytes(), base_size * 5);
    EXPECT_EQ(_num_allocated_blocks(), 3);
    EXPECT_EQ(_num_allocated_bytes(), base_size * 6);
    void *p4 = smalloc(base_size * 2);
    EXPECT_EQ(p4, p2);
    EXPECT_EQ(_num_free_blocks(), 1);
    EXPECT_EQ(_num_free_bytes(), base_size * 3);
    EXPECT_EQ(_num_allocated_blocks(), 3);
    EXPECT_EQ(_num_allocated_bytes(), base_size * 6);
    void *p5 = smalloc(base_size * 4);
    EXPECT_NE(p5, p3); // since p3 is not big enough
    EXPECT_EQ(_num_free_blocks(), 1);
    EXPECT_EQ(_num_free_bytes(), base_size * 3);
    EXPECT_EQ(_num_allocated_blocks(), 4);
    EXPECT_EQ(_num_allocated_bytes(), base_size * 10);
    EXPECT_NO_THROW(sfree(p1));
    EXPECT_NO_THROW(sfree(p2));
    EXPECT_NO_THROW(sfree(p3));
    EXPECT_NO_THROW(sfree(p4));
    EXPECT_NO_THROW(sfree(p5));
}

TEST(test_part2, test_search_in_asc_order) {
    test_search_in_asc_order();
}

void test_search_in_asc_order_wasteful() {
    resrt_to_init_config();
    size_t base_size = 10;
    void *p1 = smalloc(base_size);
    void *p2 = smalloc(base_size * 3);
    void *p3 = smalloc(base_size * 2);
    EXPECT_NO_THROW(sfree(p2));
    EXPECT_NO_THROW(sfree(p3));
    EXPECT_EQ(_num_free_blocks(), 2);
    EXPECT_EQ(_num_free_bytes(), base_size * 5);
    EXPECT_EQ(_num_allocated_blocks(), 3);
    EXPECT_EQ(_num_allocated_bytes(), base_size * 6);
    void *p4 = smalloc(base_size * 2);
    EXPECT_EQ(p4, p2);
    EXPECT_EQ(_num_free_blocks(), 1);
    EXPECT_EQ(_num_free_bytes(), base_size * 2);
    EXPECT_EQ(_num_allocated_blocks(), 3);
    EXPECT_EQ(_num_allocated_bytes(), base_size * 6);
    EXPECT_NO_THROW(sfree(p1));
    EXPECT_NO_THROW(sfree(p2));
    EXPECT_NO_THROW(sfree(p3));
    EXPECT_NO_THROW(sfree(p4));
}

TEST(test_part2, test_search_in_asc_order_wasteful) {
    test_search_in_asc_order_wasteful();
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
//    GTEST_FLAG_SET(death_test_style, "fast");
    printf("Running main() from %s\n", __FILE__);
    int status = RUN_ALL_TESTS();
    printf("Exiting main()\n");
    if (status == 0) {
        printf("⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠿⠿⠟⣛⣛⣛⣛⠻⠿⢿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿\n⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡿⠟⣉⣴⣶⣿⣿⣿⣿⣿⣿⣿⣿⣶⣦⣝⠻⣿⣿⣿⣿⣿⣿⣿\n⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡿⢋⣴⣾⣿⣿⣿⣿⡿⠿⢯⡻⣿⣿⣿⣿⣿⣿⣷⣌⠻⣿⣿⣿⣿⣿\n⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡿⢡⣾⣿⣿⣿⣿⣿⢣⣾⣿⠀⢳⣿⣿⣿⣿⣿⣿⣿⣿⣷⡹⣿⣿⣿⣿\n⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⢁⣿⣿⣿⣿⣿⣿⠇⠈⠛⠁⠀⢸⣿⣿⣿⣿⣿⣿⣿⣿⣵⠃⣿⣿⣿⣿\n⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡟⡛⢿⣿⣿⣿⡏⢸⣿⣿⣿⣿⣿⣿⠀⠀⠀⠀⠀⣸⣿⣿⣿⣿⣿⣿⣿⣿⢣⠃⣿⣿⣿⣿\n⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⢱⣿⣶⣍⠻⠿⠃⢸⣿⣿⣿⣿⣿⣿⣄⣀⠀⠠⢃⣿⣿⣿⣿⣿⣿⣿⣿⣿⠀⠀⣿⣿⣿⣿\n⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡇⣶⣶⣾⣿⢰⣮⣳⣬⡓⠘⣿⣿⣿⣿⣧⡻⢿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡀⣀⢿⣿⣿⣿\n⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣧⠸⣿⣿⣿⣸⣿⣿⣿⣿⣦⠹⣿⣿⣿⣿⣿⣷⣶⣬⣭⣍⣛⡛⠿⣿⣽⣿⣿⣿⣿⣿⣿⢸⣿⣿⣿\n⣿⣿⣿⣿⡿⠟⣛⣩⣭⣭⣭⣙⡛⢿⣿⣿⣿⣿⣿⡇⢻⣿⣿⣿⣿⣿⣿⣿⣿⣷⡙⢿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣦⣍⣛⣛⣭⣴⡶⣠⣿⣿⣿⣿\n⣿⣿⡿⢋⣴⣾⣿⣿⣿⣿⣿⣿⣿⣦⡙⣿⣿⣿⡟⢡⣆⢻⣿⣿⣿⣿⣿⣿⣿⣿⣿⣦⠙⠿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡿⢟⣴⣿⣿⣿⣿⣿\n⣿⠟⣠⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣷⡸⣿⡟⢠⣿⣿⣆⢻⣿⣿⣿⣿⣿⣿⣿⣿⣿⢰⣷⣮⣝⡛⠿⠿⣿⣿⣿⣿⡿⠿⢛⣩⣐⠿⣿⣿⣿⣿⣿⣿\n⡿⢠⣿⣿⣿⣿⣟⣋⣛⠿⣿⣿⣿⣿⣿⡇⠟⢠⣿⣿⣿⣿⡆⠻⣿⣿⣿⣿⣿⣿⡿⠃⢾⣿⣿⣿⣿⣿⣷⣶⣶⣶⣶⡄⣿⣿⣿⣿⣿⣶⣝⠻⠿⢿⣿\n⡇⢸⣿⣿⣿⣿⣿⣿⣿⣧⢹⣿⣿⣿⣿⡷⠀⢸⣿⣿⣿⣿⡇⣶⡝⠿⢿⡿⠿⠋⣠⣿⣦⡙⢿⣿⣿⣿⣿⣿⣿⣿⣿⣇⢹⣿⣿⣿⣿⣿⣿⣿⣿⡸⢿\n⣧⠸⣿⣿⣿⣿⣿⣿⣿⣿⢸⣿⣿⣿⣿⡇⢸⠈⣿⣿⣿⡿⢡⣿⡏⣰⣶⣶⣾⡇⢿⣿⣿⣿⣷⣬⣉⣛⠛⠻⠿⠟⢛⣋⢸⣿⣿⣿⣿⣿⣿⣿⣿⢫⣾\n⣿⣆⠹⣿⣿⣿⣿⣿⡿⢃⣾⣿⣿⣿⣿⡇⣾⣇⢹⣿⠟⣡⣿⡿⢡⣿⣿⣿⣿⣧⢸⣿⣿⣿⣿⣿⣿⣿⣿⡇⣼⣿⣿⡏⠘⠿⠿⢿⣿⡿⠿⠟⣋⣼⣿\n⣿⣿⣆⢬⣙⣛⣛⣩⣴⣿⣿⣿⣿⣿⣿⡇⣿⣿⣆⢋⣾⣿⡟⣡⣿⣿⣿⣿⣿⣿⢸⣿⣿⣿⣿⣿⣿⣿⣿⢡⣿⣿⣿⠇⣿⣿⣶⣶⣶⣶⣿⣿⣿⣿⣿\n⣿⣿⣿⣧⡹⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡇⣿⣿⡇⡾⢟⣋⡘⢿⣿⣿⣿⣿⠿⣫⣌⠻⣿⣿⣿⣿⣿⣿⢃⣾⣿⣿⡟⣸⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿\n⣿⣿⣿⣿⣿⣮⡻⣿⣿⣿⣿⣿⣿⣿⣿⣧⠸⠟⢀⣴⣿⣿⣿⣦⡙⠿⢟⣡⣾⣿⣿⣷⣦⣉⠛⠿⢿⠏⣼⣿⣿⠟⢰⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿\n⣿⣿⣿⣿⣿⣿⣿⣦⡙⠿⣿⣿⣿⣿⣿⣿⠇⣰⣿⣿⣿⣿⣿⣿⣷⡄⣿⣿⣿⣿⣿⣿⣿⣿⣿⣷⠆⣴⣶⣶⠆⣰⣦⠻⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿\n⣿⣿⣿⣿⣿⣿⣿⣿⣿⣷⣮⣍⡛⠻⠿⠟⢰⣿⣿⣿⣿⣿⣿⣿⣿⣷⠸⣿⣿⣿⣿⣿⣿⣿⣿⢏⣼⡿⠛⣡⣾⣿⣿⣧⢹⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿\n⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣷⢂⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠄⣉⣛⣛⣛⣛⣛⡛⠡⢚⣩⣴⣿⣿⣿⣿⣿⣿⡆⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿\n⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡿⢸⣿⣿⣿⣿⣿⣿⣿⣿⣿⡟⣠⣭⣿⣿⣭⣭⣅⡐⢾⣿⣿⣿⣿⣿⣿⣿⣿⣿⠁⢿⣿⣿⣿⣿⣿⣿⣿⣿⣿\n⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠟⣱⣿⣿⣿⣿⣿⣿⣿⣿⣿⢋⣼⣿⣿⣿⣿⣿⣿⣿⣿⣦⡙⢿⣿⣿⣿⣿⣿⣿⣿⡻⠷⣹⣿⣿⣿⣿⣿⣿⣿⣿\n⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣷⣌⡻⢿⣿⣿⣿⣿⣿⣿⡿⣡⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣦⠻⣿⣿⣿⣿⡿⠟⣋⣴⣿⣿⣿⣿⣿⣿⣿⣿⣿\n⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡸⠿⢛⣩⡻⣿⠏⣴⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣷⣌⣭⣭⣴⣶⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿\n⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣷⣿⣿⣿⣷⣦⣾⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿\n");
    }
    return status;
}