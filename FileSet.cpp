#include <fcntl.h>
#include <unistd.h>
#include <cstring>
#include "FileSet.hpp"
#include "misc.hpp"
#include "IndexEntry.hpp"
#include "rows.hpp"
#include "Message.hpp"

ink::error_t ink::FileSet::open(ink::path_t directory)
{
    using namespace std;
    directory_ = std::move(directory);
    // TODO lock the contents file
    ensure_directory(directory_);
    path_t index_path = directory_ + "/contents.indx";
    touch(index_path);
    index_fd_ = ::open(index_path.c_str(), O_RDWR);
    REQUIRE(index_fd_ > 0);
    off_t initial_size = ::lseek(index_fd_, 0, SEEK_END);
    REQUIRE(initial_size >= 0);
    off_t location = ::lseek(index_fd_, 0, SEEK_SET);
    REQUIRE(location == 0);
    while (location < initial_size) {
        IndexEntry entry;
        auto red = ::read(index_fd_, &entry, sizeof(entry));
        REQUIRE(red == sizeof(entry));
        entry.validate();
        Muid story = entry.get_story();
        auto& val = cap_files[story];
        REQUIRE(val.first == 0 and val.second.get() == nullptr);
        val.first = location;
        val.second = make_unique<CapFile>();
        PROPAGATE(val.second->open(get_location(story)));
        location += red;
        muts_t goes_to;
        PROPAGATE(val.second->goes_to(goes_to));
        auto entry_value = entry.get_value();
        // cerr << "seen " << string(story) << " entry=" << entry_value << " goes_to=" << goes_to << endl;
        REQUIRE(goes_to == entry_value);
    }
    REQUIRE(location == initial_size);
    opened = true;
    return no_error;
}


ink::error_t ink::FileSet::receive(Message& message) {
    REQUIRE(opened);
    auto& trxn_row = message.getTrxn();
    std::cerr << "received: " << trxn_row.id_.to_string() << std::endl;
    auto& story = trxn_row.story_;
    auto new_muts = trxn_row.id_.get_muts();
    auto& ref = cap_files[story];
    off_t index_offset;
    IndexEntry entry;
    if (ref.second) {
        index_offset = ref.first;
        ::lseek(index_fd_, index_offset, SEEK_SET);
        auto red = ::read(index_fd_, &entry, sizeof(entry));
        REQUIRE(red == sizeof(entry));
        REQUIRE(entry.get_story() == story);
        REQUIRE(entry.get_value() < new_muts);
        ::lseek(index_fd_, index_offset, SEEK_SET);
    } else {
        index_offset = ::lseek(index_fd_, 0, SEEK_END);
        ref.first = index_offset;
        ref.second = std::make_unique<CapFile>(get_location(story));
    }
    ref.second->receive(message);
    entry.set_story(story);
    entry.set_value(new_muts);
    auto written = ::write(index_fd_, &entry, sizeof(entry));
    REQUIRE(written == sizeof(entry));
    return no_error;
    // std::cerr << "updated " + std::string(story) + " with value " << new_muts << " at " << index_offset << std::endl;
}

ink::error_t ink::FileSet::greeting(std::string& out) {
    auto count = cap_files.size();
    REQUIRE(count < 65536);
    std::stringstream stream;
    stream << '\x92' << '\x08';
    stream << '\xDE' << char(count >> 8) << char(count & 0xFF);
    size_t buffer_size = count * sizeof(IndexEntry);
    off_t file_size = ::lseek(index_fd_, 0, SEEK_END);
    REQUIRE(buffer_size == file_size);
    char* buffer = new char[buffer_size];
    ::lseek(index_fd_, 0, SEEK_SET);
    auto red = read(index_fd_, buffer, buffer_size);
    REQUIRE(red == buffer_size);
    stream.write(buffer, buffer_size);
    delete[] buffer;
    out = stream.str();
    return no_error;
}

ink::path_t ink::FileSet::get_location(const ink::Muid& story) {
    return directory_ + "/", story.get_jell_string() + "/" + story.to_string();
}

