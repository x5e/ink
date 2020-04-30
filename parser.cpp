#include "parser.hpp"
#include "misc.hpp"

uint32_t ink::parse_array_prefix(cstr_t &ptr) {
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
    throw std::runtime_error("unexpected");
}

ink::Muid ink::parse_muid(ink::cstr_t &ptr) {
    Muid an_id;
    char tag = *ptr++;
    if (tag == '\xc0') {
        an_id.zero();
        return an_id;
    }
    if (tag != '\xd8')
        throw parse_error(__FILE__, __LINE__);
    char kind = *ptr++;
    if (kind != '\x01')
        throw parse_error(__FILE__, __LINE__);
    an_id.copy_from(ptr);
    ptr += 16;
    return an_id;
}

int64_t ink::parse_bigint(ink::cstr_t &ptr) {
    int64_t out;
    switch (*(ptr++)) {
        case '\xcf':
        case '\xd3':
            out = *reinterpret_cast<const int64_t*>(ptr);
            ptr += 8;
            return flip(out);
        case '\xce':
        case '\xd2':
            auto four = *reinterpret_cast<const int32_t*>(ptr);
            ptr += 4;
            return flip(four);
    }
    throw parse_error(__FILE__, __LINE__);
}

std::string ink::parse_string(ink::cstr_t &ptr) {
    std::string out;
    size_t bytes = -1;
    auto tag = *reinterpret_cast<const uint8_t*>(ptr++);
    if ((tag & 0b1110'0000) == 0b1010'0000) {
        bytes = tag & 0b0001'1111;
    }
    if (tag == 0xd9 || tag == 0xc4) {
        bytes = *reinterpret_cast<const uint8_t*>(ptr++);
    }
    if (bytes == -1) throw std::runtime_error("not implemented");
    auto start_at = ptr;
    ptr += bytes;
    return std::string(start_at, bytes);
}

ink::Fancy<ink::Row> ink::parse_row(ink::cstr_t &ptr) {
    int count = parse_array_prefix(ptr);
    VERIFY(count >= 2);
    auto row_tag = (tag_t) *ptr++;
    switch (row_tag) {
        case TrxnRow::Tag:
            return make_fancy<TrxnRow>(ptr, count -1);
        case PurgeRow::Tag:
            return make_fancy<PurgeRow>(ptr, count -1);
        default:
            throw parse_error(__FILE__, __LINE__);
    }
}

