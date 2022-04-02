#ifndef HW07_BAYAN_HASH_H
#define HW07_BAYAN_HASH_H

#include <boost/array.hpp>
#include <boost/crc.hpp>
#include <boost/uuid/detail/md5.hpp>

struct MD5Hash {
    static constexpr short hash_size = 4;
    using value_type = boost::array<unsigned int, hash_size>;
    static constexpr value_type default_value{};

    value_type operator()(const char *buf, std::size_t size) const;

    static std::string to_string(value_type digest);
};

struct CRC32Hash {
    using value_type = boost::crc_32_type::value_type;
    static constexpr value_type default_value{};

    value_type operator()(const char *buf, std::size_t size) const;
};

#endif //HW07_BAYAN_HASH_H
