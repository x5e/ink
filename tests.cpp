#include <gtest/gtest.h>
#include <iostream>
#include "Id.hpp"
#include "misc.hpp"
#include "Message.hpp"
using namespace ink;

TEST(muid, parse) {
    std::string eg = "05A20361C016BF-D520000000007-50002";
    ink::Muid an_id;
    an_id.parse(eg);
    EXPECT_EQ(an_id.get_wire(), 3749334650716167L);
    EXPECT_EQ(an_id.get_angl(), 327682);
    EXPECT_EQ(an_id.get_muts(), 1585510292133567L);
    EXPECT_EQ(an_id.get_jell(), 7L);
    EXPECT_EQ(an_id.get_jell_string(), "0x00000007");
    std::string eg2{an_id};
    EXPECT_EQ(eg2, eg);
    std::cerr << "okay! " << eg2 << std::endl;
}

TEST(misc, escapes) {
    std::string eg = "\x5e\xde";
    auto out = ink::escapes(eg);
    std::cerr << out << std::endl;
    EXPECT_EQ(out, "\\x5E\\xDE");
}

TEST(Message, ensure) {
    Message message{1};
    message.ensure(9);
    EXPECT_EQ(message.allocated, 16);
}