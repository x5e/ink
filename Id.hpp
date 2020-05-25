#pragma once
#include <sstream>
#include <limits>
#include <iomanip>
#include "typedefs.hpp"

namespace ink {


    struct Id {
        uint8_t data_[16] = {};

        /**
         * Copies 16 bytes worth of data from the input arg.
         * @param at
         */
        void copy_from(cstr_t at);

        /**
         * returns data as a c-string (const char*)
         */
        cstr_t data() const noexcept;

        /**
         * Copies the bytes from other.
         */
        bool operator==(const Id& other) const;

        /**
         * Compares the bytes in order of the two Ids
         */
        bool operator<(Id const& right) const;

        /**
         * Sets all the bytes to zero.
         */
        void zero();

        /**
         * Populates this using the hex characters (case insensitive).  Ignores non-hex characters.
         * @param hex_str
         */
        void parse(const std::string& hex_str);

    };

    struct Muid: public Id {

        /**
         * Treat the first seven bytes as a big-endian representation of time in micro seconds.
         * @return The implied timestamp assuming this is a Muid.
         */
        uint64_t get_muts() const;

        /**
         * Treat 13 hex digits (after the first 7 bytes) as the "wire" or unique publisher ID.
         * @return
         */
        uint64_t get_wire() const;

        /**
         *
         * @return the lower four bytes of the wire for this Muid, which will generally correspond to the account id.
         */
        uint64_t get_jell() const;

        /**
         *
         * @return A string representation of the jell for this Id, in hex, including the leading 0x
         */
        std::string get_jell_string() const;
        uint32_t get_angl() const;
        std::string to_string() const;
    };

    struct Uuid: public Id {
        std::string to_string() const;
    };
}