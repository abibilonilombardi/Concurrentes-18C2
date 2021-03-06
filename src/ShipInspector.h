#ifndef SHIP_INSPECTOR_H
#define SHIP_INSPECTOR_H

#include "Inspector.h"

using namespace std;

class ShipInspector: public Inspector{
public:
    ShipInspector();
    void inspect(int harbourToInspect, SharedMemoryShip &sharedMemoryShip, SharedMemoryPassenger &sharedMemoryPassenger);
    ~ShipInspector();
};

#endif //SHIP_INSPECTOR_H
