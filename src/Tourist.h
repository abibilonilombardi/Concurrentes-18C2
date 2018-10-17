#ifndef TOURIST_H
#define TOURIST_H

#include "Passenger.h"
#include "Logger/Logger.h"
#include "Harbour.h"
#include "Fifos/FifoEscritura.h"

#include <vector>

using namespace std;


class Tourist : public Passenger {
private:
    vector<int> destinations;
    int qtyHarbours;
public:
    Tourist(SharedMemoryPassenger &sharedMem, int maxHarbours);
    void travel();
};

#endif //TOURIST_H
