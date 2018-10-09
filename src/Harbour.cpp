#include "Harbour.h"
#include "errno.h"
#include "string.h"

#define MAX_DST_HARBOURS 10 //max distance between harbours.
#define BUFFSIZE 1
#define NO_SHIP -1

string Harbour::entranceName(int harbour_id){
    ostringstream os;
    os << "FIFOPUERTO_" << harbour_id;
    return os.str();
}

string Harbour::entranceLockName(int harbour_id){
    ostringstream os;
    os << "lock_" << harbour_id << ".bin";
    return os.str();
}

string Harbour::harbourLockName(int harbour_id){
    ostringstream os;
    os << "hb_lock_" << harbour_id << ".bin";
    return os.str();
}

Harbour::Harbour(int id):id(id){
    this->entrance = new FifoLectura(Harbour::entranceName(id));
    //Open harbour for incoming passangers:
    this->distanceNext = (rand() % MAX_DST_HARBOURS)+1;
    this->fdEntrance = open(Harbour::entranceLockName(id).c_str(), O_CREAT|O_WRONLY, 0666);
    if (this->fdEntrance < 0){
        std::cout << "fdEntrance: " << fdEntrance << std::endl;
        delete this->entrance;
        std::string mensaje = std::string("Error at Harbour creation!");
        throw mensaje;
    }
    //writeInHarbourFile(fdEntrance, NO_SHIP);
    close(this->fdEntrance);
    this->fdHarbour = open(Harbour::harbourLockName(id).c_str(), O_CREAT|O_WRONLY);
    if (this->fdHarbour < 0){
        std::cout << "fdHarbour: " << fdHarbour << std::endl;
        delete this->entrance;
        throw "Error at Harbour creation! " + string(strerror(errno));
    }
    close(this->fdHarbour);
    // cout<<" SE CREO EL HARBOUR " << id << endl;
}

int Harbour::distanceNextHarbour(){
    return this->distanceNext;
}

void Harbour::writeInHarbourFile(int fd, int value){
    ssize_t writedBytes = 0;
    while( writedBytes < (ssize_t)sizeof(value)){
        writedBytes += write(fd, to_string(value).c_str() + writedBytes , sizeof(value) - writedBytes);
        if(writedBytes == -1){throw "Error hip::writeInHarbourFile(value) =" + to_string(value);}
    }
}

Harbour::~Harbour(){
    //TODO:cerrar los FIFOS, y hacer el unlink.
    this->entrance->eliminar();
    delete this->entrance;
    unlink(Harbour::harbourLockName(this->id).c_str());
    unlink(Harbour::entranceLockName(this->id).c_str());
}
