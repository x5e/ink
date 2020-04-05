#include <iostream>
#include "CapFile.hpp"
#include "contents.hpp"


ink::CapFile::CapFile(const ink::muid &story) {
    story_ = story;
    path_ = "pcaps/" + story_.get_jell_string() + "/" + std::string(story_);
    ensure_containing_directory(path_);
    handle_.open(path_.c_str(), std::ios_base::binary);
    handle_.seekp(0, std::ios::end);
    if (handle_.tellp() == 0) {
        pcap_hdr_t pcap_hdr;
        handle_.write((char *) &pcap_hdr, sizeof(pcap_hdr_t));
    }
    VERIFY(handle_.tellg() == 0);
    handle_.seekg(sizeof(pcap_hdr_t));
    while (handle_.tellg() < handle_.tellp()) {
        pcaprec_hdr_t record_header;
        auto record_start = handle_.tellg();
        handle_.read((char *) &record_header, sizeof(record_header));
        VERIFY(record_header.incl_len == record_header.orig_len);
        uint64_t muts = (1L * MILLION * record_header.ts_sec) + record_header.ts_usec;
        VERIFY(muts > 1577854800000000L and muts < 2145934800000000L);
        index_[muts] = record_start;
        handle_.seekg(record_header.incl_len, std::ios::cur);
    }
    VERIFY(handle_.tellg() == handle_.tellp());
}


void ink::CapFile::append(const std::string& msg) {
    const char* ptr = msg.data();
    if (*ptr++ != '\x92')
        throw parse_error(__FILE__, __LINE__);
    if (*ptr++ != '\x01')
        throw parse_error(__FILE__, __LINE__);
    int rows = parse_array_prefix(ptr);
    if (rows < 1)
        throw parse_error(__FILE__, __LINE__);
    trxn one_row(ptr);
    std::cerr << "received: " << std::string(one_row.id_) << " " << one_row.id_.get_jell_string() << std::endl;
    VERIFY(one_row.story == story_);
    if (not index_.empty()) {
        VERIFY(one_row.id_.get_muts() > index_.end()->first);
    }
    pcaprec_hdr_t record_header{};
    auto muts = one_row.id_.get_muts();
    record_header.ts_sec = muts / MILLION;
    record_header.ts_usec = muts % MILLION;
    record_header.orig_len = msg.size();
    record_header.incl_len = msg.size();
    handle_.write((char*) &record_header, sizeof(record_header));
    handle_ << msg;
    handle_.flush();  // TODO figure out a way to close the file on exit.
}
