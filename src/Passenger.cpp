#include "Passenger.h"

Passenger::Passenger(SharedMemoryPassenger &sharedMem):
Process(),
sharedMem(sharedMem)
{
    SignalHandler::getInstance()->registrarHandler(SIGPIPE, &this->sigpipe_handler);
}

tuple<string,char> Passenger::getSemaphore(int passengerId){
    return make_tuple("passenger"+to_string(passengerId)+".bin",'p');
}

bool Passenger::failedBoard(){
    return this->sigpipe_handler.getFailedBoard();
}

Passenger::~Passenger(){
    this->sharedMem.freePassengerId(this->id);
    // this->semTravel->remove();
    delete this->semTravel;
    Logger::getInstance().log( string("PASSENGER-") + to_string(this->id) + string(" DELETED"),'d');
}
