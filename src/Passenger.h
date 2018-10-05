#include <stdlib.h>
#include <string.h>
#include "Process.h"
#include "Semaphore/Semaphore.h"

using namespace std;

#ifndef PASSENGER_H
#define PASSENGER_H

class Passenger : public Process{
    int id;
    bool hasTicket;
    int originHarbour;
    int destinationHarbour;
    Semaphore travel;
public:
    Passenger(int id, int originHarbour, int destinationHarbour);
    virtual void travel() = 0;
    ~Passenger();
};

#endif //PASSENGER_H
