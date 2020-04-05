#include <string>
#include <vector>
#include "verify.hpp"

namespace ink {
    std::vector<std::string> split(const std::string &thing, const std::string &how);
    void ensure_containing_directory(const std::string &fn);
}