#pragma once
// #include <string_view>
#include <boost/utility/string_ref.hpp>

namespace ink {
    /*
    class Span {
        const char* ptr_ = nullptr;
        std::size_t size_ = 0;
    public:
        explicit Span(const std::string& str): ptr_(str.data()), size_(str.size()) {}
        Span(const char* ptr, std::size_t size): ptr_(ptr), size_(size) {}
        Span() = default;
        size_t size() const { return size_; }
        const char* data() const { return ptr_; }
        const char* end() const { return ptr_ + size_; }
    };
     */
    // using Span = std::string_view;
    using Stretch = boost::string_ref;
}