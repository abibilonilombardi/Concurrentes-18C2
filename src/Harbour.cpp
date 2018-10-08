#include "Harbour.h"

string Harbour::exitName(int harbour_id){
    ostringstream os;
    os << "FIFORD_" << harbour_id;
    return os.str();
}

string Harbour::entranceName(int harbour_id){
    ostringstream os;
    os << "FIFOWR_" << harbour_id;
    return os.str();
}


Harbour::Harbour(int id):Process(),id(id){
    //Primero abro el FIFO de escritura para
    //que el barco se quede bloqueado allí en
    //cualquier caso.
    this->exit = new FifoEscritura(Harbour::exitName(id));
    this->entrance = new FifoLectura(Harbour::entranceName(id));
}

void Harbour::openHarbour(){
    cout << "Harbour " << this->id << " is now open!\n";
    while (this->running()){
        sleep(5);
    }
}

Harbour::~Harbour(){
    //TODO:cerrar los FIFOS, y hacer el unlink.
    this->exit->eliminar();
    this->entrance->eliminar();
    delete this->exit;
    delete this->entrance;
}
