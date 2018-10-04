#include "Semaphore.h"

#ifndef BINARY_SEMAPHORE
#define BINARY_SEMAPHORE

class BinarySemaphore : public Semaphore {
public:
    BinarySemaphore(const string &pathname, const char letter):
    Semaphore(pathname, letter){
        this->initialize(1);
    }
};

#endif //BINARY_SEMAPHORE
