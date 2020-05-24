#pragma once

#include <map>
#include "typedefs.hpp"
#include "Id.hpp"
#include "decoder.hpp"
#include "Message.hpp"

#define MILLION 1'000'000


namespace ink {
    using path_t = std::string;

    class CapFile {
        const path_t path_;
        const int fd;
        off_t location;
        size_t max_packet_size = 0;
        std::map<muts_t, off_t> index_;
    public:
        explicit CapFile(path_t file_path);

        void receive(const char*, size_t, muts_t);

        std::iterator<std::input_iterator_tag, Message> begin() { throw std::runtime_error(path_); }

        muts_t goes_to() const {
            VERIFY(not index_.empty());
            return (--index_.end())->first;
        }
    };


}