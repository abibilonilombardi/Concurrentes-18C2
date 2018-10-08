#include <iostream>
#include <unistd.h>
#include <sstream>
#include <iostream>
#include <string.h>


#include "Fifos/FifoEscritura.h"
#include "Fifos/FifoLectura.h"

using namespace std;

#ifndef HARBOUR_H
#define HARBOUR_H

#include "Process.h"
#include "SharedMemoryMap.h"

class Harbour : public Process{
    int id;
    FifoEscritura *exit;
    FifoLectura *entrance;
public:
    Harbour(int id);
    void openHarbour();
    static string exitName(int harbour_id);
    static string entranceName(int harbour_id);
    ~Harbour();
};

#endif //HARBOUR_H
