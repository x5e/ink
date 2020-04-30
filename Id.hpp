#pragma once
#include <sstream>
#include <limits>
#include <iomanip>

namespace ink {
    using muts_t = uint64_t;
    using cstr_t = const char *;
    using buff_t = char *;
    using tag_t = uint64_t;
    using String = std::string;

    template<typename T>
    using Fancy = std::shared_ptr<T>;

    template<typename T, typename A, typename B>
    Fancy<T> make_fancy(A a, B b) {return std::make_shared<T>(a, b);}


    struct Id {
        uint8_t data_[16] = {};
        void copy_from(cstr_t at);
        const char* data() const noexcept;
        bool operator==(const Id& other) const;
        bool operator<(Id const& right) const;
        void zero();
        void parse(const std::string& hex_str);
    };

    struct Muid: public Id {
        uint64_t get_muts() const;
        uint64_t get_wire() const;
        uint64_t get_jell() const;
        std::string get_jell_string() const;
        uint32_t get_angl() const;
        explicit operator std::string() const;
    };

    struct Uuid: public Id {
        explicit operator std::string() const;
    };
}