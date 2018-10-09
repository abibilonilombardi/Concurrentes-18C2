using namespace std;

#ifndef TICKET_INSPECTOR_H
#define TICKET_INSPECTOR_H

class TicketInspector: public Inspector{
public:
    TicketInspector();
    void inspect(SharedMemoryShip &sharedMemoryShip, SharedMemoryPassenger &sharedMemoryPassenger);
    ~TicketInspector();
};

#endif //TICKET_INSPECTOR_H
