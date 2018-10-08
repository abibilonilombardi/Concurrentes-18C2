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

Harbour::Harbour(int id):id(id){
    this->entrance = new FifoLectura(Harbour::entranceName(id));
    //Open harbour for incoming passangers:
    this->distanceNext = (rand() % MAX_DST_HARBOURS)+1;
    this->fd = open(Harbour::entranceLockName(id).c_str(), O_CREAT|O_WRONLY);
    if (this->fd < 0){
        delete this->entrance;
        std::string mensaje = std::string("Error at Harbour creation!");
        throw mensaje;
    }
}

int Harbour::distanceNextHarbour(){
    return this->distanceNext;
}

Harbour::~Harbour(){
    //TODO:cerrar los FIFOS, y hacer el unlink.
    this->entrance->eliminar();
    delete this->entrance;
    close(this->fd);
    unlink(Harbour::entranceLockName(this->id).c_str());
}
