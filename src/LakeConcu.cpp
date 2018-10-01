#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <iostream>

#include "ProcessGenerator.h"

using namespace std;


int main(int argc, char *argv[]){
    int shipQty = atoi(argv[1]);
    int shipCty = atoi(argv[2]);

    ProcessGenerator pc;
    //spawn harbour processes
    pc.spawnHarbours();
    //spawn ship processes
    pc.spawnShips(shipQty, shipCty);
    //begin simulation (spawn people processes)
    pc.beginSimulation();
    return 0;
}
