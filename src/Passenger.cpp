#include "Passenger.h"

Passenger::Passenger(int id, int originHarbour, int destinationHarbour):
Process(),
id(id),
originHarbour(originHarbour),
destinationHarbour(destinationHarbour){
    srand(1);//TODO:srand(time(NULL))
    this->hasTicket = rand() % 2;
}
