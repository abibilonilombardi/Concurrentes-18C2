#include "Harbour.h"

#define MAX_DST_HARBOURS 10 //max distance between harbours.
#define BUFFSIZE 1

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
    this->fdEntrance = open(Harbour::entranceLockName(id).c_str(), O_CREAT|O_WRONLY);
    if (this->fdEntrance < 0){
        delete this->entrance;
        std::string mensaje = std::string("Error at Harbour creation!");
        throw mensaje;
    }
    close(this->fdHarbour);
    this->fdHarbour = open(Harbour::harbourLockName(id).c_str(), O_CREAT|O_WRONLY);
    if (this->fdHarbour < 0){
        delete this->entrance;
        std::string mensaje = std::string("Error at Harbour creation!");
        throw mensaje;
    }
    close(this->fdHarbour);
}

int Harbour::distanceNextHarbour(){
    return this->distanceNext;
}

Harbour::~Harbour(){
    //TODO:cerrar los FIFOS, y hacer el unlink.
    this->entrance->eliminar();
    delete this->entrance;
    unlink(Harbour::harbourLockName(this->id).c_str());
    unlink(Harbour::entranceLockName(this->id).c_str());
}
