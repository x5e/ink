#pragma once
#include <string>
#include <vector>

namespace ink {
    std::vector<std::string> split(const std::string &thing, const std::string &how);
    void ensure_directory(const std::string &fn);
    std::string escapes(const std::string& from);
    bool exists(const std::string & fn);
    bool touch(const std::string& fn);


    inline uint64_t flip64(uint64_t x) noexcept
    {
        x = (x & 0x00000000FFFFFFFF) << 32 | (x & 0xFFFFFFFF00000000) >> 32;
        x = (x & 0x0000FFFF0000FFFF) << 16 | (x & 0xFFFF0000FFFF0000) >> 16;
        x = (x & 0x00FF00FF00FF00FF) << 8  | (x & 0xFF00FF00FF00FF00) >> 8;
        return x;
    }

    inline uint32_t flip32(uint32_t num) noexcept
    {
        uint32_t swapped = ((num>>24)&0xff) | // move byte 3 to byte 0
                  ((num<<8)&0xff0000) | // move byte 1 to byte 2
                  ((num>>8)&0xff00) | // move byte 2 to byte 1
                  ((num<<24)&0xff000000); // byte 0 to byte 3
        return swapped;
    }

}