#include "SharedMemoryPassenger.h"
#define FIELDS 3
#define LOCATION_OFFSET 1
#define STOP_OFFSET 2
#define TICKET_OFFSET 3

SharedMemoryPassenger::SharedMemoryPassenger(const std::string pathname, int maxPassengers):
MemoriaCompartida(),
maxPassengers(maxPassengers),
pathname(pathname){
    this->fd = open(pathname.c_str(), O_RDWR|O_CREAT, S_IRGRP|S_IWGRP);
    this->crear(pathname, 'p', maxPassengers * FIELDS);
}

void SharedMemoryPassenger::addPassenger(int passengerId, int location, int nextStop, bool hasTicket){
    //lock
    size_t startingPos = getStartingPosition(passengerId);
    this->escribir(passengerId, startingPos);
    this->escribir(location, startingPos + 1);
    this->escribir(nextStop, startingPos + 2);
    this->escribir(hasTicket, startingPos + 3);
}

void SharedMemoryPassenger::updateLocation(int passengerId, int location){
    //lock
    size_t startingPos = getStartingPosition(passengerId);
    size_t pos = startingPos + LOCATION_OFFSET;
    this->escribir(location, pos);
}

int SharedMemoryPassenger::getLocation(int passengerId){
    //lock lectura
    size_t startingPos = getStartingPosition(passengerId);
    size_t pos = startingPos + LOCATION_OFFSET;
    return this->leer(pos);
}

void SharedMemoryPassenger::updateNextStop(int passengerId, int nextStop){
    //lock
    size_t startingPos = getStartingPosition(passengerId);
    size_t pos = startingPos + STOP_OFFSET;
    this->escribir(nextStop, pos);
}

int SharedMemoryPassenger::getNextStop(int passengerId){
    //lock lectura
    size_t startingPos = getStartingPosition(passengerId);
    size_t pos = startingPos + STOP_OFFSET;
    return this->leer(pos);
}

bool SharedMemoryPassenger::hasTicket(int passengerId){
    size_t startingPos = getStartingPosition(passengerId);
    size_t pos = startingPos + TICKET_OFFSET;
    return this->leer(pos);
}


size_t SharedMemoryPassenger::getStartingPosition(int passengerId){
    return passengerId % this->maxPassengers;//+ FIELDS * (passengerId % FIELDS);
}

SharedMemoryPassenger::~SharedMemoryPassenger(){
    close(this->fd);
    unlink(this->pathname.c_str());
}
