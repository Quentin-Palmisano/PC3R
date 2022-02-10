#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "tapis.h"
#include "fthread.h"

#define PRODUCTEURS 10
#define CIBLE_PRODUCTION 50
#define CONSOMMATEURS 100
#define CAPACITY 10


struct Counter {
    int c;
};

struct Journal {
	size_t first;
	size_t last;
	char** msg;
};

struct Journal* create_journal(size_t capacity) {
    struct Journal* journal = malloc(sizeof(struct Journal));

    journal->msg = malloc(sizeof(char*) * PRODUCTEURS*CIBLE_PRODUCTION);

    return journal;

}

void put_messager(struct Packet* packet, struct Journal journal){

	journal.msg=packet->msg;
}

void take_messager(struct Tapis* tapis, struct Journal journal){

}

struct Tapis* tapis_producteur;
struct Tapis* tapis_consommateur;

static void* production(void* arg) {
    const char* name = (const char*) arg;
    
    return NULL;
}

ft_thread_t create_producteur(const char* name, ft_scheduler_t sched) {
    ft_thread_t thread = ft_thread_create(sched, production, NULL, name);
    printf("%s", name);
    return thread;
}

static void* consommation(void* arg) {
    int* id_m = (int*) arg;
    int id = *id_m;
    free(id_m);
    
    return NULL;
}

ft_thread_t create_consommateur(int id, ft_scheduler_t sched) {
    int* id_m = malloc(sizeof(int));
    *id_m = id;
    ft_thread_t thread = ft_thread_create(sched, consommation, NULL, id);
    printf("%i", id);
    return thread;
}


int main(int argc, char *argv[]) {

	printf("coucou");

    tapis_producteur = create_tapis(CAPACITY);
    tapis_consommateur = create_tapis(CAPACITY);

    create_producteur("Banane", tapis_producteur->sched);
    
    create_consommateur(1, tapis_consommateur->sched);
    
    return 0;
}
