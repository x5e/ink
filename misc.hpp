#pragma once
#include <string>
#include <vector>
#include <cstdlib>
#include "verify.hpp"

namespace ink {
    using path = std::string;
    std::vector<std::string> split(const std::string &thing, const std::string &how);
    void ensure_containing_directory(const std::string &fn);
    void ensure_directory(const std::string &fn);
    std::string escapes(const std::string& from);

    inline uint64_t flip64(uint64_t value) noexcept
    {
        char* ptr = reinterpret_cast<char*>(&value);
        std::reverse(ptr, ptr + sizeof(uint64_t));
        return value;
    }

    inline uint32_t flip32(uint32_t value) noexcept
    {
        char* ptr = reinterpret_cast<char*>(&value);
        std::reverse(ptr, ptr + sizeof(uint32_t));
        return value;
    }

    inline void touch(const path& fn) {
        std::string command = std::string("touch ") + fn;
        auto result = system(command.c_str());
        VERIFY(result == 0);
    }

}