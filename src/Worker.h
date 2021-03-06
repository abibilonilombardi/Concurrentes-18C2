#ifndef WORKER_H
#define WORKER_H

#include "Passenger.h"
#include "Fifos/FifoEscritura.h"
#include "Harbour.h"
#include "Logger/Logger.h"

#include <string.h>
#include <iostream>


class Worker : public Passenger {
public:
	Worker(SharedMemoryPassenger &sharedMem, int maxHarbours);
    void travel();
};
#endif //WORKER_H
