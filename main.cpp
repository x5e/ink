#include "main_msgpack.cpp"
#include "main_websocketpp.cpp"

int main(int argc, char** argv) {
    if (argc > 1)
        main_msgpack();
    else
        main_websocketpp();
    return 0;
}