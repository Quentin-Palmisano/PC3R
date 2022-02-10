#include "tapis.h"

void create_tapis(struct Tapis* tapis, size_t capacity) {
    
    tapis->first = 0;
    tapis->last = 0;
    tapis->size = 0;
    tapis->capacity = capacity;
    tapis->packets = malloc(sizeof(struct Packet*) * capacity);
    pthread_mutex_init(&tapis->mutex, NULL);
    pthread_cond_init(&tapis->empty_cond, NULL);
    pthread_cond_init(&tapis->full_cond, NULL);
    
}

void destroy_tapis(struct Tapis* tapis) {
    
    pthread_mutex_destroy(&tapis->mutex);
    pthread_cond_destroy(&tapis->empty_cond);
    pthread_cond_destroy(&tapis->full_cond);
    
}

void put_tapis(struct Tapis* tapis, struct Packet* packet) {
    
    pthread_mutex_lock(&tapis->mutex);
    
    while(tapis->size >= tapis->capacity) {
        pthread_cond_wait(&tapis->full_cond, &tapis->mutex);
    }
    
    tapis->packets[tapis->last] = packet;
    tapis->last = (tapis->last + 1) % tapis->capacity;
    tapis->size++;
    
    pthread_cond_signal(&tapis->empty_cond);
    
    pthread_mutex_unlock(&tapis->mutex);
    
}

struct Packet* take_tapis(struct Tapis* tapis) {
    
    pthread_mutex_lock(&tapis->mutex);
    
    while(tapis->size == 0) {
        pthread_cond_wait(&tapis->empty_cond, &tapis->mutex);
    }
    
    struct Packet* ret = tapis->packets[tapis->first];
    tapis->first = (tapis->first + 1) % tapis->capacity;
    tapis->size--;
    
    pthread_cond_signal(&tapis->full_cond);
    
    pthread_mutex_unlock(&tapis->mutex);
    
    return ret;
}
