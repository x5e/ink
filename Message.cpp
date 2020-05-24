#include <gtest/gtest.h>
#include "Message.hpp"

void ink::Message::ensure(size_t size) {
    if (size > allocated) {
        for (size_t i = 1; i > 0; i = i << 1) {
            if (i > size) {
                delete[] buffer;
                buffer = new char[i];
                allocated = i;
                return;
            }
        }
        throw std::runtime_error("reallocation problem");
    }
}


TEST(Message, ensure) {
    using namespace ink;
    Message message{1};
    message.ensure(9);
    EXPECT_EQ(message.allocated, 16);
}