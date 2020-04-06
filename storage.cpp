#include <iostream>
#include "storage.hpp"
#include "contents.hpp"


ink::CapFile::CapFile(const ink::muid &story, const path& location) {
    story_ = story;
    // TODO file lock, use location, etc.
    path_ = "./pcaps/" + story_.get_jell_string() + "/" + std::string(story_);
    ensure_containing_directory(path_);
    std::cerr << "path=" << path_ << std::endl;
    output_.open(path_.c_str(), std::ios_base::binary | std::ios_base::app | std::ios_base::ate);
    VERIFY(output_.is_open());
    VERIFY ((output_.rdstate() & std::ifstream::failbit ) == 0 );
    if (output_.tellp() == 0) {
        pcap_hdr_t pcap_hdr;
        output_.write((char *) &pcap_hdr, sizeof(pcap_hdr_t));
    }
    innput_.open(path_.c_str(), std::ios_base::binary);
    VERIFY(innput_.is_open());
    VERIFY(innput_.tellg() == 0);
    innput_.seekg(sizeof(pcap_hdr_t));
    uint64_t last_muts = 0;
    while (innput_.tellg() < output_.tellp()) {
        pcaprec_hdr_t record_header;
        auto record_start = innput_.tellg();
        innput_.read((char *) &record_header, sizeof(record_header));
        VERIFY(record_header.incl_len == record_header.orig_len);
        uint64_t muts = (1L * MILLION * record_header.ts_sec) + record_header.ts_usec;
        VERIFY(muts > 1577854800000000L and muts < 2145934800000000L);
        VERIFY(muts > last_muts);
        last_muts = muts;
        index_[muts] = record_start;
        innput_.seekg(record_header.incl_len, std::ios::cur);
    }
    VERIFY(innput_.tellg() == output_.tellp());
}

void ink::CapFile::receive(const std::string &msg, const trxn_row& one_row) {
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
    output_.write((char*) &record_header, sizeof(record_header));
    output_ << msg;
    output_.flush();  // TODO figure out a way to close the file on exit.
}

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
    if (not ref)
        ref = std::make_shared<CapFile>(one_row.story, directory_);
    ref->receive(msg, one_row);
}
