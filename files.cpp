#include <algorithm>
#include <regex>
#include <boost/filesystem/operations.hpp>
#include "files.h"

namespace fs = boost::filesystem;

std::vector<std::string> make_file_list(std::string path, options options, std::size_t depth) {
    std::vector<std::string> result;
    if (std::find(options.exclude.begin(), options.exclude.end(), path) != options.exclude.end()) {
        return result;
    }

    std::vector<std::regex> masks;
    for (auto &m : options.masks) masks.emplace_back(m, std::regex_constants::icase);
    fs::directory_iterator it(path), end;
    auto file_match = [&](fs::directory_iterator &i) {
        return fs::file_size(i->path()) >= options.min_size &&
               std::all_of(
                       masks.begin(),
                       masks.end(),
                       [&](std::regex &m) {
                           return std::regex_match(i->path().filename().string(), m);
                       }
               );
    };

    for (; it != end; ++it) {
        fs::file_status fs = fs::status(*it);
        if (fs::file_type::directory_file == fs.type() && depth) {
            auto from_folder = make_file_list(
                    it->path().generic_string(),
                    options,
                    --depth
            );
            std::copy(
                    from_folder.begin(), from_folder.end(), std::back_inserter(result)
            );
        } else if (fs::file_type::regular_file == fs.type() && file_match(it)) {
            result.push_back({it->path().generic_string()});
        }
    }

    return result;
}