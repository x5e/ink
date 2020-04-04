#include <string>
#include <iostream>
#include <gtest/gtest.h>
#include "hdr_t.hpp"

int main_websocketpp();
int main_msgpack();
void client_main(const std::string&);
// greeting = b'\x92\x08\x80'

int main(int argc, char** argv) {
    main_msgpack();
    for (int i=0; i < argc; i++) {
        if (std::string(argv[i]).find("--gtest") != std::string::npos) {
            ::testing::InitGoogleTest(&argc, argv);
            return RUN_ALL_TESTS();
        }
    }

    if (argc > 1) {
        client_main(argv[1]);
    } else {
        main_websocketpp();
    }
    return 0;
}