#pragma once

#include "verify.hpp"
#include "muid.hpp"
#include "misc.hpp"

namespace ink {
    class IndexEntry {
        char data_[2 + 16 + 1 + 8] = {};
    public:
        IndexEntry() {
            VERIFY(sizeof(IndexEntry) == 2 + 16 + 1 + 8);
            data_[0] = '\xd8';
            data_[1] = '\x01';
            data_[2 + 16] = '\xCF';
        }

        void validate() {
            VERIFY(data_[0] == '\xd8');
            VERIFY(data_[1] == '\x01');
            VERIFY(data_[2+16] == '\xCF');
        }

        void set_story(muid story) {
            memcpy(&data_[2], story.data(), 16);
        }

        muid get_story() const {
            muid out;
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