#include "muid.hpp"

ink::muid ink::muid::parse(const std::string &hex_str) {
    muid out;
    size_t j = 0;
    for (char hex_digit : hex_str) {
        int value = 0;
        switch (hex_digit & 0xF0) {
            case 0x40:
            case 0x60:
                value += 9;
            case 0x30:
                value += 0x0F & hex_digit;
                break;
            default:
                continue;
        }
        if (j >= 32) break;
        if ((j & 1) == 0) {
            value = value << 4;
        }
        size_t target = j >> 1;
        out.data_[target] =  static_cast<char>(value | out.data_[target]);
        j += 1;
    }
    return out;
}

uint64_t ink::muid::get_muts() const {
    return
            (static_cast<uint64_t>(data_[0]) << 48) +
            (static_cast<uint64_t>(data_[1]) << 40) +
            (static_cast<uint64_t>(data_[2]) << 32) +
            (static_cast<uint64_t>(data_[3]) << 24) +
            (static_cast<uint64_t>(data_[4]) << 16) +
            (static_cast<uint64_t>(data_[5]) << 8) +
            (static_cast<uint64_t>(data_[6]));
}

uint64_t ink::muid::get_wire() const {
    return
            (static_cast<uint64_t>(data_[7]) << 44) +
            (static_cast<uint64_t>(data_[8]) << 36) +
            (static_cast<uint64_t>(data_[9]) << 28) +
            (static_cast<uint64_t>(data_[10]) << 20) +
            (static_cast<uint64_t>(data_[11]) << 12) +
            (static_cast<uint64_t>(data_[12]) << 4) +
            (static_cast<uint64_t>(data_[13]) >> 4);
}

uint64_t ink::muid::get_jell() const {
    return get_wire() & std::numeric_limits<uint32_t>::max();
}

std::string ink::muid::get_jell_string() const {
    std::stringstream stream;
    stream << std::uppercase << std::setfill ('0') << std::setw(8) << std::hex << get_jell();
    return "0x" + stream.str();
}

uint32_t ink::muid::get_angl() const {
    return
            ((static_cast<uint64_t>(data_[13]) & 0x0F) << 16) +
            (static_cast<uint64_t>(data_[14]) << 8) +
            static_cast<uint64_t>(data_[15]);
}

ink::muid::operator std::string() const {
    std::stringstream stream;
    stream << std::uppercase << std::setfill ('0') << std::setw(14) << std::hex << get_muts();
    stream << "-";
    stream << std::uppercase << std::setfill ('0') << std::setw(13) << std::hex << get_wire();
    stream << "-";
    stream << std::uppercase << std::setfill ('0') << std::setw(5) << std::hex << get_angl();
    return stream.str();
}

bool ink::muid::operator==(const ink::muid &other) const {
    return memcmp(data_, other.data_, sizeof(data_)) == 0;
}

bool ink::muid::operator<(const ink::muid &right) const {
    return memcmp(data_, right.data_, sizeof(data_)) < 0;
}

void ink::muid::zero() {
    memset(data_, 0, 16);
}

void ink::muid::copy_from(const char *at) {
    memcpy(data_, at, 16);
}

const char *ink::muid::data() const noexcept {
    return (char*) data_;
}
