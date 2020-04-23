
#include <fcntl.h>
#include <unistd.h>
#include "FileSet.hpp"
#include "misc.hpp"
#include "IndexEntry.hpp"


ink::FileSet::FileSet(ink::path_t directory) {
    if (directory.back() != '/')
        directory_ = directory + "/";
    else
        directory_ = directory;
    // TODO lock the contents file
    ensure_directory(directory_);
    path_t index_path = directory_ + "contents.indx";
    touch(index_path);
    fd = ::open(index_path.c_str(), O_RDWR);
    VERIFY(fd > 0);
    off_t initial_size = ::lseek(fd, 0, SEEK_END);
    VERIFY(initial_size >= 0);
    off_t location = ::lseek(fd, 0, SEEK_SET);
    VERIFY(location == 0);
    while (location < initial_size) {
        IndexEntry entry;
        auto red = ::read(fd, &entry, sizeof(entry));
        VERIFY(red == sizeof(entry));
        entry.validate();
        muid story = entry.get_story();
        auto& val = cap_files[story];
        VERIFY(val.first == 0 and val.second.get() == nullptr);
        val.first = location;
        val.second = std::make_shared<CapFile>(story, directory_);
        location += red;
        auto goes_to = val.second->goes_to();
        auto entry_value = entry.get_value();
        std::cerr << "seen " << std::string(story) << " entry=" << entry_value << " goes_to=" << goes_to << std::endl;
        VERIFY(goes_to == entry_value);
    }
    VERIFY(location == initial_size);
}


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
    std::cerr << "received: " << std::string(row.id_) << std::endl;
    muid& story = row.story;
    uint64_t new_muts = row.id_.get_muts();
    auto& ref = cap_files[story];
    off_t index_offset;
    IndexEntry entry;
    if (ref.second) {
        index_offset = ref.first;
        ::lseek(fd, index_offset, SEEK_SET);
        auto red = ::read(fd, &entry, sizeof(entry));
        VERIFY(red == sizeof(entry));
        VERIFY(entry.get_story() == story);
        VERIFY(entry.get_value() < new_muts);
        ::lseek(fd, index_offset, SEEK_SET);
    } else {
        index_offset = ::lseek(fd, 0, SEEK_END);
        ref.first = index_offset;
        ref.second = std::make_shared<CapFile>(row.story, directory_);
    }
    ref.second->receive(msg, row);
    entry.set_story(story);
    entry.set_value(new_muts);
    auto written = ::write(fd, &entry, sizeof(entry));
    VERIFY(written == sizeof(entry));
    // std::cerr << "updated " + std::string(story) + " with value " << new_muts << " at " << index_offset << std::endl;
}

std::string ink::FileSet::greeting() {
    auto count = cap_files.size();
    VERIFY(count < 65536);
    std::stringstream stream;
    stream << '\x92' << '\x08';
    stream << '\xDE' << char(count >> 8) << char(count & 0xFF);
    size_t buffer_size = count * sizeof(IndexEntry);
    off_t file_size = ::lseek(fd, 0, SEEK_END);
    VERIFY(buffer_size == file_size);
    char* buffer = new char[buffer_size];
    ::lseek(fd, 0, SEEK_SET);
    auto red = read(fd, buffer, buffer_size);
    VERIFY(red == buffer_size);
    stream.write(buffer, buffer_size);
    delete[] buffer;
    return stream.str();
}

