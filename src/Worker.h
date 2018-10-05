#include "Passenger.h"
#include <string.h>

#ifndef WORKER_H
#define WORKER_H

class Worker : public Passenger {
public:
    void travel();
};
#endif //WORKER_H
