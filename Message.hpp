#pragma once

#include <memory>
#include <string>
#include <cstddef>
#include "Id.hpp"
#include "typedefs.hpp"
#include "rows.hpp"
#include "Span.hpp"


namespace ink {

    class Message: public Span {
        const char *cursor_ = nullptr;
        TrxnRow trxnRow_ = {};
        int64_t msgType_ = 0;
        uint32_t rowCount_ = 0;
        bool decoded_ = false;

        void _decode();

        uint32_t decode_array_prefix();

        int64_t decode_bigint();

        Id decode_id();

        Span decode_string();

        TrxnRow decode_trxn();

    public:
        const TrxnRow &getTrxn() {
            if (not decoded_) _decode();
            return trxnRow_;
        }
        explicit Message(const std::string& str): Span(str) {}


        class DecodeError : public std::runtime_error {
        public:
            DecodeError(const char *fn, int line) : std::runtime_error(std::string(fn) + " " + std::to_string(line)) {}
        };

    };


}