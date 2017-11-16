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
#include <stdlib.h>

#include "Header.h"

static char *next_val(char *p, char *key, char **value);
struct payload *sdp_parse(const char *payload);
static char *split(char *str[], int8_t flag);
static char *format_string(char str[]);
void destroy_data(struct payload *data);
void free_res(char ** res, char ** res_att);

int main(int argc, const char * argv[]) {
    
    int file = open("/Users/aekanman/Desktop/rfc4566_parser/rfc4566_parser/video.sdp", O_RDONLY);
    int n;
    char payload[512];
    struct payload *data;
    
    if (file < 0) {
        return -1;
    }
    
    n = read(file, payload, sizeof(payload));
    
    if (n < 0) {
        return -1;
    }
    
    payload[n] = 0;
    data = sdp_parse(payload);
    
    // use parsed "data"
    
    destroy_data(data);
    return 0;
}

struct payload *sdp_parse(const char *payload)
{
    struct payload *data = calloc(1, sizeof(*data));
    char *p, key, *value, **res = NULL, **res_att = NULL, *tmp;
    char original_data[30] = "";
    
    if (!data)
        goto fail;
    
//    p  = strdup(payload); // uses malloc
    p = payload;
    if (!p)
        goto fail;
    
    /* Protocol version (mandatory, only 0 supported) */
    p = next_val(p, &key, &value);
    if (key != 'v')
        goto fail;
    data->version = value[0] - '0';
    if (data->version != 0 || value[1])
        goto fail;
    
    
    /* Origin field (mandatory) */
    p = next_val(p, &key, &value);
    if (key != 'o')
        goto fail;
    else {
        struct origin *o = &data->orig;
        res = split(&value, 1);
        o->username = res[0];
        o->sess_id = atoi(res[1]);
        o->sess_ver = atoi(res[2]);
        o->nettype = res[3];
        o->addrtype = res[4];
        o->unicast_addr = res[5];
        
        free(res);
    }
    
    /* Session name field (mandatory) */
    p = next_val(p, &key, &value);
    if (key != 's')
        goto fail;
    data->sess_name = value;
    
    struct connection *c = &data->conn;
    struct timing *t = &data->time;
    struct media *m = &data->media;
    struct attributes *att = &data->att;
    struct att_source_filter *sf = &att->sf;
    struct att_rtp_map *rtp = &att->rtp;
    struct att_fmtp *fmtp = &att->fmtp;
    struct att_mediaclk *mclk = &att->mclk;
    struct att_tsrefclk *rclk = &att->rclk;
    
    /*Checking other fields*/
    while (key != NULL){
        p = next_val(p, &key, &value);
        if (value != NULL && key == 'a')
            strcpy(original_data, value);
        
        switch (key){
            case 'c' : // Connection data fields
                res = split(&value, 1);
                c->nettype = res[0];
                c->addrtype = res[1];
                c->address = res[2];

                free(res);
                break;
                
            case 't' : // Timing fields
                res = split(&value, 1);
                t->start = atoi(res[0]);
                t->stop = atoi(res[1]);
                
                free(res);
                break;
                
            case 'm' : // Media descriptions fields
                res = split(&value, 1);
                m->media = res[0];
                m->port = atoi(res[1]);
                m->proto = res[2];
                m->fmt = atoi(res[3]);
                
                free(res);
                break;
                
            case 'a' : // Attributes fields
                // get type
                res = split(&value, 1);
                res_att =  split(&res[0], 0);
                switch (*value){
                    case 'f' : // fmtp
                        fmtp->format = atoi(res_att[1]);
                        fmtp->sampling = format_string(res[1]);;
                        fmtp->width = atoi(format_string(res[2]));
                        fmtp->height = atoi(format_string(res[3]));
                        fmtp->depth = atoi(format_string(res[4]));
                        fmtp->colorimetry = format_string(res[5]);
                        res[6] = strchr(res[6], '=');
                        fmtp->interlace = atoi(++res[6]);
                      
                        free_res(res, res_att);
                    break;

                    case 's' : // source filter
                        sf->unknown_str = res_att[1];;
                        sf->nettype = res[1];
                        sf->addrtype = res[2];
                        sf->address = res[3];
                        sf->unicast_addr = res[4];
                        
                        free_res(res, res_att);
                        break;
                        
                    case 'r' : // rtp_map
                        rtp->payload_type =  atoi(res_att[1]);
                        res_att = split(&res[1], 2);
                        rtp->encoding_name = res_att[0];
                        rtp->clc_rate = atoi(res_att[1]);
                        
                        free_res(res, res_att);
                        break;
                        
                    case 'm' : // mediaclk
                        // create substring after "="
                        res_att[1] = strchr(res_att[1], '=');
                        res[1] = strchr(res[1], '=');
                        
                        mclk->direct =  atoi(++res_att[1]);
                        mclk->rate = atoi(++res[1]);

                        free_res(res, res_att);
                        break;
                        
                    case 't' : //ts_refclk
                        tmp = strchr(original_data, '=');
                        rclk->ptp = ++tmp;
                        
                        free_res(res, res_att);
                        break;
                }
                break;
        }
    }
    free(key);
    free(value);
    
    return data;
fail:
    free(data);
    return NULL;
}

static char *next_val(char *p, char *key, char **value){
    char *endl;
    
    if (!p)
        goto fail;
    
    endl = strstr(p, "\r\n");
    if (!endl)
        endl = strchr(p, '\n');
    
    if (endl)
        while (*endl == '\r' || *endl == '\n')
            *endl++ = '\0';
    else
        endl = &p[strlen(p)];
    
    if (!p[0] || p[1] != '=')
        goto fail;
    
    *key = p[0];
    *value = &p[2];
    
    return endl;
    
fail:
    *key   = 0;
    *value = NULL;
    return NULL;
}

static char *format_string(char str[])
{
    str = strchr(str, '=');
    str[strlen(str)-1] = 0;
    return ++str;
}

static char *split(char *str[], int8_t flag)
{
    char **res = NULL;
    int i = 0, n_spaces = 0;
    char * index = flag ? " " : ":";
    switch (flag) {
        case 0:
            index = ":";
            break;
        case 1:
            index = " ";
            break;
        case 2:
            index = "/";
            break;
        case 3:
            index = "=";
            break;
    }
    char * p = strtok(*str, index);
    
    /* split string and append tokens to 'res' */
    while (p) {
        
        res = realloc (res, sizeof (char*) * ++n_spaces);
        
        if (res == NULL)
            exit (-1);

        res[n_spaces-1] = p;
        
        p = strtok(NULL, index);
    }
    
    free(p);
    return res;
}

void free_res(char ** res, char ** res_att)
{
    free(res);
    free(res_att);
}

void destroy_data(struct payload *data){
    free(&data->att);
    free(&data->conn);
    free(&data->media);
    free(&data->orig);
    free(&data->sess_name);
    free(&data->time);
    free(&data->version);
    free(data);
}
