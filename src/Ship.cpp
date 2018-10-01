#include "Ship.h"

using namespace std;

Ship::Ship(MemoriaCompartida<int> &map, int capacity, int harbour):
map(map),
capacity(capacity),
harbour(harbour){
}

void Ship::sail(){
    cout << "I'm Ship " << getpid() << " leaving harbour "<< this->harbour <<"!\n";
    int harbours = this->map.leer(0);
    cout << "I'm Ship " << getpid() << " y read there are "<< harbours <<" harbours total!\n";
    //Leer la cantidad de puertos(se podria hacer al inicio) y determinar
    //el siguiente puerto (1...K)
    //Leer mi distancia al puerto siguiente.
    //Viajar.
    //Comunicarme con el siguiente puerto.
}

Ship::~Ship(){
    this->map.liberar();
}
