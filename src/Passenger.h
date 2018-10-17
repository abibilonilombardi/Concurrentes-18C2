#include <stdlib.h>
#include <string.h>
#include <tuple>

#include "Process.h"
#include "Semaphore/Semaphore.h"
#include "SharedMemoryPassenger.h"
#include "Handlers/SIGPIPE_Handler.h"


using namespace std;

#ifndef PASSENGER_H
#define PASSENGER_H

class Passenger : public Process{
protected:
    int id;
    SharedMemoryPassenger &sharedMem;
    SIGPIPE_Handler sigpipe_handler;
    Semaphore *semTravel;
    int locationStart;
	int locationEnd;
	bool hasTicket;
public:
    Passenger(SharedMemoryPassenger &sharedMem);
    static tuple<string,char> getSemaphore(int passengerId);
    bool failedBoard();
    virtual void travel() = 0;
    ~Passenger();
};

#endif //PASSENGER_H
