#include <cassert>
#include "Message.hpp"
#include "misc.hpp"
#include "verify.hpp"

using Byte = char;

ink::error_t ink::Message::decode_trxn(ink::TrxnRow& trxnRow) {
    int fields;
    PROPAGATE(decode_array_prefix(fields));
    REQUIRE(fields >= 3);
    bigint tag;
    PROPAGATE(decode_bigint(tag));
    REQUIRE(tag == 0x13);
    PROPAGATE(decode_id(trxnRow.id_));
    PROPAGATE(decode_id(trxnRow.story_));

    if (fields >= 4) PROPAGATE(decode_id(trxnRow.account_));
    if (fields >= 5) PROPAGATE(decode_id(trxnRow.actor_));
    if (fields >= 6) PROPAGATE(decode_bigint(trxnRow.follows_));
    if (fields >= 7) PROPAGATE(decode_string(trxnRow.note_));

    return no_error;
}


ink::error_t ink::Message::decode_array_prefix(int& out) {
    uint32_t count = 0;
    if (((*cursor_) & Byte(0xF0)) == Byte(0x90)) {
        out = ((*cursor_) & Byte(0x0F));
        cursor_ += 1;
        return no_error;
    }
    if ((*cursor_) == Byte(0xdc)) {
        cursor_ += 1;
        count += (uint8_t) *cursor_;
        cursor_ += 1;
        count = count << 8;
        count += (uint8_t) *cursor_;
        out = count;
        cursor_ += 1;
        return no_error;
    }
    if ((*cursor_) == Byte(0xdd)) {
        cursor_ += 1;
        uint32_t flipped = *((uint32_t *) cursor_);
        count = flip32(flipped);
        out = count;
        cursor_ += 4;
        return no_error;
    }
    REQUIRE(false);
}


ink::error_t ink::Message::decode_id(ink::Id& id) {
    auto &ptr = cursor_;
    Byte tag = *ptr++;
    if (tag == Byte(0xc0)) {
        id.zero();
    } else {
        REQUIRE(tag == Byte(0xd8));
        Byte kind = *ptr++;
        REQUIRE(kind == Byte(0x01));
        id.copy_from((char *) ptr);
        ptr += 16;
    }
    return no_error;
}

ink::error_t ink::Message::decode_bigint(int64_t& out) {
    // auto& ptr = cursor_;
    int64_t eight;
    uint32_t four;
    auto thing = *(cursor_++);
    if ((thing & 0x80) == 0) {
        out = thing;
        return no_error;
    }
    switch (thing) {
        case Byte(0xcf):
        case Byte(0xd3):
            eight = *reinterpret_cast<const int64_t *>(cursor_);
            cursor_ += 8;
            out= flip64(eight);
            return no_error;
        case Byte('\xce'):
        case Byte('\xd2'):
            four = *reinterpret_cast<const int32_t *>(cursor_);
            cursor_ += 4;
            out = flip32(four);
            return no_error;
        default:
            REQUIRE(false);
    }
}


ink::error_t ink::Message::decode_string(ink::Stretch& out) {
    size_t count = -1;
    auto tag = *reinterpret_cast<const uint8_t *>(cursor_++);
    if ((tag & 0b1110'0000) == 0b1010'0000) {
        count = tag & 0b0001'1111;
    }
    if (tag == 0xd9 || tag == 0xc4) {
        count = *reinterpret_cast<const uint8_t *>(cursor_++);
    }
    REQUIRE(count != -1);
    auto start_at = cursor_;
    cursor_ += count;
    out = Stretch(start_at, count);
    return no_error;
}


ink::error_t ink::Message::_decode() {
    cursor_ = begin();
    int prefix;
    PROPAGATE(decode_array_prefix(prefix));
    REQUIRE(prefix == 2);
    PROPAGATE(decode_bigint(msgType_));
    REQUIRE(msgType_ == 1);
    PROPAGATE(decode_array_prefix(rowCount_));
    REQUIRE(rowCount_ >= 1);
    PROPAGATE(decode_trxn(trxnRow_));
    REQUIRE(cursor_ <= end());
    decoded_ = true;
    return no_error;
}
