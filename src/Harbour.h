#include <iostream>
#include <unistd.h>
#include <sstream>
#include <iostream>
#include <string.h>

#include "Fifos/FifoLectura.h"

using namespace std;

#ifndef HARBOUR_H
#define HARBOUR_H

#include "Process.h"

class Harbour{
    int id;
    int fd;
    int distanceNext;
    FifoLectura *entrance;
public:
    Harbour(int id);
    int distanceNextHarbour();
    static string entranceName(int harbour_id);
    static string entranceLockName(int harbour_id);
    ~Harbour();
};

#endif //HARBOUR_H
