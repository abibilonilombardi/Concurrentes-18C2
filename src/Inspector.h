#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "Process.h"
#include "Ship.h"
#include "SharedMemoryShip.h"

using namespace std;

#ifndef INSPECTOR_H
#define INSPECTOR_H

class Inspector: public Process{
protected:
    int id;

public:
    Inspector();
    virtual void inspect(SharedMemoryShip &sharedMemoryShip, SharedMemoryPassenger &sharedMemoryPassenger) = 0;
    void behave(int maxHarbours);
    ~Inspector();
};

#endif //INSPECTOR_H
