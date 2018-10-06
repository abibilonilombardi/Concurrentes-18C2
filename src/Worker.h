#include "Passenger.h"
#include <string.h>
#include <iostream>

#ifndef WORKER_H
#define WORKER_H

class Worker : public Passenger {
public:
	Worker(int id, SharedMemoryPassenger &sharedMem, int maxHarbours);
    void travel();
    ~Worker();
};
#endif //WORKER_H
