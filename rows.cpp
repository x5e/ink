#include "rows.hpp"
#include "parser.hpp"


ink::TrxnRow::TrxnRow(ink::cstr_t &ptr, uint32_t vals) {
    id_ = parse_muid(ptr);
    story = parse_muid(ptr);
    if (vals >= 3) account = parse_muid(ptr);
    if (vals >= 4) actor = parse_muid(ptr);
    if (vals >= 5) follows = parse_bigint(ptr);
    if (vals >= 6) note = parse_string(ptr);
}

void ink::PurgeRow::parse(ink::cstr_t &ptr, uint32_t vals) {
    id_ = parse_muid(ptr);
    if (vals >= 2) account = parse_muid(ptr);
    if (vals >= 3) row_id = parse_muid(ptr);
}

void ink::BranchRow::parse(ink::cstr_t &ptr, uint32_t vals) {
    id_ = parse_muid(ptr);
    throw std::runtime_error("not implemented");
}

void ink::MergeRow::parse(ink::cstr_t &ptr, uint32_t vals) {
    id_ = parse_muid(ptr);
    throw std::runtime_error("not implemented");
}

void ink::NodeRow::parse(ink::cstr_t &ptr, uint32_t vals) {
    id_ = parse_muid(ptr);
    throw std::runtime_error("not implemented");
}

void ink::EntryRow::parse(ink::cstr_t &ptr, uint32_t vals) {
    id_ = parse_muid(ptr);
    throw std::runtime_error("not implemented");
}

void ink::PopRow::parse(ink::cstr_t &ptr, uint32_t vals) {
    id_ = parse_muid(ptr);
    throw std::runtime_error("not implemented");
}
