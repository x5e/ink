#include <cstdio>
#include "signaling.hpp"

sig_atomic_t ink::signaled = 0;

void ink::handler(int param) {
    signaled = param;
    fprintf(stderr, "got signal: %d\n", param);
}

void ink::setupSig() {
    static bool called = false;
    if (called) return;
    // see http://tinyurl.com/lulc752 (stackoverflow)
    struct sigaction sigIntHandler = {};
    sigIntHandler.sa_handler = handler;
    sigemptyset(&sigIntHandler.sa_mask);
    sigIntHandler.sa_flags = 0;
    sigaction(SIGINT, &sigIntHandler, nullptr);  // ctrl-c
    sigaction(SIGTERM, &sigIntHandler, nullptr);
    sigaction(SIGTSTP, &sigIntHandler, nullptr); // ctrl-z
    // sigaction(SIGQUIT, &sigIntHandler, nullptr); // ctrl-\
    //signal(SIGSEGV,onSegFault);
    //set_terminate(onTerminate);
    called = true;
}