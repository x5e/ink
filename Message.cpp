#include "Message.hpp"
#include "misc.hpp"

#define DECODE_REQUIRE(x) if (not (x)) throw ::ink::Message::DecodeError(__FILE__, __LINE__)
using Byte = char;

ink::TrxnRow ink::Message::decode_trxn() {
    auto trxnRow = TrxnRow{};
    auto fields = decode_array_prefix();
    DECODE_REQUIRE(fields >= 3);
    auto tag = decode_bigint();
    DECODE_REQUIRE(tag == 0x13);
    trxnRow.id_ = decode_id();
    trxnRow.story_ = decode_id();

    if (fields >= 4) trxnRow.account_ = decode_id();
    if (fields >= 5) trxnRow.actor_ = decode_id();
    if (fields >= 6) trxnRow.follows_ = decode_bigint();
    if (fields >= 7) trxnRow.note_ = decode_string();

    return trxnRow;
}


uint32_t ink::Message::decode_array_prefix() {
    uint32_t count = 0;
    if (((*cursor_) & Byte(0xF0)) == Byte(0x90)) {
        auto aByte = ((*cursor_) & Byte(0x0F));
        cursor_ += 1;
        return aByte;
    }
    if ((*cursor_) == Byte(0xdc)) {
        cursor_ += 1;
        count += (uint8_t) *cursor_;
        cursor_ += 1;
        count = count << 8;
        count += (uint8_t) *cursor_;
        cursor_ += 1;
        return count;
    }
    if ((*cursor_) == Byte(0xdd)) {
        cursor_ += 1;
        uint32_t flipped = *((uint32_t *) cursor_);
        count = flip32(flipped);
        cursor_ += 4;
        return count;
    }
    throw DecodeError(__FILE__, __LINE__);
}


ink::Id ink::Message::decode_id() {
    Id id;
    auto &ptr = cursor_;
    Byte tag = *ptr++;
    if (tag == Byte(0xc0)) {
        id.zero();
    } else {
        DECODE_REQUIRE(tag == Byte(0xd8));
        Byte kind = *ptr++;
        DECODE_REQUIRE(kind == Byte(0x01));
        id.copy_from((char *) ptr);
        ptr += 16;
    }
    return id;
}

int64_t ink::Message::decode_bigint() {
    // auto& ptr = cursor_;
    int64_t eight;
    uint32_t four;
    auto thing = *(cursor_++);
    if ((thing & 0x80) == 0) {
        return thing;
    }
    switch (thing) {
        case Byte(0xcf):
        case Byte(0xd3):
            eight = *reinterpret_cast<const int64_t *>(cursor_);
            cursor_ += 8;
            return flip64(eight);
        case Byte('\xce'):
        case Byte('\xd2'):
            four = *reinterpret_cast<const int32_t *>(cursor_);
            cursor_ += 4;
            return flip32(four);
        default:
            throw DecodeError(__FILE__, __LINE__);
    }
}


ink::Span ink::Message::decode_string() {
    size_t count = -1;
    auto tag = *reinterpret_cast<const uint8_t *>(cursor_++);
    if ((tag & 0b1110'0000) == 0b1010'0000) {
        count = tag & 0b0001'1111;
    }
    if (tag == 0xd9 || tag == 0xc4) {
        count = *reinterpret_cast<const uint8_t *>(cursor_++);
    }
    if (count == -1)
        throw std::runtime_error("not implemented");
    auto start_at = cursor_;
    cursor_ += count;
    return {start_at, count};
}


void ink::Message::_decode() {
    auto prefix = decode_array_prefix();
    DECODE_REQUIRE(prefix == 2);
    msgType_ = decode_bigint();
    DECODE_REQUIRE(msgType_ == 1);
    rowCount_ = decode_array_prefix();
    DECODE_REQUIRE(rowCount_ >= 1);
    trxnRow_ = decode_trxn();
    DECODE_REQUIRE(cursor_ <= end());
    decoded_ = true;
}
