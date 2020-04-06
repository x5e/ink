//
// Created by Darin McGill on 4/6/20.
//

#include "FileSet.hpp"


void ink::FileSet::receive(const std::string& msg) {
    const char* ptr = msg.data();
    if (*ptr++ != '\x92')
        throw parse_error(__FILE__, __LINE__);
    if (*ptr++ != '\x01')
        throw parse_error(__FILE__, __LINE__);
    int rows = parse_array_prefix(ptr);
    if (rows < 1)
        throw parse_error(__FILE__, __LINE__);
    TrxnRow row(ptr);
    std::cerr << "received: " << std::string(row.id_) << " " << row.id_.get_jell_string() << std::endl;
    auto& ref = cap_files[row.story];
    bool new_sequence = false;
    if (not ref) {
        new_sequence = true;
        ref = std::make_shared<CapFile>(row.story, directory_);
    }
    ref->receive(msg, row);
    IndexEntry entry;
    std::ios::pos_type start_entry;
    muid& story = row.story;
    if (new_sequence) {
        output_.seekp(0, output_.end);
        start_entry = locations_[story] = output_.tellp();
        entry.set_story(story);
    } else {
        start_entry = locations_[row.story];
        output_.seekp(start_entry);
        innput_.seekg(start_entry);
        innput_.read((char*) &entry, sizeof(entry));
        VERIFY(entry.get_story() == row.story);
        VERIFY(entry.get_value() < row.id_.get_muts());
    }
    entry.set_value(row.id_.get_muts());
    output_.write((char*) &entry, sizeof(entry));
    output_.flush();
    std::cerr << "wrote " + std::string(story) + " with value " << entry.get_value() << " to " << start_entry << std::endl;
}

std::string ink::FileSet::greeting() {
    auto count = cap_files.size();
    VERIFY(count < 65536);
    std::stringstream stream;
    stream << '\x92' << '\x08';
    stream << '\xDE' << char(count >> 8) << char(count & 0xFF);
    size_t buffer_size = count * sizeof(IndexEntry);
    char* buffer = new char[buffer_size];
    innput_.seekg(0, innput_.end);
    innput_.read(buffer, buffer_size);
    stream.write(buffer, buffer_size);
    return stream.str();
}

ink::FileSet::FileSet(ink::path directory) {
    if (directory.back() != '/')
        directory_ = directory + "/";
    else
        directory_ = directory;
    // TODO lock the contents file
    ensure_directory(directory_);
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
        auto goes_to = cap_files[story]->goes_to();
        auto entry_value = entry.get_value();
        std::cerr << "seen " << std::string(story) << " entry=" << entry.get_value() << " goes_to=" << goes_to << std::endl;
        VERIFY(goes_to == entry_value);
    }
}
