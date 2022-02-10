#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "tapis.h"

#define PROD_TARGET 10
#define CONS_TARGET 100
#define CAPACITY 10


struct Counter {
    int c;
};

static struct Tapis* tapis_principal;

static void* production(void* arg) {
    const char* name = (const char*) arg;
    
    return NULL;
}

pthread_t create_producteur(const char* name) {
    pthread_t thread;
    pthread_create(&thread, NULL, &production, (void*) name);
    return thread;
}

static void* consommation(void* arg) {
    int* id_m = (int*) arg;
    int id = *id_m;
    free(id_m);
    
    return NULL;
}

pthread_t create_consommateur(int id) {
    int* id_m = malloc(sizeof(int));
    *id_m = id;
    pthread_t thread;
    pthread_create(&thread, NULL, &consommation, (void*) id_m);
    return thread;
}


int main(int argc, char *argv[]) {
    
    tapis_principal = create_tapis(CAPACITY);
    
    
    
    create_producteur("Banane");
    
    create_consommateur(1);
    
    return 0;
}
