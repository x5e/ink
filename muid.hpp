#pragma once
#include <sstream>
#include <limits>
#include <iomanip>

namespace ink {

    using muts_t = uint64_t;
    using ccp_t = const char *;

    class muid {
        uint8_t data_[16] = {};
    public:
        const char* data() const noexcept;

        void copy_from(const char *at);

        void zero();

        static muid parse(const std::string& hex_str);

        uint64_t get_muts() const;

        uint64_t get_wire() const;

        uint64_t get_jell() const;

        std::string get_jell_string() const;

        uint32_t get_angl() const;

        explicit operator std::string() const;

        bool operator==(const muid& other) const;

        bool operator<(muid const& right) const;
    };
}