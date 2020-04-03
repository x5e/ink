#include <string>
#include <iostream>
int main_websocketpp();
int main_msgpack();
void client_main(std::string);
// greeting = b'\x92\x08\x80'

int main(int argc, char** argv) {
    if (argc > 1) {
        client_main(argv[1]);
    } else {
        std::cerr << "called without an arg?" << std::endl;
        main_msgpack();
        main_websocketpp();
    }
    return 0;
}