#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fthread.h"

#define PROD_TARGET 10
#define NUM_PROD 10
#define NUM_CONS 10
#define NUM_MSG 10
#define CAPACITY 10


static struct Counter counter;
static struct Tapis tapis_producteur;
static struct Tapis tapis_consommateur;
static struct Journal journal_producteur;
static struct Journal journal_consommateur;
static struct Journal journal_trajet;



struct Packet {
	char* msg;
};

struct Tapis {
	size_t first;
	size_t last;
	size_t size;
	size_t capacity;
	struct Packet** packets;
	ft_scheduler_t sched;
};

struct Tapis* create_tapis(struct Tapis* tapis, size_t capacity) {

    tapis->first = 0;
    tapis->last = 0;
    tapis->size = 0;
    tapis->capacity = capacity;
    tapis->packets = malloc(sizeof(struct Packet*) * capacity);
    tapis->sched = ft_scheduler_create();

    return tapis;

}

void put_tapis(struct Tapis* tapis, struct Packet* packet) {

    if(tapis->size < tapis->capacity) {
        tapis->packets[tapis->last] = packet;
        tapis->last = (tapis->last + 1) % tapis->capacity;
        tapis->size++;
    }

}

struct Packet* take_tapis(struct Tapis* tapis) {

    if(tapis->size > 0) {
        struct Packet* ret = tapis->packets[tapis->first];
        tapis->first = (tapis->first + 1) % tapis->capacity;
        tapis->size--;
        return ret;
    }

    return NULL;
}


struct Journal {
	size_t size;
	struct Packet** packets;
};

void create_journal(struct Journal* journal) {

	journal->size=0;
	journal->packets = malloc(sizeof(struct Packet*) * NUM_PROD*PROD_TARGET);

}

void put_messager(struct Journal* journal, struct Packet* packet){

	journal->packets[journal->size] = packet;
	journal->size++;
}

struct Packet* take_messager(struct Journal* journal){

	if(journal->size > 0) {
		struct Packet* ret = journal->packets[journal->size];
		journal->size--;
		return ret;
	}
	return NULL;

}


struct Counter {
    int remaining;
};

void init_counter(struct Counter* counter, size_t value) {
    counter->remaining = value;
}


void production(void* arg) {
    char* name = (char*) arg;

    for(int i = 0; i<PROD_TARGET; i++) {
        struct Packet* packet = malloc(sizeof(struct Packet));
        packet->msg = malloc(strlen(name) + 10);
        sprintf(packet->msg, "%s %i", name, i+1);
        put_tapis(&tapis_producteur, packet);
    }
    free(name);
    ft_thread_cooperate();
}

void consommation(void* arg) {
    int* id_m = (int*) arg;
    int id = *id_m;
    free(id_m);

    while(counter.remaining > 0) {
        counter.remaining--;

        struct Packet* packet = take_tapis(&tapis_consommateur);
        printf("C%i mange %s\n", id, packet->msg);
        free(packet);
        ft_thread_cooperate();
    }
}

void messager() {

    while(counter.remaining > 0) {
    	ft_thread_link(tapis_producteur.sched);
        struct Packet* packet = take_tapis(&tapis_producteur);
        if(packet!=NULL){
        	put_messager(&journal_trajet, packet);
        	ft_thread_unlink();
        	ft_thread_link(tapis_consommateur.sched);
        	if(tapis_consommateur.size < tapis_consommateur.capacity){
        		put_tapis(&tapis_consommateur, packet);
            	ft_thread_unlink();
                free(packet);
        	}
        }
    }
}

ft_thread_t create_producteur(const char* name, ft_scheduler_t sched) {
    ft_thread_t thread = ft_thread_create(sched, production, NULL, (void*) name);
    return thread;
}

ft_thread_t create_consommateur(int id, ft_scheduler_t sched) {
    int* id_m = malloc(sizeof(int));
    *id_m = id;
    ft_thread_t thread = ft_thread_create(sched, consommation, NULL, (void*) id_m);
    return thread;
}

ft_thread_t create_messager() {
    ft_thread_t thread = ft_thread_create(NULL, messager, NULL, NULL);
    return thread;
}


int main(int argc, char *argv[]) {

    create_tapis(&tapis_producteur, CAPACITY);
    create_tapis(&tapis_consommateur, CAPACITY);
    create_journal(&journal_producteur);
    create_journal(&journal_consommateur);
    create_journal(&journal_trajet);

    init_counter(&counter, NUM_PROD * PROD_TARGET);


    ft_thread_t threads[NUM_PROD + NUM_CONS];
    for(int i = 0; i<NUM_PROD; i++) {
        char* name = malloc(128);
        sprintf(name, "P%i", i);
        threads[i] = create_producteur(name, tapis_producteur.sched);
    }

    for(int i = 0; i<NUM_MSG; i++) {
            threads[NUM_PROD + i] = create_messager();
        }

    for(int i = 0; i<NUM_CONS; i++) {
        threads[NUM_PROD + NUM_MSG + i] = create_consommateur(i,tapis_consommateur.sched);
    }

    ft_scheduler_start(tapis_producteur.sched);
    ft_scheduler_start(tapis_consommateur.sched);


    for(int i = 0; i<NUM_PROD + NUM_CONS + NUM_MSG; i++) {
        ft_thread_join(threads[i]);
    }

    return 0;
}
