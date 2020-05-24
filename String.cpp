#include "String.hpp"
#include <stdexcept>

void ink::String::decode(ink::cstr_t &ptr) {
    size_t bytes = -1;
    auto tag = *reinterpret_cast<const uint8_t*>(ptr++);
    if ((tag & 0b1110'0000) == 0b1010'0000) {
        bytes = tag & 0b0001'1111;
    }
    if (tag == 0xd9 || tag == 0xc4) {
        bytes = *reinterpret_cast<const uint8_t*>(ptr++);
    }
    if (bytes == -1)
        throw std::runtime_error("not implemented");
    auto start_at = ptr;
    ptr += bytes;
    string_ = std::string(start_at, bytes);
}
