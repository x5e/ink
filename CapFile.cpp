#include <iostream>
#include <utility>
#include <fcntl.h>
#include <unistd.h>
#include "CapFile.hpp"
#include "decoder.hpp"
#include "misc.hpp"


ink::CapFile::CapFile(path_t file_path): path_(std::move(file_path)),
    fd(exists(path_) || touch(path_) ? ::open(path_.c_str(), O_RDWR) : 0)
{
    // TODO file lock, use location, etc.
    std::cerr << "opening CapFile with path=" << path_ << std::endl;
    VERIFY(fd > 0);
    off_t initial_size = ::lseek(fd, 0, SEEK_END);
    if (initial_size == 0) {
        pcap_hdr_t pcap_hdr;
        ::write(fd, (char *) &pcap_hdr, sizeof(pcap_hdr_t));
    } else {
        location = ::lseek(fd, sizeof(pcap_hdr_t), SEEK_SET);
        VERIFY(location == sizeof(pcap_hdr_t));
        uint64_t last_muts = 0;
        while (location < initial_size) {
            pcaprec_hdr_t record_header;
            auto record_start = location;
            auto red = ::read(fd, (char *) &record_header, sizeof(record_header));
            VERIFY(red == sizeof(record_header));
            VERIFY(record_header.incl_len == record_header.orig_len);
            uint64_t muts = (1L * MILLION * record_header.ts_sec) + record_header.ts_usec;
            VERIFY(muts > 1577854800000000L and muts < 2145934800000000L and muts > last_muts);
            last_muts = muts;
            index_[muts] = record_start;
            location = ::lseek(fd, record_header.incl_len, SEEK_CUR);
            if (record_header.incl_len > max_packet_size)
                max_packet_size = record_header.incl_len;
        }
        VERIFY(location == initial_size);
    }
}

void ink::CapFile::receive(const char* ptr, size_t size, ink::muts_t muts) {
    if (not index_.empty()) {
        VERIFY(muts >= (--index_.end())->first);
    }
    pcaprec_hdr_t record_header{};
    record_header.ts_sec = muts / MILLION;
    record_header.ts_usec = muts % MILLION;
    record_header.orig_len = size;
    record_header.incl_len = size;
    index_[muts] = location;
    ssize_t written;
    location += written = ::write(fd, &record_header, sizeof(record_header));
    VERIFY(written == sizeof(record_header));
    location += written = ::write(fd, ptr, size);
    VERIFY(written == size);
    if (size > max_packet_size)
        max_packet_size = size;
}