#include <fstream>
#include <gtest/gtest.h>
#include <boost/filesystem/operations.hpp>
#include "files.h"
#include "hash.h"

namespace fs = boost::filesystem;

class FilesTest : public ::testing::Test {
protected:
    void SetUp() override {
        fs::create_directories("bayan_test/1/2/3");
        std::ofstream ofs_0("bayan_test/fs.cpp");
        std::ofstream ofs_1("bayan_test/1/fs_42.c");
        std::ofstream ofs_2("bayan_test/1/2/fs_43.py");
        std::ofstream ofs_3("bayan_test/1/2/3/fs.so");
        ofs_0 << "1";
        ofs_1 << "42";
        ofs_2 << "-43-";
        ofs_3 << 0x42 << 0x43;
    }

    void TearDown() override {
        fs::remove_all("bayan_test");
    }
};

TEST_F(FilesTest, ScanOptionsDepth) {
    for (int i = 0; i < 4; ++i) {
        auto list = make_file_list("bayan_test", options(), i);
        ASSERT_EQ(list.size(), i + 1);
    }
}

TEST_F(FilesTest, ScanOptionsSize) {
    auto list = make_file_list(
            "bayan_test", options().with_min_size(4), 4
    );
    ASSERT_EQ(list.size(), 2);

    list = make_file_list(
            "bayan_test", options().with_min_size(4), 1
    );
    ASSERT_EQ(list.size(), 0);

    list = make_file_list(
            "bayan_test", options().with_min_size(2), 4
    );
    ASSERT_EQ(list.size(), 3);
}

TEST_F(FilesTest, ScanOptionsIgnore) {
    auto list = make_file_list(
            "bayan_test",
            options().with_exclude("bayan_test/1"),
            4
    );
    ASSERT_EQ(list.size(), 1);

    list = make_file_list(
            "bayan_test",
            options().with_exclude("bayan_test/1/2/3"),
            4
    );
    ASSERT_EQ(list.size(), 3);
}

TEST_F(FilesTest, ScanOptionsMasks) {
    auto list = make_file_list(
            "bayan_test",
            options().with_mask(".*\\.cpp"),
            4
    );
    ASSERT_EQ(list.size(), 1);

    list = make_file_list(
            "bayan_test",
            options().with_mask(".*\\.CPP"),
            4
    );
    ASSERT_EQ(list.size(), 1);

    list = make_file_list(
            "bayan_test",
            options().with_mask("^fs.*"),
            4
    );
    ASSERT_EQ(list.size(), 4);
}

TEST_F(FilesTest, ReadOneBlock) {
    auto f = file<MD5Hash>{"bayan_test/fs.cpp", 42};
    auto hash = MD5Hash();
    ASSERT_TRUE(read_next_block(f, 1, hash));
    ASSERT_EQ(f.blocks.size(), 1);
    ASSERT_EQ(f.blocks.back(), hash("1", 1));

    f = file<MD5Hash>{"bayan_test/1/fs_42.c"};
    ASSERT_TRUE(read_next_block(f, 3, hash));
    ASSERT_EQ(f.blocks.size(), 1);
    ASSERT_EQ(f.blocks.back(), hash("42\0", 3));
}

TEST_F(FilesTest, ReadUntilEOF) {
    auto f = file<MD5Hash>{"bayan_test/1/2/fs_43.py", 43};
    auto hash = MD5Hash();

    ASSERT_TRUE(read_next_block(f, 3, hash));
    ASSERT_EQ(f.blocks.size(), 1);
    ASSERT_FALSE(f.at_eof);
    ASSERT_EQ(f.blocks.back(), hash("-43", 3));

    ASSERT_TRUE(read_next_block(f, 3, hash));
    ASSERT_EQ(f.blocks.size(), 2);
    ASSERT_TRUE(f.at_eof);
    ASSERT_EQ(f.blocks.back(), hash("-\0\0", 3));

    ASSERT_FALSE(read_next_block(f, 3, hash));
}