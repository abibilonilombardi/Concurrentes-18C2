#include <iostream>
#include <unistd.h>
#include "Harbour.h"
#include "MemoriaCompartida.h"

#ifndef SHIP_H
#define SHIP_H

class Ship {
    MemoriaCompartida<int> &map;
    int capacity, harbour;

public:
    Ship(MemoriaCompartida<int> &map,int capacity, int harbour);
    void sail();
    ~Ship();
};

#endif //SHIP_H
