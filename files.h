#ifndef HW07_BAYAN_FILES_H
#define HW07_BAYAN_FILES_H

#include <fstream>
#include <memory>
#include <string>
#include <utility>
#include <vector>

struct options {
    options &with_min_size(std::size_t size) {
        min_size = size;
        return *this;
    }

    options &with_exclude(const std::string &to_exclude) {
        exclude.push_back(to_exclude);
        return *this;
    }

    options &with_exclude(std::vector<std::string> to_exclude) {
        exclude = std::move(to_exclude);
        return *this;
    }

    options &with_mask(const std::string &mask) {
        masks.push_back(mask);
        return *this;
    }

    options &with_mask(std::vector<std::string> mask) {
        masks = std::move(mask);
        return *this;
    }

    std::vector<std::string> exclude;
    std::vector<std::string> masks;
    std::size_t min_size = 1;
};

template<typename Hash>
struct file {
    std::string path;
    std::size_t id = 0;
    std::vector<typename Hash::value_type> blocks{};
    bool at_eof = false;
};

std::vector<std::string> make_file_list(std::string path, options options, std::size_t depth = 0);

template<typename Hash>
bool read_next_block(file<Hash> &f, std::size_t at_most_bytes, const Hash &hash) {
    if (f.at_eof) return false;

    auto buf = std::unique_ptr<char>(new char[at_most_bytes]);
    std::uninitialized_fill(buf.get(), buf.get() + at_most_bytes, 0);
    std::ifstream fs(f.path, std::ifstream::binary);
    fs.seekg(f.blocks.size() * at_most_bytes);
    f.at_eof = fs.read(buf.get(), at_most_bytes).eof();

    auto block_hash = hash(buf.get(), at_most_bytes);
    f.blocks.push_back(block_hash);
    return true;
}

#endif
