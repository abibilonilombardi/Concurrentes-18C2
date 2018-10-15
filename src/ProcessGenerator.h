#ifndef PROCGEN_H
#define PROCGEN_H
    
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

#include "SharedMemoryPassenger.h"
#include "Logger/Logger.h"
#include "Semaphore/Semaphore.h"
#include "Ship.h"
#include "Process.h"
#include "Harbour.h"
#include "Worker.h"
#include "Tourist.h"
#include "ShipInspector.h"
#include "TicketInspector.h"

using namespace std;


class ProcessGenerator : public Process{
    size_t harbourQty;
    vector<pid_t> processes;

    vector<Harbour*> harbours;
    SharedMemoryPassenger* passengersMem;

public:
    ProcessGenerator();
    ~ProcessGenerator();
    pid_t spawnShips(int qty, int capacity);
    pid_t spawnHarbours();
    pid_t spawnPassenger();
    pid_t spawnShipInspector();
    pid_t spawnTicketInspector();
    int beginSimulation();

};

#endif //PROCGEN_H
