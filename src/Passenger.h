#include <stdlib.h>
#include <string.h>
#include "Process.h"
#include "SharedMemoryPassenger.h"

using namespace std;

#ifndef PASSENGER_H
#define PASSENGER_H

class Passenger : public Process{
protected:
    int id;
    SharedMemoryPassenger &sharedMem;
    //Semaphore *semTravel;
    int locationStart;
	int locationEnd;
	bool hasTicket;
public:
    Passenger(SharedMemoryPassenger &sharedMem);
    virtual void travel() = 0;
    ~Passenger();
};

#endif //PASSENGER_H
