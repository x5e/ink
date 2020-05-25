#include "rows.hpp"
#include "decoder.hpp"


void ink::TrxnRow::decode(ink::cstr_t &ptr, uint32_t vals) {
    id_.decode(ptr);
    story.decode(ptr);
    if (vals >= 3) account.decode(ptr);
    if (vals >= 4) actor.decode(ptr);
    if (vals >= 5) follows = decode_bigint(ptr);
    if (vals >= 6) note.decode(ptr);
}

/*
void ink::PurgeRow::parse(ink::cstr_t &ptr, uint32_t vals) {
    id_.decode(ptr);
    if (vals >= 2) account.decode(ptr);
    if (vals >= 3) row_id.decode(ptr);
}

void ink::BranchRow::parse(ink::cstr_t &ptr, uint32_t ) {
    id_.decode(ptr);
    throw std::runtime_error("not implemented");
}

void ink::MergeRow::parse(ink::cstr_t &ptr, uint32_t ) {
    id_.decode(ptr);
    throw std::runtime_error("not implemented");
}

void ink::NodeRow::parse(ink::cstr_t &ptr, uint32_t ) {
    id_.decode(ptr);
    throw std::runtime_error("not implemented");
}

void ink::EntryRow::parse(ink::cstr_t &ptr, uint32_t ) {
    id_.decode(ptr);
    throw std::runtime_error("not implemented");
}

void ink::PopRow::parse(ink::cstr_t &ptr, uint32_t ) {
    id_.decode(ptr);
    throw std::runtime_error("not implemented");
}
*/