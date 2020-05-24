#pragma once
#include <string>
#include <vector>
#include <cstdlib>
#include "verify.hpp"
#include "Id.hpp"


namespace ink {
    using path_t = std::string;
    std::vector<std::string> split(const std::string &thing, const std::string &how);
    void ensure_directory(const std::string &fn);
    std::string escapes(const std::string& from);


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


    inline bool exists(const path_t & fn) {
        using namespace std::string_literals;
        std::string command = "/bin/test -e "s + fn;
        auto result = system(command.c_str());
        return (result == 0);
    }

    inline bool touch(const path_t& fn) {
        std::string command = std::string("touch ") + fn;
        auto result = system(command.c_str());
        return(result == 0);
    }

}