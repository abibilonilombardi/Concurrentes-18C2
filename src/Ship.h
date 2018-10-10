#ifndef SHIP_H
#define SHIP_H

#include <iostream>
#include <unistd.h>

#include <errno.h>
#include <string.h>
#include <vector>

#include "Harbour.h"
#include "SharedMemoryShip.h"
#include "SharedMemoryPassenger.h"
#include "Process.h"
#include "Lock/ExclusiveLock.h"
#include "Handlers/SIGALRM_Handler.h"

class Ship : public Process{
    int id;
    vector<Harbour*> &map;
    int harbour, capacity; // actual harbour??  
    int fdShip;
    SharedMemoryShip *shmship;
    SharedMemoryPassenger& shmPassenger;
    SIGALRM_Handler sigint_handler;

public:
    Ship(int id, vector<Harbour*> &map, size_t harbour,int capacity, SharedMemoryPassenger& shmPassenger);
    void sail();
    static string getShmName(int shipId); //private?
    ~Ship();
    
private:
    void freeResources();
    void initialize();
    void arrivalAnnouncement();
    void departureAnnouncement();
    void writeInHarbourFile(int fd,int value);
    void loadPeople();
    void unloadPeople();

    void blockSigAlarm();
    void unblockSigAlarm();
    void setDepartureAlarm();
    // void setAlarmAction();
    // void rangAlamr(int signum);

};

#endif //SHIP_H
