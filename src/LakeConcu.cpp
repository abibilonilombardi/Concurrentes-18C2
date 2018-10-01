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
    pc.spawnHarbours();
    pc.spawnShips(shipQty, shipCty);

    return 0;
}
