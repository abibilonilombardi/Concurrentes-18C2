#ifndef SHMMAP_H
#define SHMMAP_H

#include "MemoriaCompartida.h"

class SharedMemoryMap: protected MemoriaCompartida<int> {
private:
    int harbourQty;
public:
    SharedMemoryMap(int harbourQty);
    void initialize();
    void addHarbourPid(pid_t harbourPid, int harbourId);
    int getHarbourPid(int harbourId);
    int totalHarbours();
    int distanceNextHarbour(int currentHb);
};

#endif //SHMMAP_H
