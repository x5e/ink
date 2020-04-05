#pragma once

#include <fstream>
#include "hdr_t.hpp"
#include "misc.hpp"
#include "muid.hpp"


namespace ink {

    class CapFile {
        std::ofstream out_;
        muid story_;
        std::string path_;
    public:
        explicit CapFile(const muid& story) {
            story_ = story;
            path_ = "well/" + story_.get_jell_string() + "/" + std::string(story_);
            ensure_containing_directory(path_);
            out_ = std::ofstream(std::ofstream(path_.c_str(),
                    std::ios_base::out | std::ios_base::binary | std::ios_base::app));
            pcap_hdr_t pcap_hdr;
            out_.write((char*) &pcap_hdr, sizeof(pcap_hdr));
        }
        void append(const std::string& msg);
    };
}