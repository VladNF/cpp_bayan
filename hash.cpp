#include <boost/algorithm/hex.hpp>
#include "hash.h"

using boost::uuids::detail::md5;

CRC32Hash::value_type CRC32Hash::operator()(const char *buf, std::size_t size) const {
    boost::crc_32_type hash;
    hash.process_bytes(buf, size);
    return hash.checksum();
}

MD5Hash::value_type MD5Hash::operator()(const char *buf, std::size_t size) const {
    md5 hash;
    md5::digest_type digest;
    hash.process_bytes(buf, size);
    hash.get_digest(digest);
    value_type result;
    std::copy(std::begin(digest), std::end(digest), result.begin());
    return result;
}

std::string MD5Hash::to_string(MD5Hash::value_type digest) {
    std::string result;
    boost::algorithm::hex(digest.begin(), digest.end(), std::back_inserter(result));
    return result;
}
