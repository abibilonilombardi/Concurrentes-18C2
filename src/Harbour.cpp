#include "Harbour.h"

#define MAX_DST_HARBOURS 10 //max distance between harbours.
#define BUFFSIZE 1

string Harbour::entranceName(int harbour_id){
    ostringstream os;
    os << "FIFOPUERTO_" << harbour_id;
    return os.str();
}

Harbour::Harbour(int id):id(id){
    this->entrance = new FifoLectura(Harbour::entranceName(id));
    //Open harbour for incoming passangers:
    this->distanceNext = (rand() % MAX_DST_HARBOURS)+1;
}

/*void Harbour::open(){
    //PROVISORIO: esto lo va a hacer el barco...
    int passId;
    this->entrance->abrir();
    this->entrance->leer(static_cast<void*>(&passId),sizeof(int));
    cout << "Harbour received passenger " << passId << "!\n";
}*/

int Harbour::distanceNextHarbour(){
    return this->distanceNext;
}

Harbour::~Harbour(){
    //TODO:cerrar los FIFOS, y hacer el unlink.
    this->entrance->eliminar();
    delete this->entrance;
}
