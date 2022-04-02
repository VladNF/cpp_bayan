#include <gtest/gtest.h>
#include "hash.h"
#include "store.h"

TEST(HashStore, InsertQuery) {
    HashStore<MD5Hash> store;
    MD5Hash hash = MD5Hash();
    auto hash1 = hash("test1", 5);
    auto hash2 = hash("test2", 5);
    auto hash3 = hash("test3", 5);
    auto hash4 = hash("test4", 5);
    store.insert({"1", 1, {hash1, hash3}});
    store.insert({"2", 2, {hash2, hash3}});
    store.insert({"3", 3, {hash1, hash3}});
    store.insert({"4", 4, {hash2, hash4}});

    ASSERT_EQ(store.get_count_with_hash({hash1, hash3}), 2);
    ASSERT_EQ(store.get_count_with_hash({hash2, hash3}), 1);
    ASSERT_EQ(store.get_count_with_hash({hash1, hash4}), 0);
    ASSERT_EQ(store.get_count_with_hash({hash2, hash4}), 1);
}

TEST(HashStore, GetByHash) {
    HashStore<MD5Hash> store;
    MD5Hash hash = MD5Hash();
    auto hash1 = hash("test1", 5);
    auto hash2 = hash("test2", 5);
    auto hash3 = hash("test3", 5);
    auto hash4 = hash("test4", 5);
    store.insert({"1", 1, {hash1, hash3}});
    store.insert({"2", 2, {hash2, hash3}});
    store.insert({"3", 3, {hash1, hash3}});
    store.insert({"4", 4, {hash2, hash4}});

    auto files = store.get_with_hash({hash1, hash3});
    ASSERT_EQ(files.size(), 2);

    files = store.get_with_hash({hash2, hash4});
    ASSERT_EQ(files.size(), 1);

    files = store.get_with_hash({hash2});
    ASSERT_TRUE(files.empty());
}

TEST(HashStore, Delete) {
    HashStore<MD5Hash> store;
    MD5Hash hash = MD5Hash();
    auto hash1 = hash("test1", 5);
    auto hash2 = hash("test2", 5);
    auto hash3 = hash("test3", 5);
    auto hash4 = hash("test4", 5);
    store.insert({"1", 1, {hash1, hash3}});
    store.insert({"2", 2, {hash2, hash3}});
    store.insert({"3", 3, {hash1, hash3}});
    store.insert({"4", 4, {hash2, hash4}});

    store.delete_with_hash({hash1, hash3});
    ASSERT_EQ(store.size(), 2);

    store.delete_with_hash({hash2, hash3});
    ASSERT_EQ(store.size(), 1);

    store.delete_with_hash({hash2, hash4});
    ASSERT_EQ(store.size(), 0);
}

