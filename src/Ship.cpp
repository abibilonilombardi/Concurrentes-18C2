#include "Ship.h"

using namespace std;

string Ship::getShmName(int shipId){
    return string("shmship")+to_string(shipId)+string(".bin");
}

Ship::Ship(int id, SharedMemoryMap &map, int harbour, int capacity):
Process(),
id(id),
map(map),
harbour(harbour),
capacity(capacity),
fdShip(-1){
    this->totalHarbours = map.totalHarbours();
    this->fdShip = open(Ship::getShmName(id).c_str(), O_RDWR|O_CREAT, S_IRGRP|S_IWGRP);

    if (fdShip < 0 || harbour>=this->totalHarbours){
        close(this->fdShip);
        delete this->shmship;
        //TODO:log
        std::string mensaje = std::string("Error at ship creation! Invalid parameters!\n");
		throw mensaje;
    }
    try{
        srand(time(NULL));
        bool authorized = rand() % 2;
        this->shmship = new SharedMemoryShip(Ship::getShmName(id), false);
    }catch(char const* error){
        close(this->fdShip);
        delete this->shmship;
        //TODO:log
        throw error;
    }

}

void Ship::sail(){
    while(this->running()){
        int nextHarbour = (this->harbour+1) % this->totalHarbours;

        int dstNextHarbour = map.distanceNextHarbour(this->harbour);
        //cout << "I'm Ship " << getpid() << " leaving for harbour "<< this->harbour+1 << " at distance " <<  dstNextHarbour <<"!\n";

        string auth = to_string(this->shmship->authorizedToSail());
        cout << "I'm Ship " << getpid() << " starts authorized " << auth << "\n";
        string conf = this->shmship->confiscated()? "YES":"NO";
        cout << "I'm Ship " << getpid() << " starts confiscated " << conf << "\n";
        this->shmship->confiscateShip();
        conf = this->shmship->confiscated()? "YES":"NO";
        cout << "I'm Ship " << getpid() << " ends confiscated " << conf << "\n";
        //Viajar.
        sleep(5);//TODO:hacerlo proporcional a dstNextHarbour
        this->harbour = nextHarbour;
        //Subir/bajar pasajeros etc...
    }
}

Ship::~Ship(){
    close(this->fdShip);
    delete this->shmship;
    unlink(Ship::getShmName(this->id).c_str());
}
