#include <stdlib.h>
#include "Process.h"

#ifndef PASSENGER_H
#define PASSENGER_H

class Passenger : public Process{
    int id;
    bool hasTicket;
    int originHarbour;
    int destinationHarbour;
public:
    Passenger(int id, int originHarbour, int destinationHarbour);
};

#endif //PASSENGER_H
