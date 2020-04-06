#pragma once
#include "IndexEntry.hpp"
#include "misc.hpp"
#include "muid.hpp"
#include "CapFile.hpp"


namespace ink {

    class FileSet {
        path directory_;
        std::ofstream output_;
        std::ifstream innput_;
        std::map<muid, std::ios::pos_type> locations_;
        std::map<muid, std::shared_ptr<CapFile>> cap_files;
    public:
        // TODO put universe and/or account info in
        explicit FileSet(path directory);

        void receive(const std::string &msg);

        std::string greeting();
    };
}