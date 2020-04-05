#include "misc.hpp"

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

void ink::ensure_containing_directory(const std::string &fn) {
    auto parts = split(fn, "/");
    parts.pop_back();
    std::string command = "mkdir -p ";
    for (auto &part: parts) {
        if (not part.empty()) {
            if (part != ".")
                command += "/";
            command += part;
        }
    }
    std::cerr << "running: " << command << std::endl;
    auto result = system(command.c_str());
    VERIFY(result != -1);
}