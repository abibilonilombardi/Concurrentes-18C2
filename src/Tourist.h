#include "Passenger.h"
#include "Logger/Logger.h"
#include "Harbour.h"
#include "Fifos/FifoEscritura.h"

#include <queue>

using namespace std;

#ifndef TOURIST_H
#define TOURIST_H

class Tourist : public Passenger {
private:
    queue<int> destinations;
public:
    Tourist(SharedMemoryPassenger &sharedMem, int maxHarbours);
    void travel();
};

#endif //TOURIST_H
