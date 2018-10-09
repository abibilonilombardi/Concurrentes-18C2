#include "Ship.h"

using namespace std;

string Ship::getShmName(int shipId){
    return string("shmship")+to_string(shipId)+string(".bin");
}

Ship::Ship(int id, vector<Harbour*> &map, size_t harbour, int capacity):
Process(),
id(id),
map(map),
harbour(harbour),
capacity(capacity),
fdShip(-1){
    size_t totalHarbours = map.size();
    this->fdShip = open(Ship::getShmName(id).c_str(), O_RDWR|O_CREAT, S_IRGRP|S_IWGRP);

    if (fdShip < 0 || harbour>=totalHarbours){
        close(this->fdShip);
        delete this->shmship;
        //TODO:log
        std::string mensaje = std::string("Error at ship creation! Invalid parameters!\n");
		throw mensaje;
    }
    try{
        srand(time(NULL));
        bool authorized = rand() % 2;
        this->shmship = new SharedMemoryShip(Ship::getShmName(id), authorized);
    }catch(char const* error){
        close(this->fdShip);
        delete this->shmship;
        //TODO:log
        throw error;
    }

}

void Ship::sail(){
    while(this->running()){
        int nextHarbour = (this->harbour+1) % this->map.size();

        int dstNextHarbour = map[this->harbour]->distanceNextHarbour();
        //cout << "I'm Ship " << getpid() << " leaving for harbour "<< this->harbour+1 << " at distance " <<  dstNextHarbour <<"!\n";
        //Lock harbour so no other ships can approach it:
        ExclusiveLock lockHarbour(Harbour::harbourLockName(this->harbour));
        //string entranceName = Harbour::entranceName(this->harbour);
        //LockEscritura lockEntrance(entranceName);
        //write id in lockEntrance to mark arrival
        //lockEntrance.unlock();

        //unload passengers
        //load new passengers

        //LockEscritura lockExit(entranceName);
        //write -1 in lockExit to mark exit
        //lockExit.unlock();
        //Lock harbour other ships can approach it:
        lockHarbour.unlock();
        //check if ship was confiscated and if so exit process


        //Travel to next harbour:
        sleep(5);//TODO:hacerlo proporcional a dstNextHarbour
        this->harbour = nextHarbour;
    }
}

Ship::~Ship(){
    close(this->fdShip);
    delete this->shmship;
    unlink(Ship::getShmName(this->id).c_str());
}
