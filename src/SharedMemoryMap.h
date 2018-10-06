#include "MemoriaCompartida.h"
#ifndef SHMMAP_H
#define SHMMAP_H

class SharedMemoryMap: protected MemoriaCompartida<int> {
private:
    int harbourQty;
public:
    SharedMemoryMap(int harbourQty);
    void initialize();
    int totalHarbours();
    int distanceNextHarbour(int currentHb);
};

#endif //SHMMAP_H
