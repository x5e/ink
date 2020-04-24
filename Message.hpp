#pragma once
#include <iterator>
#include <tuple>
#include <limits>
#include "Id.hpp"

namespace ink {
    class Message {
    public:
        muts_t muts{};
        size_t used = 0;
        buff_t buffer;
        size_t allocated;

        explicit Message(size_t size=1): allocated(size) {
            buffer = new char[size];
        }

        void ensure(size_t size);

        ~Message() {delete[] buffer;}
    };
}
