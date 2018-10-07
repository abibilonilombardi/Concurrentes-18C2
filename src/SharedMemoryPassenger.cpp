#include "SharedMemoryPassenger.h"
#define FIELDS 4
#define LOCATION_OFFSET 0
#define STOP_OFFSET 1
#define TICKET_OFFSET 2
#define FREE_ID 3

#define OCCUPIED 1
#define FREE 0

//TODO:chequear si estos dos tienen que ser distintos...

string SharedMemoryPassenger::shmFileName(){
    return string("passengers_data.bin");
}

string SharedMemoryPassenger::shmLockName(){
    return string("passengers.txt");
}

SharedMemoryPassenger::SharedMemoryPassenger(std::string pathname, int maxPassengers):
MemoriaCompartida(),
maxPassengers(maxPassengers),
pathname(pathname){
    this->fd = open(pathname.c_str(), O_RDWR|O_CREAT, S_IRGRP|S_IWGRP);
    this->crear(pathname, 'p', maxPassengers * FIELDS);
    LockFile l(SharedMemoryPassenger::shmLockName());
    l.tomarLock();
    for(size_t i=0; i<this->size(); i++){
        //initialize shm in -1;
        this->escribir(FREE, i);
    }
    l.liberarLock();
}

int SharedMemoryPassenger::addPassenger(int location, int nextStop, bool hasTicket){
    LockFile l(SharedMemoryPassenger::shmLockName());
    l.tomarLock();
    int passengerId = getFreeId();
    if (passengerId < 0){
        l.liberarLock();
        return passengerId;
    }
    size_t startingPos = getStartingPosition(passengerId);
    //this->escribir(passengerId, startingPos);
    this->escribir(location, startingPos + LOCATION_OFFSET);
    this->escribir(nextStop, startingPos + STOP_OFFSET);
    this->escribir(hasTicket, startingPos + TICKET_OFFSET);
    this->escribir(OCCUPIED, startingPos + FREE_ID);
    l.liberarLock();
    return passengerId;
}

void SharedMemoryPassenger::updateLocation(int passengerId, int location){
    size_t startingPos = getStartingPosition(passengerId);
    size_t pos = startingPos + LOCATION_OFFSET;
    LockFile l(SharedMemoryPassenger::shmLockName());
    l.tomarLock();
    this->escribir(location, pos);
    l.liberarLock();
}

int SharedMemoryPassenger::getFreeId(){
    for(size_t i=0; i<this->size(); i+=FIELDS){
        if(this->leer(i+FREE_ID)==FREE){
            return i / FIELDS;
        }
    }
    return -1;
}

void SharedMemoryPassenger::freePassengerId(int passengerId){
    //Mark this id position as free:
    LockFile l(SharedMemoryPassenger::shmLockName());
    l.tomarLock();
    this->escribir(FREE,getStartingPosition(passengerId)+FREE_ID);
    l.liberarLock();
}


int SharedMemoryPassenger::getLocation(int passengerId){
    size_t startingPos = getStartingPosition(passengerId);
    size_t pos = startingPos + LOCATION_OFFSET;
    //LockFile lectura
    int loc = 0;
    LockFile l(SharedMemoryPassenger::shmLockName());
    l.tomarLock();
    loc = this->leer(pos);
    l.liberarLock();
    return loc;
}

void SharedMemoryPassenger::updateNextStop(int passengerId, int nextStop){
    //lock
    size_t startingPos = getStartingPosition(passengerId);
    size_t pos = startingPos + STOP_OFFSET;
    LockFile l(SharedMemoryPassenger::shmLockName());
    l.tomarLock();
    this->escribir(nextStop, pos);
    l.liberarLock();
}

int SharedMemoryPassenger::getNextStop(int passengerId){
    size_t startingPos = getStartingPosition(passengerId);
    size_t pos = startingPos + STOP_OFFSET;
    //lock lectura
    LockFile l(SharedMemoryPassenger::shmLockName());
    l.tomarLock();
    int ns = this->leer(pos);
    l.liberarLock();
    return ns;
}

bool SharedMemoryPassenger::hasTicket(int passengerId){
    size_t startingPos = getStartingPosition(passengerId);
    size_t pos = startingPos + TICKET_OFFSET;
    //lock lectura
    LockFile l(SharedMemoryPassenger::shmLockName());
    l.tomarLock();
    bool hasTicket = this->leer(pos);
    l.liberarLock();
    return hasTicket;
}


size_t SharedMemoryPassenger::getStartingPosition(int passengerId){
    return passengerId * FIELDS;
}

SharedMemoryPassenger::~SharedMemoryPassenger(){
    close(this->fd);
    unlink(this->pathname.c_str());
}
