#pragma once
#include <csignal>
namespace ink {
    using std::sig_atomic_t;
    extern sig_atomic_t signaled;
    void handler(int param);
    void setupSig();
}