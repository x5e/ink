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
        explicit FileSet(path directory) {
            if (directory.back() != '/')
                directory_ = directory + "/";
            else
                directory_ = directory;
            // TODO lock the contents file
            path index_path = directory_ + "contents.indx";
            output_.open(index_path.c_str(), std::ios_base::binary | std::ios_base::app | std::ios_base::ate);
            VERIFY(output_.is_open());
            innput_.open(index_path.c_str(), std::ios_base::binary);
            VERIFY(innput_.is_open());
            while (innput_.tellg() != output_.tellp()) {
                IndexEntry entry;
                auto location = innput_.tellg();
                innput_.read((char*) &entry, sizeof(entry));
                entry.validate();
                muid story = entry.get_story();
                locations_[story] = location;
                cap_files[story] = std::make_shared<CapFile>(story, directory_);
                VERIFY(cap_files[story]->goes_to() == entry.get_value());
            }
        }

        void receive(const std::string &msg);

        std::string greeting() const;
    };
}