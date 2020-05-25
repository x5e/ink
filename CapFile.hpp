#pragma once

#include <map>
#include "typedefs.hpp"
#include "Id.hpp"
#include "Message.hpp"
#include "verify.hpp"

#define MILLION 1'000'000


namespace ink {

    class CapFile {
        const path_t path_;
        const int fd;
        off_t location;
        size_t max_packet_size = 0;
        std::map<muts_t, off_t> index_;
    public:
        explicit CapFile(path_t file_path);

        void receive(Message&);

        muts_t goes_to() const {
            VERIFY(not index_.empty());
            return (--index_.end())->first;
        }
    };


}