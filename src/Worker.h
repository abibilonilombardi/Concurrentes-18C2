#include "Passenger.h"
#include <string.h>
#include <iostream>

#ifndef WORKER_H
#define WORKER_H

class Worker : public Passenger {
	int maxPassengers;
public:
	Worker(size_t id, SharedMemoryPassenger &sharedMem, int maxPassengers, int maxHarbours);
    void travel();
};
#endif //WORKER_H
