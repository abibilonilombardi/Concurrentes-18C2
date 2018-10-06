#include "SharedMemoryShip.h"

int SharedMemoryShip::shipCty = 0;

void SharedMemoryShip::setShipCapacity(const int shipCty){
    //TODO:AGREGAR LOCK ACA POR LAS DUDAS
    SharedMemoryShip::shipCty = shipCty;
}

SharedMemoryShip::SharedMemoryShip(const string &pathname){
    int sz = SharedMemoryShip::shipCty;
    //TODO:AGREGAR LOCK ACA POR LAS DUDAS
    //attach to shm, or create and attach if it doesn't exist.
    this->crear(pathname, 's', sz);
}
