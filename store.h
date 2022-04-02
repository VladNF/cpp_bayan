#ifndef HW07_BAYAN_STORE_H
#define HW07_BAYAN_STORE_H

#include <algorithm>
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/composite_key.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/member.hpp>
#include "files.h"

namespace mix = boost::multi_index;

template<typename Hash>
class HashStore {
    using file_t = file<Hash>;

    struct hash_key: mix::member<
            file_t, decltype(file_t::blocks), &file_t::blocks
    > {};
    using hash_store_t = mix::multi_index_container<
            file_t,
            mix::indexed_by<mix::hashed_non_unique<hash_key>>
    >;

public:
    std::size_t size() {return files_.size();}

    void clear() {
        auto &index = mix::get<0>(files_);
        index.clear();
    }

    void insert(file_t f) {
        files_.insert(f);
    }

    std::size_t get_count_with_hash(decltype(file_t::blocks) hash) {
        auto &index = mix::get<0>(files_);
        return index.count(hash);
    }

    std::vector<file_t> get_with_hash(decltype(file_t::blocks) hash) {
        auto &index = mix::get<0>(files_);
        std::vector<file_t> result;
        result.reserve(index.count(hash));
        auto [start, end] = index.equal_range(hash);
        std::copy(start, end, std::back_inserter(result));
        return result;
    }

    void delete_with_hash(decltype(file_t::blocks) hash) {
        auto &index = mix::get<0>(files_);
        auto [start, end] = index.equal_range(hash);
        index.erase(start, end);
    }

private:
    hash_store_t files_;
};

#endif //HW07_BAYAN_STORE_H
