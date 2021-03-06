#ifndef INSPECTOR_H
#define INSPECTOR_H

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <vector>
#include "Process.h"
#include "Ship.h"
#include "SharedMemoryShip.h"
#include "SharedMemoryPassenger.h"
#include "Harbour.h"
#include "Passenger.h"
#include "Semaphore/Semaphore.h"

using namespace std;


class Inspector: public Process{
public:
    Inspector();
    virtual void inspect(int harbourToInspect, SharedMemoryShip &sharedMemoryShip, SharedMemoryPassenger &sharedMemoryPassenger) = 0;
    void behave(int maxHarbours, int maxPassengers);
    // void unloadShipPassenger(int passengerId, int harbourToInspect, SharedMemoryPassenger &sharedMemoryPassenger);
    ~Inspector();
};

#endif //INSPECTOR_H
