#include <gtest/gtest.h>
#include "hdr_t.hpp"
#include <iostream>
#include "muid.hpp"

TEST(muid, parse) {
    std::string eg = "05A20361C016BF-D520000000007-50002";
    ink::muid an_id = ink::muid::parse(eg);
    EXPECT_EQ(an_id.get_wire(), 3749334650716167L);
    EXPECT_EQ(an_id.get_angl(), 327682);
    EXPECT_EQ(an_id.get_muts(), 1585510292133567L);
    std::string eg2{an_id};
    EXPECT_EQ(eg2, eg);
    std::cerr << "okay! " << eg2 << std::endl;
}