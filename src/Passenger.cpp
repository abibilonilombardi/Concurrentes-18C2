#include "Passenger.h"

Passenger::Passenger(SharedMemoryPassenger &sharedMem):
Process(),
sharedMem(sharedMem)
{
    //Write passenger data to shared memory:
    this->id = this->sharedMem.addPassenger(this->locationStart, this->locationEnd, this->hasTicket);

    //Can't travel unless you've boarded a ship:
    //Initialize passenger semaphore at 0:
    tuple<string,char> s = Passenger::getSemaphore(this->id);
    this->semTravel = new Semaphore(0, get<0>(s), get<1>(s));
}

tuple<string,char> Passenger::getSemaphore(int passengerId){
    return make_tuple("passenger"+to_string(passengerId)+".bin",'p');
}


Passenger::~Passenger(){
    cout << "PASSENGER " << this->id << " FREED!\n";
    //Free id:
    this->sharedMem.freePassengerId(this->id);
    this->semTravel->remove();
    delete this->semTravel;
}
