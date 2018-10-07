#include "Passenger.h"

Passenger::Passenger(SharedMemoryPassenger &sharedMem):
Process(),
sharedMem(sharedMem)
{
    //string path = string("/tmp/pass")+to_string(id);
    //this->semTravel = new Semaphore(path, 'p');
    //Can't travel unless you've boarded a ship:
    //this->semTravel->initialize(0);
    //Write passenger data to shared memory:
    this->id = this->sharedMem.addPassenger(this->locationStart, this->locationEnd, this->hasTicket);
}


Passenger::~Passenger(){
    //Free id?
    //delete this->semTravel;
    cout << "Passenger with id " << this->id << " freed!\n";
    this->sharedMem.freePassengerId(this->id);
}
