#pragma once
#include <string>
#include "typedefs.hpp"

namespace ink {
    class String {
        std::string string_;
    public:
        void decode(cstr_t& ptr);
        std::string to_string() const {return string_;}
    };
}
