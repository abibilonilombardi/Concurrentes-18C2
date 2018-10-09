#include "Passenger.h"

Passenger::Passenger(SharedMemoryPassenger &sharedMem):
Process(),
sharedMem(sharedMem)
{
    //string path = string("/tmp/pass")+to_string(id);

    //Can't travel unless you've boarded a ship:
    //this->semTravel->initialize(0);
    //Write passenger data to shared memory:
    this->id = this->sharedMem.addPassenger(this->locationStart, this->locationEnd, this->hasTicket);
    tuple s = Passenger::getSemaphore(this->id);
    //initialize passenger semaphore at 0:
    //this->semTravel = new Semaphore(s.get<0>(s), s.get<1>(s),0);
}

tuple<string,char> Passenger::getSemaphore(int passengerId){
    return make_tuple("passenger"+string(passengerId)+".bin",'p')
}


Passenger::~Passenger(){
    cout << "Passenger with id " << this->id << " freed!\n";
    //Free id:
    this->sharedMem.freePassengerId(this->id);
    delete this->semTravel;
}
