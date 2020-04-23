#pragma once
#include <utility>
#include "muid.hpp"
#include "CapFile.hpp"



namespace ink {

    class FileSet {
        path_t directory_;
        int index_fd_;
        std::map<muid, std::pair<off_t, std::shared_ptr<CapFile>>> cap_files;
        path_t get_location(muid story) {return directory_ + story.get_jell_string() + "/" + std::string(story);}
    public:
        // TODO put universe and/or account info in
        explicit FileSet(path_t directory);

        void receive(const char*, size_t);

        std::string greeting();
    };
}