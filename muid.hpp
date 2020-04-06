#pragma once
#include <sstream>
#include <limits>
#include <iomanip>

namespace ink {

    class muid {
        uint8_t data_[16] = {};
    public:
        const char* bytes() const {
            return (char*) data_;
        }

        void copy_from(const char *at) {
            memcpy(data_, at, 16);
        }

        void zero() {
            memset(data_, 0, 16);
        }

        static muid parse(const std::string& hex_str) {
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

        uint64_t get_muts() const {
            return
                    (static_cast<uint64_t>(data_[0]) << 48) +
                    (static_cast<uint64_t>(data_[1]) << 40) +
                    (static_cast<uint64_t>(data_[2]) << 32) +
                    (static_cast<uint64_t>(data_[3]) << 24) +
                    (static_cast<uint64_t>(data_[4]) << 16) +
                    (static_cast<uint64_t>(data_[5]) << 8) +
                    (static_cast<uint64_t>(data_[6]));
        }

        uint64_t get_wire() const {
            return
                    (static_cast<uint64_t>(data_[7]) << 44) +
                    (static_cast<uint64_t>(data_[8]) << 36) +
                    (static_cast<uint64_t>(data_[9]) << 28) +
                    (static_cast<uint64_t>(data_[10]) << 20) +
                    (static_cast<uint64_t>(data_[11]) << 12) +
                    (static_cast<uint64_t>(data_[12]) << 4) +
                    (static_cast<uint64_t>(data_[13]) >> 4);
        }

        uint64_t get_jell() const {
            return get_wire() & std::numeric_limits<uint32_t>::max();
        }

        std::string get_jell_string() const {
            std::stringstream stream;
            stream << std::uppercase << std::setfill ('0') << std::setw(8) << std::hex << get_jell();
            return "0x" + stream.str();
        }

        uint32_t get_angl() const {
            return
                    ((static_cast<uint64_t>(data_[13]) & 0x0F) << 16) +
                    (static_cast<uint64_t>(data_[14]) << 8) +
                    static_cast<uint64_t>(data_[15]);
        }

        explicit operator std::string() const {
            std::stringstream stream;
            stream << std::uppercase << std::setfill ('0') << std::setw(14) << std::hex << get_muts();
            stream << "-";
            stream << std::uppercase << std::setfill ('0') << std::setw(13) << std::hex << get_wire();
            stream << "-";
            stream << std::uppercase << std::setfill ('0') << std::setw(5) << std::hex << get_angl();
            return stream.str();
        }

        bool operator==(const muid& other) const {
            return memcmp(data_, other.data_, sizeof(data_)) == 0;
        }

        bool operator<(muid const& right) const
        {
            return memcmp(data_, right.data_, sizeof(data_)) < 0;
        }
    };
}