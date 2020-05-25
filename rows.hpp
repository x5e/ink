#pragma once
#include "Id.hpp"


namespace ink {

    struct Row {
        Id id_ = {};
    };

    using String = std::string_view;

    struct TrxnRow : public Row {
        static const tag_t Tag = 0x13;
        Id story_ = {};
        Id account_ = {};
        Id actor_ = {};
        Id request_ = {};
        muts_t follows_ = {};
        String note_ = {};
    };

    /*
    struct PurgeRow: public Row {
        static const tag_t Tag = 0x15;
        Muid account = {};  // TODO references accounts
        Muid row_id = {};
        void parse(cstr_t &ptr, uint32_t vals);
    };

    struct ContentsRow: public Row {};

    struct BranchRow: public ContentsRow {
        String moniker = {};
        void parse(cstr_t &ptr, uint32_t vals);
    };

    struct MergeRow: public ContentsRow {
        Muid branch = {};
        void parse(cstr_t &ptr, uint32_t vals);
    };

    struct NodeRow: public ContentsRow {
        String packed = {};
        String varied = {};
        String vector = {};
        Muid branch = {};
        int nature = 0;
        void parse(cstr_t &ptr, uint32_t vals);
    };

    enum Bool: uint8_t {Nil = 0xc0,False = 0xc2, True = 0xc3};

    struct EntryRow: public ContentsRow {
        Muid node = {};
        Muid reference = {};
        String the_key = {};
        Bool the_bool = Bool::Nil;
        muts_t expiry = 0;
        Muid branch = {};
        void parse(cstr_t &ptr, uint32_t vals);
    };

    struct PopRow: public ContentsRow {
        Muid entry = {};
        muts_t expiry = {};
        Muid branch = {};
        // null entry to indicate deletion of branch
        void parse(cstr_t &ptr, uint32_t vals);
    };
     */
}
