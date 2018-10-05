#include "Passenger.h"

Passenger::Passenger(int id, int originHarbour, int destinationHarbour):
Process(),
id(id),
originHarbour(originHarbour),
destinationHarbour(destinationHarbour){
    srand(1);//TODO:srand(time(NULL))
    this->hasTicket = rand() % 2;
    this->travel = Semaphore(string("/tmp/pass")+string(id));
    //Can't travel unless you've boarded a ship:
    this->travel.initialize(0);
    //TODO: write to shm passenger data.
}

Passenger::~Passenger(){
    //Free id?
}
