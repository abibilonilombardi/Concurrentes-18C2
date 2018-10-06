#include <iostream>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#include "Harbour.h"
#include "SharedMemoryMap.h"
#include "Process.h"

#ifndef SHIP_H
#define SHIP_H

class Ship : public Process{
    SharedMemoryMap &map;
    int harbour, capacity;
    int totalHarbours;

public:
    Ship(SharedMemoryMap &map, int harbour,int capacity);
    void sail();
    ~Ship();
};

#endif //SHIP_H
