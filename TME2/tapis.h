#ifndef TAPIS_H
#define TAPIS_H

#include <stdlib.h>
#include <stdio.h>
#include "fthread.h"

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

struct Tapis* create_tapis(struct Tapis* tapis, size_t capacity);

void put_tapis(struct Tapis* tapis, struct Packet* packet);

struct Packet* take_tapis(struct Tapis* tapis);


#endif
