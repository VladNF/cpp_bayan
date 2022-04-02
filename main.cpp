#include <algorithm>
#include <iostream>
#include <list>
#include <string>
#include <utility>
#include <vector>
#include <boost/program_options.hpp>
#include "files.h"
#include "hash.h"
#include "store.h"

namespace opt = boost::program_options;

template <typename Hash>
void find_duplicate_files(
        std::vector<std::string> target,
        std::vector<std::string> exclude,
        std::vector<std::string> masks,
        std::size_t depth,
        std::size_t block_size,
        std::size_t file_size
);

int main(int argc, char const *argv[]) {
    std::vector<std::string> target, exclude, masks;
    std::size_t depth, block_size, file_size;
    std::string hash_algo;
    opt::options_description desc("All options");
    desc.add_options()
            ("help,h", "This screen")
            ("target,t", opt::value<std::vector<std::string>>(&target), "target directories")
            ("exclude,x", opt::value<std::vector<std::string>>(&exclude), "directories to exclude")
            ("mask,m", opt::value<std::vector<std::string>>(&masks), "file masks")
            ("depth,d", opt::value<std::size_t>(&depth)->default_value(0), "scan depth")
            ("file_size,s", opt::value<std::size_t>(&file_size)->default_value(1), "min file size")
            ("block_size,b", opt::value<std::size_t>(&block_size)->default_value(512), "min file size")
            ("hash_algo,a", opt::value<std::string>(&hash_algo)->default_value("md5"),
             "hash function name, supported only: md5, crc32");

    opt::variables_map vm;
    opt::store(opt::parse_command_line(argc, argv, desc), vm);
    opt::notify(vm);

    if (vm.count("help")) {
        std::cout << desc << "\n";
        return 1;
    }

    if (target.empty()) target.emplace_back(".");
    if (hash_algo == "md5") {
        find_duplicate_files<MD5Hash>(target, exclude, masks, depth, block_size, file_size);
    }
    else if (hash_algo == "crc32") {
        find_duplicate_files<CRC32Hash>(target, exclude, masks, depth, block_size, file_size);
    } else {
        std::cerr << "Unsupported hash algo: " << hash_algo << "\n";
    }

}

template <typename Hash>
void find_duplicate_files(
        std::vector<std::string> target,
        std::vector<std::string> exclude,
        std::vector<std::string> masks,
        std::size_t depth,
        std::size_t block_size,
        std::size_t file_size
) {
    std::list<file<Hash>> files;
    std::size_t files_count = 0;

    options opt = options()
            .with_exclude(std::move(exclude))
            .with_mask(std::move(masks))
            .with_min_size(file_size);

    for (auto &path : target) {
        for (auto &p : make_file_list(path, opt, depth)) {
            files.push_back({p, files_count++});
        }
    }

    bool read_new_blocks;
    HashStore<Hash> hash_store;

    do {
        // read blocks
        read_new_blocks = false;
        for (auto &file : files) {
            if (!file.at_eof)
                read_new_blocks |= read_next_block(file, block_size, Hash());
        }

        // remove files with unique blocks
        hash_store.clear();
        for (auto &file : files) hash_store.insert(file);
        files.remove_if([&](file <Hash> &f) { return hash_store.get_count_with_hash(f.blocks) < 2; });

    } while (read_new_blocks);

    // print out duplicates
    for (auto it = files.begin(); it != files.end();) {
        if (hash_store.get_count_with_hash(it->blocks)) {
            std::cout << std::endl;
            for (auto &f : hash_store.get_with_hash(it->blocks)) {
                std::cout << f.path << "\n";
            }
            std::cout << std::endl;
            hash_store.delete_with_hash(it->blocks);
        }
        ++it;
    }   
}