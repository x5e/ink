#include <string>
#include <gtest/gtest.h>
#include <thread>

int main_websocketpp();
int main_msgpack();
void client_main(const std::string&);
// greeting = b'\x92\x08\x80'

int main(int argc, char** argv) {
    for (int i=0;i<0;i++) {
        std::cerr << i << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
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