#include "Process.h"

Process::Process(){
    // event handler para la senial SIGINT (-2)
    SignalHandler::getInstance()->registrarHandler(SIGINT, &this->sigint_handler);
}

bool Process::running(){
    return !this->sigint_handler.getGracefulQuit();
}

Process::~Process(){
    SignalHandler::destruir();
}
