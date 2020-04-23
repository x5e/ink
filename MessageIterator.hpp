#pragma once
#include <iterator>
#include <tuple>
#include "muid.hpp"

namespace ink {
    class MessageIterator: public std::iterator<std::input_iterator_tag, std::tuple<muts_t, ccp_t, size_t>> {
        char* buffer;

        ~MessageIterator() {free(buffer);}
    };
}
