//
//  main.c
//  rfc4566_parser
//
//  Created by Atakan Efe Kanman on 2017-11-15.
//  Copyright © 2017 Atakan Efe Kanman. All rights reserved.
//

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

#include "Header.h"

int main(int argc, const char * argv[]) {
    
    int file = open("/Users/aekanman/Desktop/rfc4566_parser/rfc4566_parser/video.sdp", O_RDONLY);
    int n;
    char payload[1024];
    struct payload *data;
    
    if (file < 0) {
        return -1;
    }
    
    n = read(file, payload, sizeof(payload));
    
    if (n < 0) {
        return -1;
    }
    
    payload[n] = 0;
    
    return 0;
}
