#ifndef TAPIS_H
#define TAPIS_H

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

struct Packet {
    char* msg;
};

struct Tapis {
    size_t first;
    size_t last;
    size_t size;
    size_t capacity;
    struct Packet** packets;
    pthread_mutex_t mutex;
    pthread_cond_t empty_cond;
    pthread_cond_t full_cond;
    
};

void create_tapis(struct Tapis* tapis, size_t capacity);

void destroy_tapis(struct Tapis* tapis);

void put_tapis(struct Tapis* tapis, struct Packet* packet);

struct Packet* take_tapis(struct Tapis* tapis);


#endif
