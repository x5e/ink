#include <iostream>
#include "CapFile.hpp"
#include "contents.hpp"



void ink::CapFile::append(const std::string& msg) {
    const char* ptr = msg.data();
    if (*ptr++ != '\x92')
        throw parse_error(__FILE__, __LINE__);
    if (*ptr++ != '\x01')
        throw parse_error(__FILE__, __LINE__);
    int rows = parse_array_prefix(ptr);
    if (rows < 1)
        throw parse_error(__FILE__, __LINE__);
    trxn one(ptr);
    std::cerr << "received: " << std::string(one.id_) << " " << std::string(one.story) << std::endl;
    out_ << msg;
    out_.flush();
}
