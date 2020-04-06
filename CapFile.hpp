#pragma once

#include <fstream>
#include <map>
#include <utility>
#include "hdr_t.hpp"
#include "misc.hpp"
#include "muid.hpp"
#include "verify.hpp"
#include "parser.hpp"

#define MILLION 1000000


namespace ink {
    using path = std::string;

    class CapFile {
        std::ofstream output_;
        std::ifstream innput_;
        muid story_;
        std::string path_;
        std::map<uint64_t, std::ios::pos_type> index_;
    public:
        CapFile(const muid &story, const path& directory);

        void receive(const std::string &, const TrxnRow&);

        uint64_t goes_to() const {
            VERIFY(not index_.empty());
            return (--index_.end())->first;
        }
    };


}