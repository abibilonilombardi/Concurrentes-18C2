#ifndef PROCESS_H
#define PROCESS_H

#include "Handlers/SignalHandler.h"
#include "Handlers/SIGINT_Handler.h"
#include <time.h>   

#define RANDOM(maxNumber) \
    (srand(time(NULL) + getpid()), \
     rand() % maxNumber)

class Process {
protected:
    SIGINT_Handler sigint_handler;
    SignalHandler *sh;

public:
    Process();
    bool running();
    ~Process();
};

#endif //PROCESS_H
