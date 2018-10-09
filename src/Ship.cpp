#include "Ship.h"
#include "Lock/ExclusiveLock.h"
#include "Fifos/FifoEscritura.h"
#include "Fifos/FifoLectura.h"
#include "errno.h"

using namespace std;

string Ship::getShmName(int shipId){
    return string("shmship")+to_string(shipId)+string(".bin");
}

Ship::Ship(int id, vector<Harbour*> &map, size_t harbour, int capacity, SharedMemoryPassenger& shmPassenger):Process(),
id(id), map(map), harbour(harbour), capacity(capacity), fdShip(-1), shmPassenger(shmPassenger){
    this->initialize();
    srand(time(NULL));
    cout<< "termino de crear el barco en el harbour " << harbour <<endl;
}

void Ship::freeResources(){
    if(this->fdShip > 0){
        close(this->fdShip);
    }
    if(this->shmship != NULL){
        delete this->shmship;
    }
}

void Ship::initialize(){
    size_t totalHarbours = this->map.size();
    if (this->harbour >= (int)totalHarbours){throw "Error at ship creation! Too many Harbours.\n";}
    
    this->fdShip = open(Ship::getShmName(id).c_str(), 0777|O_CREAT|O_WRONLY|O_TRUNC); 
    
    cout<< "barco "<< id<< " abrio su  archivo "<< this->fdShip <<endl;
    if (fdShip < 0 ){
        //TODO:log
        throw "Error al abrir el archivo" + string(strerror(errno));
    }

    bool authorized = rand() % 2;
    this->shmship = new SharedMemoryShip(Ship::getShmName(id), authorized);       

    cout<< "barco memoria compartida"<<this->shmship->getMemId() <<endl;
}

void Ship::sail(){
    while(this->running()){
        int nextHarbour = (this->harbour+1) % this->map.size();----
        // int dstNextHarbour = map[this->harbour]->distanceNextHarbour();
        //cout << "I'm Ship " << getpid() << " leaving for harbour "<< this->harbour+1 << " at distance " <<  dstNextHarbour <<"!\n";
        //Lock harbour so no other ships can approach it:
        //ExclusiveLock lockHarbour(Harbour::harbourLockName(this->harbour));
        //string entranceName = Harbour::entranceName(this->harbour);
        //LockEscritura lockEntrance(entranceName);
        //write id in lockEntrance to mark arrival
        //lockEntrance.unlock();
        // cout << "I'm Ship " << getpid() << " leaving for harbour "<< nextHarbour << " at distance " <<  dstNextHarbour <<"!\n";
        //unload passengers (update their locations and Unblock semaphores)

        //Unblock SIGALRM
        //while (vble_cambiada por SIGALRM){
            //load new passengers
        //}
        //Block SIGALRM!!

        //LockEscritura lockExit(entranceName);
        //write -1 in lockExit to mark exit
        //lockExit.unlock();
        //Lock harbour other ships can approach it:
        //lockHarbour.unlock();
        //check if ship was confiscated and if so exit process


        //Travel to next harbour:
        sleep(5);//TODO:hacerlo proporcional a dstNextHarbour
        this->harbour = nextHarbour; ---
        
        cout << this->harbour << endl;
        for (auto &harbour : map) {
            cout << harbour->getid() << endl;
        }
        
        int dstNextHarbour = map.at(this->harbour)->distanceNextHarbour();
        cout << "I'm Ship " << getpid() << " leaving for harbour "<< nextHarbour << " at distance " <<  dstNextHarbour <<"!\n";
        
        string auth = to_string(this->shmship->authorizedToSail());
        cout << "I'm Ship " << getpid() << " starts authorized " << auth << "\n";
        string conf = this->shmship->confiscated()? "YES":"NO";
        cout << "I'm Ship " << getpid() << " starts confiscated " << conf << "\n";
        this->shmship->confiscateShip();
        conf = this->shmship->confiscated()? "YES":"NO";
        cout << "I'm Ship " << getpid() << " ends confiscated " << conf << "\n";
        
        sleep(dstNextHarbour); //   tiempo hasta destino
        // this->harbour++;
        
        
        ExclusiveLock l(Harbour::entranceLockName(this->harbour));
        cout<< "--------------------sail---------------"<<endl;
        
        this->arrivalAnnouncement();

        this->bajarPeople();

        this->subirPeople();

        cout<< "-----------------------------------"<<endl;
        l.unlock();
    }
}

void Ship::arrivalAnnouncement(){

}

void Ship::subirPeople(){
    FifoLectura fl(); 
    // pero aca el hardboud deberia tener un lfifo de escritura y not
}

void Ship::bajarPeople(){
    FifoEscritura fe(Harbour::entranceLockName(this->harbour)); // es atomica
    
    int i = 0;
    int idPassenger = this->shmship->leer(i);
    while (idPassenger > 0 && i < this->capacity){
        // buscar en el passenger
        // ver si quiere bajar
        // bajarlo
        //elminarlo de la MCship
    }
} 

Ship::~Ship(){
    cout<< "se llama al destructor de ship"<<endl;
    this->freeResources(); 
    // unlink(Ship::getShmName(this->id).c_str());
}
