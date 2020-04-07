//
// Created by Darin McGill on 4/6/20.
//
#include "parser.hpp"
#include "misc.hpp"

uint32_t ink::parse_array_prefix(ccp &ptr) {
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

void ink::parse_muid(ink::ccp &ptr, ink::muid &an_id) {
    char tag = *ptr++;
    if (tag == '\xc0') {
        an_id.zero();
        return;
    }
    if (tag != '\xd8')
        throw parse_error(__FILE__, __LINE__);
    char kind = *ptr++;
    if (kind != '\x01')
        throw parse_error(__FILE__, __LINE__);
    an_id.copy_from(ptr);
    ptr += 16;
}

ink::TrxnRow::TrxnRow(ink::ccp &ptr) {
    int count = parse_array_prefix(ptr);
    if (count < 3)
        throw parse_error(__FILE__, __LINE__);
    char row_tag = *ptr++;
    if (row_tag != '\x13')
        throw parse_error(__FILE__, __LINE__);
    parse_muid(ptr, id_);
    parse_muid(ptr, story);
    if (count >= 4) {
        parse_muid(ptr, acct);
    }
}
