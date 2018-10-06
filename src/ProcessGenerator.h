#include <unistd.h>
#include <iostream>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <time.h>
#include <vector>

#include "SharedMemoryPassenger.h"
#include "SharedMemoryMap.h"
#include "Ship.h"
#include "Process.h"
#include "Harbour.h"
#include "Worker.h"

using namespace std;

#ifndef PROCGEN_H
#define PROCGEN_H

class ProcessGenerator : public Process{
    int harbourQty;
    vector<pid_t> processes;

    void initializeMap(MemoriaCompartida<int> &m);

public:
    ProcessGenerator();
    ~ProcessGenerator();
    pid_t spawnShips(int qty, int capacity);
    pid_t spawnHarbours();
    pid_t spawnPassengers();
    int beginSimulation();

};

#endif //PROCGEN_H
