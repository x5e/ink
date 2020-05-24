#pragma once
#include <utility>
#include "Id.hpp"
#include "CapFile.hpp"



namespace ink {

    class FileSet {
        path_t directory_;
        int index_fd_;
        std::map<Muid, std::pair<off_t, std::unique_ptr<CapFile>>> cap_files;
        path_t get_location(Muid story);
    public:
        // TODO put universe and/or account info in
        explicit FileSet(path_t directory);

        void receive(const char*, size_t);

        std::string greeting();
    };
}