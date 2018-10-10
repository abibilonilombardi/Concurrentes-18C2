#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <vector>
#include "Process.h"
#include "Ship.h"
#include "SharedMemoryShip.h"
#include "SharedMemoryPassenger.h"
#include "Harbour.h"

using namespace std;

#ifndef INSPECTOR_H
#define INSPECTOR_H

class Inspector: public Process{
public:
    Inspector();
    virtual void inspect(int harbourToInspect, SharedMemoryShip &sharedMemoryShip, SharedMemoryPassenger &sharedMemoryPassenger) = 0;
    void behave(int maxHarbours);
    ~Inspector();
};

#endif //INSPECTOR_H
