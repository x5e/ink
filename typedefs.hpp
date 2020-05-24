#pragma once

#include <cstdint>

namespace ink {
    using guint32 = uint32_t;
    using guint16 = uint16_t;
    using gint32 = uint32_t;

    // https://wiki.wireshark.org/Development/LibpcapFileFormat
    typedef struct pcap_hdr_s {
        guint32 magic_number = 0xa1b2c3d4;   // magic number //
        guint16 version_major = 2;           // major version number //
        guint16 version_minor = 4;           // minor version number //
        gint32 thiszone = 0;                 // GMT to local correction //
        guint32 sigfigs = 0;                 // accuracy of timestamps //
        guint32 snaplen = 0x7FFFFFFF;        // max length of captured packets, in octets //
        guint32 network = 147;               // data link type (private) //
    } pcap_hdr_t;

    typedef struct pcaprec_hdr_s {
        guint32 ts_sec;         /* timestamp seconds */
        guint32 ts_usec;        /* timestamp microseconds */
        guint32 incl_len;       /* number of octets of packet saved in file */
        guint32 orig_len;       /* actual length of packet */
    } pcaprec_hdr_t;

    using muts_t = uint64_t;
    using cstr_t = const char *;
    using buff_t = char *;
    using tag_t = uint64_t;
}