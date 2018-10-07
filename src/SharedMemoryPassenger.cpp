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
    return string("/bin/cat");
}

SharedMemoryPassenger::SharedMemoryPassenger(std::string pathname, int maxPassengers):
MemoriaCompartida(),
maxPassengers(maxPassengers),
pathname(pathname){
    this->fd = open(pathname.c_str(), O_RDWR|O_CREAT, S_IRGRP|S_IWGRP);
    this->crear(pathname, 'p', maxPassengers * FIELDS);
    ExclusiveLock l(SharedMemoryPassenger::shmLockName());
    for(size_t i=0; i<this->size(); i++){
        //initialize shm in -1;
        this->escribir(FREE, i);
    }
    l.unlock();
}

int SharedMemoryPassenger::addPassenger(int location, int nextStop, bool hasTicket){
    ExclusiveLock l(SharedMemoryPassenger::shmLockName());
    int passengerId = getFreeId();
    if (passengerId < 0){
        l.unlock();
        return passengerId;
    }
    size_t startingPos = getStartingPosition(passengerId);
    this->escribir(location, startingPos + LOCATION_OFFSET);
    this->escribir(nextStop, startingPos + STOP_OFFSET);
    this->escribir(hasTicket, startingPos + TICKET_OFFSET);
    this->escribir(OCCUPIED, startingPos + FREE_ID);
    l.unlock();
    return passengerId;
}

void SharedMemoryPassenger::updateLocation(int passengerId, int location){
    size_t startingPos = getStartingPosition(passengerId);
    size_t pos = startingPos + LOCATION_OFFSET;
    ExclusiveLock l(SharedMemoryPassenger::shmLockName());
    this->escribir(location, pos);
    l.unlock();
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
    ExclusiveLock l(SharedMemoryPassenger::shmLockName());
    this->escribir(FREE,getStartingPosition(passengerId)+FREE_ID);
    l.unlock();
}


int SharedMemoryPassenger::getLocation(int passengerId){
    size_t startingPos = getStartingPosition(passengerId);
    size_t pos = startingPos + LOCATION_OFFSET;
    //lock lectura
    int loc = 0;
    SharedLock l(SharedMemoryPassenger::shmLockName());
    loc = this->leer(pos);
    l.unlock();
    return loc;
}

void SharedMemoryPassenger::updateNextStop(int passengerId, int nextStop){
    //lock
    size_t startingPos = getStartingPosition(passengerId);
    size_t pos = startingPos + STOP_OFFSET;
    ExclusiveLock l(SharedMemoryPassenger::shmLockName());
    this->escribir(nextStop, pos);
    l.unlock();
}

int SharedMemoryPassenger::getNextStop(int passengerId){
    size_t startingPos = getStartingPosition(passengerId);
    size_t pos = startingPos + STOP_OFFSET;
    //lock lectura
    SharedLock l(SharedMemoryPassenger::shmLockName());
    int ns = this->leer(pos);
    l.unlock();
    return ns;
}

bool SharedMemoryPassenger::hasTicket(int passengerId){
    size_t startingPos = getStartingPosition(passengerId);
    size_t pos = startingPos + TICKET_OFFSET;
    //lock lectura
    SharedLock l(SharedMemoryPassenger::shmLockName());
    bool hasTicket = this->leer(pos);
    l.unlock();
    return hasTicket;
}


size_t SharedMemoryPassenger::getStartingPosition(int passengerId){
    return passengerId * FIELDS;
}

SharedMemoryPassenger::~SharedMemoryPassenger(){
    close(this->fd);
    unlink(this->pathname.c_str());
}
