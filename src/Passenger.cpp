#include "Passenger.h"

Passenger::Passenger(SharedMemoryPassenger &sharedMem):
Process(),
sharedMem(sharedMem)
{
}

tuple<string,char> Passenger::getSemaphore(int passengerId){
    return make_tuple("passenger"+to_string(passengerId)+".bin",'p');
}


Passenger::~Passenger(){
    string logMessage = string("PASSENGER: ") + to_string(this->id) + string(" MUERE id") + to_string(this->semTravel->getId());
    Logger::getInstance().log(logMessage);

    this->sharedMem.freePassengerId(this->id);
    this->semTravel->remove();
    delete this->semTravel;
}
