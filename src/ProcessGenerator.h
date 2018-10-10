#include <unistd.h>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <time.h>
#include <set>
#include <errno.h>
#include <vector>
//#include <error.h>

#include "SharedMemoryPassenger.h"
#include "Logger/Logger.h"
#include "Semaphore/Semaphore.h"
#include "Ship.h"
#include "Process.h"
#include "Harbour.h"
#include "Worker.h"
#include "ShipInspector.h"

using namespace std;

#ifndef PROCGEN_H
#define PROCGEN_H

class ProcessGenerator : public Process{
    size_t harbourQty;
    set<pid_t> processes;
    vector<Harbour*> harbours;
    SharedMemoryPassenger* passengersMem;

public:
    ProcessGenerator();
    ~ProcessGenerator();
    pid_t spawnShips(int qty, int capacity);
    pid_t spawnHarbours();
    pid_t spawnPassenger();
    int beginSimulation();

};

#endif //PROCGEN_H
