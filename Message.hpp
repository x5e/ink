#pragma once

#include <memory>
#include <string>
#include <string_view>
#include <cstddef>
#include "Id.hpp"
#include "typedefs.hpp"
#include "rows.hpp"


namespace ink {

    class Message {
        std::string_view view_;
        const char *cursor_ = nullptr;
        TrxnRow trxnRow_ = {};
        int64_t msgType_ = 0;
        uint32_t rowCount_ = 0;
        bool decoded_ = false;

        void _decode();

        uint32_t decode_array_prefix();

        int64_t decode_bigint();

        Id decode_id();

        std::string_view decode_string();

        TrxnRow decode_trxn();

    public:
        const TrxnRow &getTrxn() {
            if (not decoded_) _decode();
            return trxnRow_;
        }

        Message() = default;

        Message &operator=(const std::string_view &view) {
            view_ = view;
            decoded_ = false;
            return *this;
        }

        size_t size() const { return view_.size(); }

        const char *data() const { return view_.data(); }

        class DecodeError : public std::runtime_error {
        public:
            DecodeError(const char *fn, int line) : std::runtime_error(std::string(fn) + " " + std::to_string(line)) {}
        };

    };


}