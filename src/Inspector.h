#include <stdlib.h>
#include <string.h>
#include "Process.h"
#include "Semaphore/Semaphore.h"
#include "SharedMemoryPassenger.h"

using namespace std;

#ifndef INSPECTOR_H
#define INSPECTOR_H

class Inspector: public Process{
protected:
    int id;
    SharedMemoryPassenger &sharedMem;
    Semaphore *semTravel;
    //bool hasTicket;
    //int originHarbour;
    //int destinationHarbour;
public:
    Inspector(SharedMemoryPassenger &sharedMem);
    void inspect();
    void behave(int maxHarbours);
    ~Inspector();
};

#endif //INSPECTOR_H
