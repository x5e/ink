#pragma once

namespace ink {

    class muid {
        uint8_t data[16] = {};
    public:
        static muid copy_from(char *at) {
            muid out;
            memcpy(out.data, at, 16);
            return out;
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
                out.data[target] =  static_cast<char>(value | out.data[target]);
                j += 1;
            }
            return out;
        }

        uint64_t get_muts() const {
            return
                    (static_cast<uint64_t>(data[0]) << 48) +
                    (static_cast<uint64_t>(data[1]) << 40) +
                    (static_cast<uint64_t>(data[2]) << 32) +
                    (static_cast<uint64_t>(data[3]) << 24) +
                    (static_cast<uint64_t>(data[4]) << 16) +
                    (static_cast<uint64_t>(data[5]) << 8) +
                    (static_cast<uint64_t>(data[6]));
        }

        uint64_t get_wire() const {
            return
                    (static_cast<uint64_t>(data[7]) << 44) +
                    (static_cast<uint64_t>(data[8]) << 36) +
                    (static_cast<uint64_t>(data[9]) << 28) +
                    (static_cast<uint64_t>(data[10]) << 20) +
                    (static_cast<uint64_t>(data[11]) << 12) +
                    (static_cast<uint64_t>(data[12]) << 4) +
                    (static_cast<uint64_t>(data[13]) >> 4);
        }

        uint32_t get_angl() const {
            return
                    ((static_cast<uint64_t>(data[13]) & 0x0F) << 16) +
                    (static_cast<uint64_t>(data[14]) << 8) +
                    static_cast<uint64_t>(data[15]);
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
    };
}