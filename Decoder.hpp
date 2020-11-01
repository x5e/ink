#pragma once

#include <memory>
#include <string>
#include <cstddef>
#include "Id.hpp"
#include "typedefs.hpp"
#include "rows.hpp"
#include "Strings.hpp"


namespace ink {

    class Decoder: public Stretch {
        const char *cursor_ = nullptr;

    public:

        explicit Decoder(const Stretch& str): Stretch(str) {}

        error_t decode_message(bigint& msgType, int& rowCount);

        error_t decode_array_prefix(int&);

        error_t decode_bigint(bigint&);

        error_t decode_id(Id&);

        error_t decode_string(Stretch&);

        error_t decode_meta(MetaRow&);

    };


}