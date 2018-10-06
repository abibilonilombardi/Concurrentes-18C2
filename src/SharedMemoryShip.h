#include "MemoriaCompartida.h"
//#include "Lock/Lock.h"
#include <string.h>

using namespace std;

#ifndef SHMSHIP_H
#define SHMSHIP_H

class SharedMemoryShip: protected MemoriaCompartida<int> {
    static int shipCty;
public:
    SharedMemoryShip(const string &pathname);
    //bool authorizedToSail(int shipId);
    //void decommissionShip(int shipId);
    //int removePassenger(int shipId, int passengerId);
    //int addPassenger(int shipId, int passengerId);
    static void setShipCapacity(int shipCty);
};

#endif //SHMSHIP_H
