#include "Passenger.h"

Passenger::Passenger(int id, SharedMemoryPassenger &sharedMem):
Process(),
id(id),
sharedMem(sharedMem)
{
    //string path = string("/tmp/pass")+to_string(id);
    //this->semTravel = new Semaphore(path, 'p');
    //Can't travel unless you've boarded a ship:
    //this->semTravel->initialize(0);
    //TODO: write to shm passenger data.
}

Passenger::~Passenger(){
    //Free id?
    //delete this->semTravel;
}
