#include <cassert>
#include "decoder.hpp"
#include "misc.hpp"
#include "Window.hpp"
#define DECODE_REQUIRE(x) if (not (x)) throw ::ink::DecodeError(__FILE__, __LINE__)

namespace ink {
    class DecodeError : public std::runtime_error {
    public:
        DecodeError(const char *fn, int line) : std::runtime_error(std::string(fn) + std::to_string(line)) {}
    };
}

uint32_t ink::Window::decode_array_prefix() {
    auto& ptr = start_;
    uint32_t count = 0;
    if (((*ptr) & 0xF0) == 0x90) {
        count = (*ptr) & 0x0F;
        ptr += 1;
        return count;
    }
    if ((*ptr) == '\xdc') {
        ptr += 1;
        count += (uint8_t) *ptr;
        ptr += 1;
        count = count << 8;
        count += (uint8_t) *ptr;
        ptr += 1;
        return count;
    }
    if ((*ptr) == '\xdd') {
        ptr += 1;
        uint32_t flipped = *((uint32_t*) ptr);
        count = flip32(flipped);
        ptr += 4;
        return count;
    }
    throw DecodeError(__FILE__, __LINE__);
}


void ink::Window::decode_id(Id& id) {
    auto& ptr = start_;
    char tag = *ptr++;
    if (tag == '\xc0') {
        id.zero();
    } else {
        DECODE_REQUIRE(tag == '\xd8');
        char kind = *ptr++;
        DECODE_REQUIRE(kind == '\x01');
        id.copy_from(ptr);
        ptr += 16;
    }
}

int64_t ink::Window::decode_bigint() {
    auto& ptr = start_;
    int64_t out;
    switch (*(ptr++)) {
        case '\xcf':
        case '\xd3':
            out = *reinterpret_cast<const int64_t*>(ptr);
            ptr += 8;
            return flip64(out);
        case '\xce':
        case '\xd2':
            auto four = *reinterpret_cast<const int32_t*>(ptr);
            ptr += 4;
            return flip32(four);
    }
    throw DecodeError(__FILE__, __LINE__);
}

/*
void ink::parse_row(ink::cstr_t &ptr, buff_t dest, size_t size) {
    int count = decode_array_prefix(ptr);
    VERIFY(count >= 2);
    auto row_tag = (tag_t) *ptr++;
    switch (row_tag) {
        case TrxnRow::Tag:
            assert(size >= sizeof(TrxnRow));
            reinterpret_cast<TrxnRow *>(dest)->decode(ptr, count - 1);
            break;
        case PurgeRow::Tag:
            assert(size >= sizeof(PurgeRow));
            auto prgRow = reinterpret_cast<PurgeRow*>(dest);
            prgRow->parse(ptr, count -1);
            return;
        default:
            throw DecodeError(__FILE__, __LINE__);
    }
}
*/
