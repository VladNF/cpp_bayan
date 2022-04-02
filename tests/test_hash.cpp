#include <gtest/gtest.h>
#include "hash.h"

TEST(HashTest, MD5) {
    std::string buf("test_string");
    MD5Hash hash;
    MD5Hash::value_type digest = hash(buf.c_str(), buf.size());
    ASSERT_EQ(MD5Hash::to_string(digest), "3474851A3410906697EC77337DF7AAE4");
}

TEST(HashTest, CRC32) {
    std::string buf("test_string");
    CRC32Hash hash;
    CRC32Hash::value_type digest = hash(buf.c_str(), buf.size());
    ASSERT_EQ(digest, 157791623);
}