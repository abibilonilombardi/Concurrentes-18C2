#include <stdlib.h>
#include <string.h>
#include "Process.h"
#include "Semaphore/Semaphore.h"
#include "SharedMemoryPassenger.h"

using namespace std;

#ifndef PASSENGER_H
#define PASSENGER_H

class Passenger : public Process{
protected:
    int id;
    int originHarbour;
    int destinationHarbour;
    Semaphore *semTravel;
    bool hasTicket;
    SharedMemoryPassenger &sharedMem;

public:
    Passenger(int id, SharedMemoryPassenger &sharedMem, int maxHarbours);
    virtual void travel() = 0;
    ~Passenger();
};

#endif //PASSENGER_H
