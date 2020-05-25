#include <gtest/gtest.h>
#include "Id.hpp"
#include "verify.hpp"
#include "decoder.hpp"

void ink::Id::parse(const std::string &hex_str) {
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
        data_[target] = static_cast<char>(value | data_[target]);
        j += 1;
    }
}


uint64_t ink::Muid::get_muts() const {
    return
            (static_cast<uint64_t>(data_[0]) << 48) +
            (static_cast<uint64_t>(data_[1]) << 40) +
            (static_cast<uint64_t>(data_[2]) << 32) +
            (static_cast<uint64_t>(data_[3]) << 24) +
            (static_cast<uint64_t>(data_[4]) << 16) +
            (static_cast<uint64_t>(data_[5]) << 8) +
            (static_cast<uint64_t>(data_[6]));
}

uint64_t ink::Muid::get_wire() const {
    return
            (static_cast<uint64_t>(data_[7]) << 44) +
            (static_cast<uint64_t>(data_[8]) << 36) +
            (static_cast<uint64_t>(data_[9]) << 28) +
            (static_cast<uint64_t>(data_[10]) << 20) +
            (static_cast<uint64_t>(data_[11]) << 12) +
            (static_cast<uint64_t>(data_[12]) << 4) +
            (static_cast<uint64_t>(data_[13]) >> 4);
}

uint64_t ink::Muid::get_jell() const {
    return get_wire() & std::numeric_limits<uint32_t>::max();
}

std::string ink::Muid::get_jell_string() const {
    std::stringstream stream;
    stream << std::uppercase << std::setfill('0') << std::setw(8) << std::hex << get_jell();
    return "0x" + stream.str();
}

uint32_t ink::Muid::get_angl() const {
    return
            ((static_cast<uint64_t>(data_[13]) & 0x0F) << 16) +
            (static_cast<uint64_t>(data_[14]) << 8) +
            static_cast<uint64_t>(data_[15]);
}

std::string ink::Muid::to_string() const {
    std::stringstream stream;
    stream << std::uppercase << std::setfill('0') << std::setw(14) << std::hex << get_muts();
    stream << "-";
    stream << std::uppercase << std::setfill('0') << std::setw(13) << std::hex << get_wire();
    stream << "-";
    stream << std::uppercase << std::setfill('0') << std::setw(5) << std::hex << get_angl();
    return stream.str();
}

bool ink::Id::operator==(const ink::Id &other) const {
    return memcmp(data_, other.data_, sizeof(data_)) == 0;
}

bool ink::Id::operator<(const ink::Id &right) const {
    return memcmp(data_, right.data_, sizeof(data_)) < 0;
}

void ink::Id::zero() {
    memset(data_, 0, 16);
}

void ink::Id::copy_from(const char *at) {
    memcpy(data_, at, 16);
}

const char *ink::Id::data() const noexcept {
    return (char *) data_;
}

std::string ink::Uuid::to_string() const {
    std::stringstream stream;
    /*
    stream << std::uppercase << std::setfill ('0') << std::setw(14) << std::hex << get_muts();
    stream << "-";
    stream << std::uppercase << std::setfill ('0') << std::setw(13) << std::hex << get_wire();
    stream << "-";
    stream << std::uppercase << std::setfill ('0') << std::setw(5) << std::hex << get_angl();
     */
    VERIFY(false);
    return stream.str();
}


TEST(Id, parse) {
    using namespace ink;
    std::string eg = "05A20361C016BF-D520000000007-50002";
    ink::Muid an_id;
    an_id.parse(eg);
    EXPECT_EQ(an_id.get_wire(), 3749334650716167L);
    EXPECT_EQ(an_id.get_angl(), 327682);
    EXPECT_EQ(an_id.get_muts(), 1585510292133567L);
    EXPECT_EQ(an_id.get_jell(), 7L);
    EXPECT_EQ(an_id.get_jell_string(), "0x00000007");
    std::string eg2 = an_id.to_string();
    EXPECT_EQ(eg2, eg);
    // std::cerr << "okay! " << eg2 << std::endl;
}
