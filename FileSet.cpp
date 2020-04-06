//
// Created by Darin McGill on 4/6/20.
//

#include "FileSet.hpp"


void ink::FileSet::receive(const std::string& msg) {
    const char* ptr = msg.data();
    if (*ptr++ != '\x92')
        throw parse_error(__FILE__, __LINE__);
    if (*ptr++ != '\x01')
        throw parse_error(__FILE__, __LINE__);
    int rows = parse_array_prefix(ptr);
    if (rows < 1)
        throw parse_error(__FILE__, __LINE__);
    trxn_row one_row(ptr);
    std::cerr << "received: " << std::string(one_row.id_) << " " << one_row.id_.get_jell_string() << std::endl;
    auto& ref = cap_files[one_row.story];
    if (not ref) {
        ref = std::make_shared<CapFile>(one_row.story, directory_);
    }
    ref->receive(msg, one_row);
}

std::string ink::FileSet::greeting() const {
    auto count = cap_files.size();
    VERIFY(count < 65536);
    std::stringstream stream;
    stream << '\x92' << '\x08';
    stream << '\xDE' << char(count >> 8) << char(count & 0xFF);
    for (const auto& thing: cap_files) {
        stream << '\xD8' << '\x01';
        stream.write(thing.first.bytes(), 16);
        auto gone_to = thing.second->goes_to();
        for (int i=7;i>=0;i--) {
            stream << char((gone_to >> (i*8)) & 0xFF);
        }
    }
    return stream.str();
}