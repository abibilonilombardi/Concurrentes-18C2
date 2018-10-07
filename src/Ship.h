#include <iostream>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#include "Harbour.h"
#include "SharedMemoryMap.h"
#include "SharedMemoryShip.h"
#include "Process.h"

#ifndef SHIP_H
#define SHIP_H

class Ship : public Process{
    int id;
    SharedMemoryMap &map;
    int harbour, capacity;
    int totalHarbours;
    int fdShip;
    SharedMemoryShip *shmship;

public:
    Ship(int id, SharedMemoryMap &map, int harbour,int capacity);
    void sail();
    static string getShmName(int shipId);
    ~Ship();
};

#endif //SHIP_H
