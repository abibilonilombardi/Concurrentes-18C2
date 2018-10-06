#include "MemoriaCompartidaMapa.h"
#define MAP "/bin/bash"

#define MAX_DST_HARBOURS 10 //max distance between harbours.

MemoriaCompartidaMapa::MemoriaCompartidaMapa(int harbourQty):
MemoriaCompartida(){
    int sz = harbourQty+1;
    this->crear(MAP, 'm', sz);
    this->escribir(harbourQty, 0);
    for(int i=1; i <= harbourQty; i++){
        //Write distance to next harbour(map) in shmem and save it
        //in a vector in case I need it later:
        int distanceNextHarbour = (rand() % MAX_DST_HARBOURS)+1;
        this->escribir(distanceNextHarbour, i);
    }
}

int MemoriaCompartidaMapa::totalHarbours(){
    return this->leer(0);
}

int MemoriaCompartidaMapa::distanceNextHarbour(int currentHb){
    return this->leer(currentHb+1);
}
