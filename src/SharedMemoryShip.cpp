#include "SharedMemoryShip.h"

size_t SharedMemoryShip::shipCty = 0;

void SharedMemoryShip::setShipCapacity(const int shipCty){
    //TODO:AGREGAR LOCK ACA POR LAS DUDAS
    SharedMemoryShip::shipCty = shipCty;
}

SharedMemoryShip::SharedMemoryShip(const string &pathname, bool authorized){
    //TODO:AGREGAR LOCK ACA POR LAS DUDAS
    //attach to shm, or create and attach if it doesn't exist.
    bool confiscated = 0;
    this->crear(pathname, 's', SharedMemoryShip::shipCty + 2);
    this->escribir(authorized, SharedMemoryShip::shipCty);
    this->escribir(confiscated, SharedMemoryShip::shipCty+1);
    for(size_t j=0; j<SharedMemoryShip::shipCty;j++){
        this->escribir(-1, j);
    }
}

SharedMemoryShip::SharedMemoryShip(const string &pathname){
    this->crear(pathname, 's', SharedMemoryShip::shipCty + 2);//Guarda con donde se usa por el shipCty
}

bool SharedMemoryShip::authorizedToSail(){
    return this->leer(SharedMemoryShip::shipCty)!=0;
}

void SharedMemoryShip::confiscateShip(){
    if (!this->authorizedToSail()){
        this->escribir(1, SharedMemoryShip::shipCty+1);
    }
}

bool SharedMemoryShip::confiscated(){
    return this->leer(SharedMemoryShip::shipCty+1)!=0;
}

std::vector<int> SharedMemoryShip::getPassengers(){
    std::vector<int> passengerIds;
    //LOCK
    for(size_t i = 0; i < SharedMemoryShip::shipCty; i++){
        int passengerId = this->leer(i);
        passengerIds.push_back(passengerId);
    }
    return passengerIds;
}

void SharedMemoryShip::updatePassengers(std::vector<int> &passengerIds){
    //LOCK
    for(size_t i = 0; i < passengerIds.size(); i++){
        this->escribir(passengerIds[i], i);
    }
}

bool SharedMemoryShip::removePassenger(int passengerId){
    //TODO: mejorar esto...
    try{
        for(size_t j=0; j<SharedMemoryShip::shipCty;j++){
            if (this->leer(j)==passengerId){
                this->escribir(-1, j);
                return true;
            }
        }
    }catch(const char *error){
        //log
    }
    return false;
}

bool SharedMemoryShip::addPassenger(int passengerId){
    //TODO: mejorar esto...
    try{
        for(size_t j=0; j<SharedMemoryShip::shipCty;j++){
            if (this->leer(j)==-1){
                this->escribir(passengerId, j);
                return true;
            }
        }
    }catch (const char *error){
        //log
    }
    return false;
}
