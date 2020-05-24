#include <fcntl.h>
#include <unistd.h>
#include <cstring>
#include "FileSet.hpp"
#include "misc.hpp"
#include "IndexEntry.hpp"


ink::FileSet::FileSet(ink::path_t directory): directory_(std::move(directory))
{
    using namespace std;
    // TODO lock the contents file
    ensure_directory(directory_);
    path_t index_path = directory_ + "/contents.indx";
    touch(index_path);
    index_fd_ = ::open(index_path.c_str(), O_RDWR);
    VERIFY(index_fd_ > 0);
    off_t initial_size = ::lseek(index_fd_, 0, SEEK_END);
    VERIFY(initial_size >= 0);
    off_t location = ::lseek(index_fd_, 0, SEEK_SET);
    VERIFY(location == 0);
    while (location < initial_size) {
        IndexEntry entry;
        auto red = ::read(index_fd_, &entry, sizeof(entry));
        VERIFY(red == sizeof(entry));
        entry.validate();
        Muid story = entry.get_story();
        auto& val = cap_files[story];
        VERIFY(val.first == 0 and val.second.get() == nullptr);
        val.first = location;
        val.second = make_unique<CapFile>(get_location(story));
        location += red;
        auto goes_to = val.second->goes_to();
        auto entry_value = entry.get_value();
        // cerr << "seen " << string(story) << " entry=" << entry_value << " goes_to=" << goes_to << endl;
        VERIFY(goes_to == entry_value);
    }
    VERIFY(location == initial_size);
}


void ink::FileSet::receive(cstr_t ptr, size_t size) {
    DECODE_REQUIRE(*ptr++ == '\x92');
    DECODE_REQUIRE(*ptr++ == '\x01');
    auto row_count = decode_array_prefix(ptr);
    DECODE_REQUIRE(row_count >= 1);
    auto element_count = decode_array_prefix(ptr);
    DECODE_REQUIRE(element_count >= 2);
    auto row_tag = (tag_t) *ptr++;
    DECODE_REQUIRE(row_tag == TrxnRow::Tag);
    auto trxn_row = TrxnRow();
    trxn_row.decode(ptr, element_count - 1);
    std::cerr << "received: " << trxn_row.id_.to_string() << std::endl;
    auto& story = trxn_row.story;
    auto new_muts = trxn_row.id_.get_muts();
    auto& ref = cap_files[story];
    off_t index_offset;
    IndexEntry entry;
    if (ref.second) {
        index_offset = ref.first;
        ::lseek(index_fd_, index_offset, SEEK_SET);
        auto red = ::read(index_fd_, &entry, sizeof(entry));
        VERIFY(red == sizeof(entry));
        VERIFY(entry.get_story() == story);
        VERIFY(entry.get_value() < new_muts);
        ::lseek(index_fd_, index_offset, SEEK_SET);
    } else {
        index_offset = ::lseek(index_fd_, 0, SEEK_END);
        ref.first = index_offset;
        ref.second = std::make_unique<CapFile>(get_location(story));
    }
    ref.second->receive(ptr, size, new_muts);
    entry.set_story(story);
    entry.set_value(new_muts);
    auto written = ::write(index_fd_, &entry, sizeof(entry));
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
    off_t file_size = ::lseek(index_fd_, 0, SEEK_END);
    VERIFY(buffer_size == file_size);
    char* buffer = new char[buffer_size];
    ::lseek(index_fd_, 0, SEEK_SET);
    auto red = read(index_fd_, buffer, buffer_size);
    VERIFY(red == buffer_size);
    stream.write(buffer, buffer_size);
    delete[] buffer;
    return stream.str();
}

ink::path_t ink::FileSet::get_location(ink::Muid story) {
    return directory_ + "/", story.get_jell_string() + "/" + story.to_string();
}

