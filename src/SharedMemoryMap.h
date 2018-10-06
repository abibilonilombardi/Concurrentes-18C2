#include "MemoriaCompartida.h"
#ifndef SHMMAP_H
#define SHMMAP_H

class SharedMemoryMap: public MemoriaCompartida<int> {
public:
    SharedMemoryMap(int harbourQty);
    int totalHarbours();
    int distanceNextHarbour(int currentHb);
};

#endif //SHMMAP_H
