#include "Harbour.h"
#include "errno.h"
#include "string.h"

#define MAX_DST_HARBOURS 10 //max distance between harbours.
#define BUFFSIZE 1
#define NO_SHIP -1

string Harbour::entranceName(int harbour_id){
    return string("fifopuerto")+to_string(harbour_id);
}

string Harbour::entranceLockName(int harbour_id){
    ostringstream os;
    os << "lockcito_" << harbour_id << ".bin";
    return os.str();
}

string Harbour::harbourLockName(int harbour_id){
    ostringstream os;
    os << "hb_lock_" << harbour_id << ".bin";
    return os.str();
}

Harbour::Harbour(int id):id(id){
    //Open harbour for incoming passangers:
    this->distanceNext = (rand() % MAX_DST_HARBOURS)+1;
    // this->distanceNext = 2; //TODO SOF: CAMBIAR
    this->fdEntrance = open(Harbour::entranceLockName(id).c_str(), O_CREAT|O_WRONLY, 0666);
    // std::cout << "Se crea archivo de harbour: " << Harbour::entranceLockName(id) << std::endl;
    if (this->fdEntrance < 0){
        std::string mensaje = "Error at Harbour creation!";
        throw mensaje;
    }
    writeInHarbourFile(fdEntrance, NO_SHIP);
    close(this->fdEntrance);
    this->fdHarbour = open(Harbour::harbourLockName(id).c_str(), O_CREAT|O_WRONLY, 0666);
    if (this->fdHarbour < 0){
        throw std::string("Error at Harbour creation! ") + string(strerror(errno));
    }
    close(this->fdHarbour);
    Logger::getInstance().log(string("HARBOUR-") + to_string(this->id) + string(" CREATED"), 'd');
}

int Harbour::distanceNextHarbour(){
    return this->distanceNext;
}

void Harbour::writeInHarbourFile(int fd, int value){
    ssize_t writtenBytes = 0;
    while( writtenBytes < (ssize_t)sizeof(value)){
        writtenBytes += write(fd, (char *)&value + writtenBytes , sizeof(value) - writtenBytes);
        if(writtenBytes == -1){
            throw std::string("Error hip::writeInHarbourFile(value) =") + to_string(value);
        }
    }
}

Harbour::~Harbour(){
    //eliminar los archivos de FIFO:
    unlink(Harbour::entranceName(this->id).c_str());
    unlink(Harbour::harbourLockName(this->id).c_str());
    unlink(Harbour::entranceLockName(this->id).c_str());
    Logger::getInstance().log(string("HARBOUR-") + to_string(this->id) + string(" DELETED"), 'd');
}
