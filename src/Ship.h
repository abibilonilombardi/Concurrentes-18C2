#include <iostream>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <vector>

#include "Harbour.h"
#include "SharedMemoryShip.h"
#include "Process.h"
#include "Lock/ExclusiveLock.h"


#ifndef SHIP_H
#define SHIP_H

class Ship : public Process{
    int id;
    vector<Harbour*> &map;
    int harbour, capacity; // actual harbour??  
    int fdShip;
    SharedMemoryShip *shmship;

public:
    Ship(int id, vector<Harbour*> &map, size_t harbour,int capacity);
    void sail();
    static string getShmName(int shipId); //private?
    ~Ship();
};

#endif //SHIP_H
