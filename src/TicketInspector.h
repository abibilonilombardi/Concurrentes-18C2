#ifndef TICKET_INSPECTOR_H
#define TICKET_INSPECTOR_H

#include "Inspector.h"

using namespace std;


class TicketInspector: public Inspector{
public:
    TicketInspector();
    void inspect(int harbourToInspect, SharedMemoryShip &sharedMemoryShip, SharedMemoryPassenger &sharedMemoryPassenger);
    ~TicketInspector();
};

#endif //TICKET_INSPECTOR_H
