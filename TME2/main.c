#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tapis.h"

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
static struct Sched sched;

struct Sched {
	ft_scheduler_t prod;
	ft_scheduler_t cons;
};

void create_sched(struct Sched* sched, ft_scheduler_t prod, ft_scheduler_t cons) {
	sched->prod=prod;
	sched->cons=cons;
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


static void* production(void* arg) {
    char* name = (char*) arg;

    for(int i = 0; i<PROD_TARGET; i++) {
        struct Packet* packet = malloc(sizeof(struct Packet));
        packet->msg = malloc(strlen(name) + 10);
        sprintf(packet->msg, "%s %i", name, i+1);
        put_tapis(&tapis_producteur, packet);
    }
    free(name);
    ft_thread_cooperate();

    return NULL;
}

static void* consommation(void* arg) {
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

    return NULL;
}

static void* messager(struct Sched* sched) {

    while(counter.remaining > 0) {
    	ft_thread_link(sched->prod);
        struct Packet* packet = take_tapis(&tapis_producteur);
        if(packet!=NULL){
        	put_messager(&journal_trajet, packet);
        	ft_thread_unlink();
        	ft_thread_link(sched->cons);
        	if(tapis_consommateur.size < tapis_consommateur->capacity){
        		put_tapis(&tapis_consommateur, packet);
            	ft_thread_unlink();
                free(packet);
        	}
        }
    }

    return NULL;
}

ft_thread_t create_producteur(const char* name, ft_scheduler_t sched) {
    ft_thread_t thread = ft_thread_create(sched, &production, NULL, (void*) name);
    return thread;
}

ft_thread_t create_consommateur(int id, ft_scheduler_t sched) {
    int* id_m = malloc(sizeof(int));
    *id_m = id;
    ft_thread_t thread = ft_thread_create(sched, &consommation, NULL, (void*) id_m);
    return thread;
}

ft_thread_t create_messager(struct Sched sched) {
    ft_thread_t thread = ft_thread_create(NULL, &messager, NULL, (void*) sched);
    return thread;
}


int main(int argc, char *argv[]) {

    create_tapis(&tapis_producteur, CAPACITY);
    create_tapis(&tapis_consommateur, CAPACITY);
    create_journal(&journal_producteur);
    create_journal(&journal_consommateur);
    create_journal(&journal_trajet);
    create_sched(&sched, tapis_producteur->sched, tapis_consommateur->sched);

    init_counter(&counter, NUM_PROD * PROD_TARGET);


    ft_thread_t threads[NUM_PROD + NUM_CONS];
    for(int i = 0; i<NUM_PROD; i++) {
        char* name = malloc(128);
        sprintf(name, "P%i", i);
        threads[i] = create_producteur(name);
    }

    for(int i = 0; i<NUM_MSG; i++) {
            threads[NUM_PROD + i] = create_messager(sched);
        }

    for(int i = 0; i<NUM_CONS; i++) {
        threads[NUM_PROD + NUM_MSG + i] = create_consommateur(i);
    }

    ft_scheduler_start(tapis_producteur->sched);
    ft_scheduler_start(tapis_consommateur->sched);


    for(int i = 0; i<NUM_PROD + NUM_CONS + NUM_MSG; i++) {
        ft_thread_join(threads[i], NULL);
    }

    return 0;
}
