#pragma once

#include <utility>
#include "Id.hpp"
#include "CapFile.hpp"


namespace ink {

    class FileSet {
        path_t directory_;
        int index_fd_;
        std::map<Muid, std::pair<off_t, std::unique_ptr<CapFile>>> cap_files;
        bool opened = false;
        path_t get_location(const Muid &story);

    public:
        // TODO put universe and/or account info in
        error_t open(path_t directory);

        error_t receive(Message &);

        error_t greeting(std::string&);
    };
}