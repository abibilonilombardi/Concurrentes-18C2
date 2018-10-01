#include "Handlers/SignalHandler.h"
#include "Handlers/SIGINT_Handler.h"

#ifndef PROCESS_H
#define PROCESS_H

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
