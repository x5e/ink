#include <iostream>
#include "CapFile.hpp"
#include "contents.hpp"
#define MILLION (1000*1000)


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
    std::cerr << "received: " << std::string(one.id_) << " " << one.id_.get_jell_string() <<  std::endl;
    pcaprec_hdr_t record_header{};
    auto muts = one.id_.get_muts();
    record_header.ts_sec = muts / MILLION;
    record_header.ts_usec = muts % MILLION;
    record_header.orig_len = msg.size();
    record_header.incl_len = msg.size();
    out_.write((char*) &record_header, sizeof(record_header));
    out_ << msg;
    out_.flush();
}
