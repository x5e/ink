#pragma once
#include "typedefs.hpp"
#include "verify.hpp"
#include "Id.hpp"


namespace ink {

    class Window {
        cstr_t start_;
        cstr_t end_;
    public:
        Window(const Window&) = default;
        Window(cstr_t ptr, size_t size): start_(ptr), end_(start_ + size){}
        uint32_t decode_array_prefix();
        int64_t decode_bigint();
        void decode_id(Id& id);


    };
}