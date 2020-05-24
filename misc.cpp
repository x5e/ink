#include <gtest/gtest.h>
#include <sstream>
#include "misc.hpp"

#define HEX(x) char(x < 10 ? x + '0' : x + 'A' - 10)

std::vector<std::string> ink::split(const std::string &thing, const std::string &how) {
    std::vector<std::string> out;
    size_t blen = how.size();
    size_t loc = 0;
    while (true) {
        size_t x = thing.find(how, loc);
        if (x == std::string::npos) {
            out.push_back(thing.substr(loc));
            return out;
        }
        out.push_back(thing.substr(loc, x - loc));
        loc = x + blen;
    }
}

TEST(misc, split) {
    auto eg = "foo|bar|baz";
    auto out = ink::split(eg, "|");
    EXPECT_EQ(out.size(), 3);
    EXPECT_EQ(out[0], "foo");
    EXPECT_EQ(out[1], "bar");
    EXPECT_EQ(out[2], "baz");
}

void ink::ensure_directory(const std::string &fn) {
    std::string command = "mkdir -p " + fn;
    std::cerr << "running: " << command << std::endl;
    auto result = system(command.c_str());
    VERIFY(result != -1);
}

std::string ink::escapes(const std::string &from) {
    std::stringstream stream;
    for (char c : from) {
        auto i = (uint8_t) c;
        stream << '\\' << 'x' << HEX(i / 16) << HEX(i % 16);
    }
    return stream.str();
}

TEST(misc, escapes) {
    std::string eg = "\x5e\xde";
    auto out = ink::escapes(eg);
    // std::cerr << out << std::endl;
    EXPECT_EQ(out, "\\x5E\\xDE");
}

TEST(misc, flip32) {
    uint32_t input = 0xAB'CD'EF'01;
    auto out = ink::flip32(input);
    uint32_t expected = 0x01'EF'CD'AB;
    EXPECT_EQ(out, expected);
}

TEST(misc, flip64) {
    uint64_t input = 0x11'22'33'44'55'66'77'88;
    auto out = ink::flip64(input);
    uint64_t expected = 0x88'77'66'55'44'33'22'11;
    EXPECT_EQ(out, expected);
}