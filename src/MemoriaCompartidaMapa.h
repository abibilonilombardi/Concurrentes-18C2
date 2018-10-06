#include "MemoriaCompartida.h"
#ifndef MEMCOMPMAPA_H
#define MEMCOMPMAPA_H

class MemoriaCompartidaMapa: public MemoriaCompartida<int> {
public:
    MemoriaCompartidaMapa(int harbourQty);
    int totalHarbours();
    int distanceNextHarbour(int currentHb);
};

#endif //MEMCOMPMAPA_H
