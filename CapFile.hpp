#pragma once

#include <map>
#include "typedefs.hpp"
#include "Id.hpp"
#include "Message.hpp"
#include "verify.hpp"

#define MILLION 1'000'000


namespace ink {

    class CapFile {
        path_t path_;
        int fd;
        off_t location;
        size_t max_packet_size = 0;
        std::map<muts_t, off_t> index_;
    public:
        error_t open(path_t file_path);

        error_t receive(Message&);

        error_t goes_to(muts_t& out) const {
            REQUIRE(not index_.empty());
            out = (--index_.end())->first;
            return no_error;
        }
    };


}