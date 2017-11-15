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
    int *fmt;
};

struct payload {
    int version;
    struct origin orig;
    char *sess_name;
    struct timing time;
    struct media media;
    struct connection conn;
};


#endif /* Header_h */
