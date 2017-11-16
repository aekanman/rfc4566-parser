//
//  Header.h
//  rfc4566_parser
//
//  Created by Atakan Efe Kanman on 2017-11-15.
//  Copyright © 2017 Atakan Efe Kanman. All rights reserved.
//

#ifndef Header_h
#define Header_h

struct origin {
    char *username;
    long long int sess_id;
    long long int sess_ver;
    char *nettype;
    char *addrtype;
    char *unicast_addr;
};

struct connection {
    char *nettype;
    char *addrtype;
    char *address;
};

struct timing {
    time_t start;
    time_t stop;
};

//assumption: no port number
struct media {
    char *media;
    int port;
    char *proto;
    int fmt;
};

struct att_source_filter {
    char *unknown_str;
    char *nettype;
    char *addrtype;
    char *address;
    char *unicast_addr;
};

struct att_rtp_map {
    int payload_type;
    char *encoding_name;
    int clc_rate;
};

struct att_fmtp {
    int format;
    char *sampling;
    int width;
    int height;
    int depth;
    char *colorimetry;
    int interlace;
};

struct att_mediaclk {
    long direct;
    int rate;
};

struct att_tsrefclk {
    char *ptp;
};

struct attributes{
    struct att_source_filter sf;
    struct att_rtp_map rtp;
    struct att_fmtp fmtp;
    struct att_mediaclk mclk;
    struct att_tsrefclk rclk;
};

struct payload {
    int version;
    struct origin orig;
    char *sess_name;
    struct timing time;
    struct media media;
    struct connection conn;
    struct attributes att;
}payload;

#endif /* Header_h */
