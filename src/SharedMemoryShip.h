#include "MemoriaCompartida.h"
//#include "Lock/Lock.h"
#include <string.h>

using namespace std;

#ifndef SHMSHIP_H
#define SHMSHIP_H

class SharedMemoryShip: public MemoriaCompartida<int> {
    static size_t shipCty;
public:
    SharedMemoryShip(const string &pathname, bool authorized);
    SharedMemoryShip(const string &pathname);
    void confiscateShip();
    bool authorizedToSail();
    bool confiscated();
    bool removePassenger(int passengerId);
    bool addPassenger(int passengerId);
    static void setShipCapacity(int shipCty);
};

#endif //SHMSHIP_H
