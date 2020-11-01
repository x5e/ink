#pragma once

#include "verify.hpp"
#include "Id.hpp"
#include "misc.hpp"

namespace ink {
    class IndexEntry {
        char data_[2 + 16 + 1 + 8] = {};
    public:
        IndexEntry() {
            static_assert(sizeof(IndexEntry) == 2 + 16 + 1 + 8, "IndexEntry size problem");
            data_[0] = '\xd8';
            data_[1] = '\x01';
            data_[2 + 16] = '\xCF';
        }

        error_t validate() {
            REQUIRE(data_[0] == '\xd8');
            REQUIRE(data_[1] == '\x01');
            REQUIRE(data_[2 + 16] == '\xCF');
            return no_error;
        }

        void set_story(const Muid& story) {
            memcpy(&data_[2], story.data(), 16);
        }

        Muid get_story() const {
            Muid out;
            out.copy_from(&data_[2]);
            return out;
        }

        uint64_t get_value() const {
            uint64_t flipped = *((uint64_t *) &data_[2 + 16 + 1]);
            return flip64(flipped);
        }

        void set_value(uint64_t value) {
            uint64_t flipped = flip64(value);
            memcpy(&data_[2 + 16 + 1], &flipped, 8);
        }
    };

}