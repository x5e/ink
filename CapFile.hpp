#pragma once

#include "hdr_t.hpp"
#include <fstream>

namespace ink {

    class CapFile {
        std::ofstream out_;
    public:
        explicit CapFile(const std::string &fn = "received.pcap") :
                out_(std::ofstream(fn.c_str(), std::ios_base::out | std::ios_base::binary | std::ios_base::app)) {
            pcap_hdr_t pcap_hdr;
            out_.write((char*) &pcap_hdr, sizeof(pcap_hdr));
        }
        void append(const std::string& msg) {
            out_ << msg;
            out_.flush();
        }
    };
}