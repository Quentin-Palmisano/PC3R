#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include "tapis.h"

#define PROD_TARGET 10
#define NUM_PROD 10
#define NUM_CONS 10
#define CAPACITY 10


static struct Tapis tapis_principal;

struct Counter {
    int remaining;
};

void init_counter(struct Counter* counter, size_t value) {
    counter->remaining = value;
}

static struct Counter counter;

static void* production(void* arg) {
    char* name = (char*) arg;
    
    for(int i = 0; i<PROD_TARGET; i++) {
        struct Packet* packet = malloc(sizeof(struct Packet));
        packet->msg = malloc(strlen(name) + 10);
        sprintf(packet->msg, "%s %i", name, i+1);
        put_tapis(&tapis_principal, packet);
    }
    
    free(name);
    
    return NULL;
}

static void* consommation(void* arg) {
    int* id_m = (int*) arg;
    int id = *id_m;
    free(id_m);
    
    while(counter.remaining > 0) {
        counter.remaining--;
        
        struct Packet* packet = take_tapis(&tapis_principal);
        printf("C%i mange %s\n", id, packet->msg);
        free(packet);
    }
    
    return NULL;
}

pthread_t create_producteur(const char* name) {
    pthread_t thread;
    pthread_create(&thread, NULL, &production, (void*) name);
    return thread;
}

pthread_t create_consommateur(int id) {
    int* id_m = malloc(sizeof(int));
    *id_m = id;
    pthread_t thread;
    pthread_create(&thread, NULL, &consommation, (void*) id_m);
    return thread;
}


int main(int argc, char *argv[]) {
    
    create_tapis(&tapis_principal, CAPACITY);
    
    init_counter(&counter, NUM_PROD * PROD_TARGET);
    
    
    pthread_t threads[NUM_PROD + NUM_CONS];
    for(int i = 0; i<NUM_PROD; i++) {
        char* name = malloc(128);
        sprintf(name, "P%i", i);
        threads[i] = create_producteur(name);
    }
    
    for(int i = 0; i<NUM_CONS; i++) {
        threads[NUM_PROD + i] = create_consommateur(i);
    }
    
    for(int i = 0; i<NUM_PROD + NUM_CONS; i++) {
        pthread_join(threads[i], NULL);
    }
    
    return 0;
}
