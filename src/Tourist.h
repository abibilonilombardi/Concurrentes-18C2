#include "Passenger.h"
#include <vector>

using namespace std;

#ifndef TOURIST_H
#define TOURIST_H

class Tourist : public Passenger {
private:
    std::vector<int> destinations;
public:
    void travel();
};

#endif //TOURIST_H
