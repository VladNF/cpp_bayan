# Bayan
Bayan is yet another file deduplication util. There's nothing innovative in it
as it uses per block file comparison using one of two hash functions `md5` or `crc32`.

Usage:
```
All options:
  -h [ --help ]                  This screen
  -t [ --target ] arg            target directories
  -x [ --exclude ] arg           directories to exclude
  -m [ --mask ] arg              file masks
  -d [ --depth ] arg (=0)        scan depth
  -s [ --file_size ] arg (=1)    min file size
  -b [ --block_size ] arg (=512) min file size
  -a [ --hash_algo ] arg (=md5)  hash function name, supported only: md5, crc32
```

# Build notes
It is expected `googletest` is located in the parent directory, please lookt at CMakeLists.txt