#include "MemoriaCompartida.h"
//#include "Lock/Lock.h"
#include <string.h>
#include <vector>

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
    std::vector<int> getPassengers(); //no los elimina
    void updatePassengers(std::vector<int> &passengerIds);
    bool removePassenger(int passengerId);
    bool addPassenger(int passengerId);
    static void setShipCapacity(int shipCty);
    ~SharedMemoryShip();
};

#endif //SHMSHIP_H
