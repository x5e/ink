//
// Created by Darin McGill on 4/6/20.
//
#include "parser.hpp"

int ink::parse_array_prefix(ccp &ptr) {
    int count;
    if (((*ptr) & 0xF0) == 0x90) {
        count = (*ptr) & 0x0F;
        ptr += 1;
        return count;
    }
    if ((*ptr) == '\xdc' or *ptr == '\xdd') {
        throw std::runtime_error("not implemented");
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

ink::trxn_row::trxn_row(ink::ccp &ptr) {
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
