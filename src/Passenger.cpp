#include "Passenger.h"

Passenger::Passenger(int id, int originHarbour, int destinationHarbour):
Process(),
id(id),
originHarbour(originHarbour),
destinationHarbour(destinationHarbour){
    srand(1);//TODO:srand(time(NULL))
    this->hasTicket = rand() % 2;
    string path = string("/tmp/pass")+to_string(id);
    this->semTravel = new Semaphore(path, 'p');
    //Can't travel unless you've boarded a ship:
    this->semTravel->initialize(0);
    //TODO: write to shm passenger data.
}

Passenger::~Passenger(){
    //Free id?
    delete this->semTravel;
}
