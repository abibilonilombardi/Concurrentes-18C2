#include "Passenger.h"
#include <string.h>
#include <iostream>

#ifndef WORKER_H
#define WORKER_H

class Worker : public Passenger {
	int maxPassengers;
public:
	Worker(SharedMemoryPassenger &sharedMem, int maxHarbours);
    void travel();
};
#endif //WORKER_H
