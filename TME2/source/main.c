#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "fthread.h"

#define PROD_TARGET 10
#define NUM_PROD 1
#define NUM_CONS 1
#define NUM_MSG 1
#define CAPACITY 10

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
	ft_event_t put_event;
	ft_event_t take_event;
};

struct Journal {
	size_t size;
	struct Packet** packets;
};

static struct Counter counter;
static struct Tapis tapis_producteur;
static struct Tapis tapis_consommateur;
static struct Journal journal_producteur;
static struct Journal journal_consommateur;
static struct Journal journal_trajet;

void check_err(int err) {
	if(err != 0) printf("Error : %i\n", err);
}

struct Tapis* create_tapis(struct Tapis* tapis, size_t capacity) {

    tapis->first = 0;
    tapis->last = 0;
    tapis->size = 0;
    tapis->capacity = capacity;
    tapis->packets = malloc(sizeof(struct Packet*) * capacity);
    tapis->sched = ft_scheduler_create();
    tapis->put_event = ft_event_create(tapis->sched);
    tapis->take_event = ft_event_create(tapis->sched);

    return tapis;

}

void put_tapis(struct Tapis* tapis, struct Packet* packet) {

    while(tapis->size == tapis->capacity) {
    	check_err(ft_thread_await(tapis->take_event));
    }

    tapis->packets[tapis->last] = packet;
	tapis->last = (tapis->last + 1) % tapis->capacity;
	tapis->size++;

	check_err(ft_thread_generate(tapis->put_event));

}

struct Packet* take_tapis(struct Tapis* tapis) {

    while(tapis->size == 0) {
    	check_err(ft_thread_await(tapis->put_event));
    }

    struct Packet* ret = tapis->packets[tapis->first];
	tapis->first = (tapis->first + 1) % tapis->capacity;
	tapis->size--;

	check_err(ft_thread_generate(tapis->take_event));

	return ret;
}


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
        check_err(ft_thread_cooperate());
    }
    free(name);
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
        check_err(ft_thread_cooperate());
    }

    printf("cons end");
}

void messager(void* args) {

    while(counter.remaining > 0) {
        struct Packet* packet = take_tapis(&tapis_producteur);
        if(packet!=NULL){
        	put_messager(&journal_trajet, packet);
        	check_err(ft_thread_unlink());
        	check_err(ft_thread_link(tapis_consommateur.sched));
			put_tapis(&tapis_consommateur, packet);
			check_err(ft_thread_unlink());
			free(packet);
        	check_err(ft_thread_link(tapis_producteur.sched));
        }
    }
}

ft_thread_t create_producteur(const char* name, ft_scheduler_t sched) {
    ft_thread_t thread = ft_thread_create(sched, production, NULL, (void*) name);

    if(thread == NULL) printf("NULLPROD");
    return thread;
}

ft_thread_t create_consommateur(int id, ft_scheduler_t sched) {
    int* id_m = malloc(sizeof(int));
    *id_m = id;
    ft_thread_t thread = ft_thread_create(sched, consommation, NULL, (void*) id_m);
    if(thread == NULL) printf("NULLCONS");
    return thread;
}

ft_thread_t create_messager(ft_scheduler_t sched) {
    ft_thread_t thread = ft_thread_create(sched, messager, NULL, NULL);
    if(thread == NULL) printf("NULLMESS");
    return thread;
}

int main(int argc, char *argv[]) {

    create_tapis(&tapis_producteur, CAPACITY);
    create_tapis(&tapis_consommateur, CAPACITY);
    create_journal(&journal_producteur);
    create_journal(&journal_consommateur);
    create_journal(&journal_trajet);

    init_counter(&counter, NUM_PROD * PROD_TARGET);

    ft_thread_t threads[NUM_PROD + NUM_CONS + NUM_MSG];
    for(int i = 0; i<NUM_PROD + NUM_CONS + NUM_MSG; i++) {
		threads[i] = NULL;
	}

    for(int i = 0; i<NUM_PROD; i++) {
        char* name = malloc(128);
        sprintf(name, "P%i", i);
        threads[i] = create_producteur(name, tapis_producteur.sched);
    }

    for(int i = 0; i<NUM_MSG; i++) {
		threads[NUM_PROD + i] = create_messager(tapis_producteur.sched);
	}

    for(int i = 0; i<NUM_CONS; i++) {
        threads[NUM_PROD + NUM_MSG + i] = create_consommateur(i,tapis_consommateur.sched);
    }


    check_err(ft_scheduler_start(tapis_producteur.sched));
    check_err(ft_scheduler_start(tapis_consommateur.sched));

    printf("ft_thread_join renvoie une erreur EBADCREATE, et n'attends pas\n");

    for(int i = 0; i<NUM_PROD + NUM_CONS + NUM_MSG; i++) {

    	check_err(ft_thread_join(threads[i]));
    }

    return 0;
}
