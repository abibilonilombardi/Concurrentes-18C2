#include <unistd.h>
#include <iostream>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <time.h>

#include "MemoriaCompartida.h"
#include "Ship.h"
#include "Harbour.h"

using namespace std;

#ifndef PROCGEN_H
#define PROCGEN_H

class ProcessGenerator {
    int processQty;
    int harbourQty;
    //int shipQty;
    //pid_t *ships;
    MemoriaCompartida<int> map;
public:
    ProcessGenerator();
    ~ProcessGenerator();
    void spawnShips(int qty, int capacity);
    void spawnHarbours();
};

#endif //PROCGEN_H
