#include "SharedMemoryMap.h"
#define MAP "/bin/bash"

#define MAX_DST_HARBOURS 10 //max distance between harbours.

SharedMemoryMap::SharedMemoryMap(int harbourQty):
MemoriaCompartida(),
harbourQty(harbourQty){
    int sz = harbourQty+1;
    this->crear(MAP, 'm', sz);
}

void SharedMemoryMap::initialize(){
    this->escribir(this->harbourQty, 0);
    for(int i=1; i <= this->harbourQty; i++){
        //Write distance to next harbour(map) in shmem and save it
        //in a vector in case I need it later:
        int distanceNextHarbour = (rand() % MAX_DST_HARBOURS)+1;
        this->escribir(distanceNextHarbour, i);
    }
}

int SharedMemoryMap::totalHarbours(){
    return this->leer(0);
}

int SharedMemoryMap::distanceNextHarbour(int currentHb){
    return this->leer(currentHb+1);
}
