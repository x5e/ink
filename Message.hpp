#pragma once

#include <memory>
#include <string>
#include <cstddef>
#include "Id.hpp"
#include "typedefs.hpp"
#include "rows.hpp"
#include "Strings.hpp"


namespace ink {

    class Message: public Stretch {
        const char *cursor_ = nullptr;
        TrxnRow trxnRow_ = {};
        bigint msgType_ = 0;
        int rowCount_ = 0;
        bool decoded_ = false;

        error_t _decode();

        error_t decode_array_prefix(int&);

        error_t decode_bigint(bigint&);

        error_t decode_id(Id&);

        error_t decode_string(Stretch&);

        error_t decode_trxn(TrxnRow&);

    public:
        const TrxnRow &getTrxn() {
            if (not decoded_) _decode();
            return trxnRow_;
        }
        explicit Message(const std::string& str): Stretch(str) {}


    };


}