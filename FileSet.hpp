#pragma once
#include <utility>
#include "muid.hpp"
#include "CapFile.hpp"



namespace ink {

    class FileSet {
        path directory_;
        int fd;
        std::map<muid, std::pair<off_t, std::shared_ptr<CapFile>>> cap_files;
    public:
        // TODO put universe and/or account info in
        explicit FileSet(path directory);

        void receive(const std::string &msg);

        std::string greeting();
    };
}