#include "Ship.h"

using namespace std;

Ship::Ship(MemoriaCompartida<int> &map, int harbour, int capacity):
Process(),
map(map),
harbour(harbour),
capacity(capacity){
    this->totalHarbours = this->map.leer(0);
    if (harbour>=this->totalHarbours){
        std::string mensaje = std::string("Starting point beyond harbour map!") + "\n";
		throw mensaje;
    }
}

void Ship::sail(){
    while(this->running()){
        int nextHarbour = (this->harbour+1) % this->totalHarbours;

        int dstNextHarbour = this->map.leer(this->harbour+1);
        cout << "I'm Ship " << getpid() << " leaving for harbour "<< this->harbour+1 << " at distance " <<  dstNextHarbour <<"!\n";
        //Viajar.
        sleep(10);//TODO:hacerlo proporcional a dstNextHarbour
        this->harbour = nextHarbour;
        //Subir/bajar pasajeros etc...
    }
}

Ship::~Ship(){
    this->map.liberar();
}
