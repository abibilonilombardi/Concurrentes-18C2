#include "Ship.h"
#include "Lock/ExclusiveLock.h"
#include "Fifos/FifoEscritura.h"
#include "Fifos/FifoLectura.h"
#include "errno.h"
#include <unistd.h>


using namespace std;

string Ship::getShmName(int shipId){
    return string("shmship")+to_string(shipId)+string(".bin");
}

Ship::Ship(int id, vector<Harbour*> &map, size_t harbour, int capacity, SharedMemoryPassenger& shmPassenger):Process(),
id(id), map(map), harbour(harbour), capacity(capacity), fdShip(-1), shmPassenger(shmPassenger){
    this->initialize();
    srand(time(NULL));
    cout<< getpid()<<" Se termino de crear el barco "<<to_string(this->id)<<endl;
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
    
    cout<< getpid()<<" barco "<< id<< " abrio su  archivo "<< this->fdShip <<endl;
    if (fdShip < 0 ){
        //TODO:log
        throw "Error al abrir el archivo" + string(strerror(errno));
    }

    bool authorized = rand() % 2;
    this->shmship = new SharedMemoryShip(Ship::getShmName(id), authorized);       

    cout<<getpid()<< "barco memoria compartida"<<this->shmship->getMemId() <<endl;
}

void Ship::sail(){
    while(this->running()){
        int nextHarbour = (this->harbour+1) % this->map.size();

        int dstNextHarbour = map.at(this->harbour)->distanceNextHarbour();
        cout << getpid()<<" I'm Ship " << getpid() << " leaving for harbour "<< to_string(nextHarbour) << " at distance " <<  to_string(dstNextHarbour) <<"!\n";
        
        sleep(dstNextHarbour); 
        
        // cout << "I'm Ship " << getpid() << " leaving for harbour "<< nextHarbour << " at distance " <<  dstNextHarbour <<"!\n";
        //unload passengers (update their locations and Unblock semaphores)
        
        ExclusiveLock lockHarbour(Harbour::harbourLockName(this->harbour));
        cout<<getpid()<< " Ship" << to_string(this->id)<< " ENTRA AL PUERTO "<< to_string(this->harbour) <<endl;
        
        // ExclusiveLock lockEntrance(Harbour::entranceName(this->harbour));
        // this->arrivalAnnouncement();
        // lockEntrance.unlock();

        //Unblock SIGALRM
        //while (vble_cambiada por SIGALRM){
        //load new passengers
        //}
        //Block SIGALRM!!
        
        // this->unloadPeople();
        
        // this->loadPeople();
        
        // ExclusiveLock lockExit(Harbour::entranceName(this->harbour));
        // this->departureAnnouncement();
        // lockExit.unlock();
        
        cout<< getpid()<<" Ship" << to_string(this->id)<< " SALE DEL PUERTO "<< to_string(this->harbour) <<endl;
        lockHarbour.unlock();
        //check if ship was confiscated and if so exit process
        this->harbour = nextHarbour;
    }
}

void Ship::arrivalAnnouncement(){
    int fd = open(Harbour::entranceLockName(id).c_str(), O_WRONLY, 0666);
    if (fd < 0){
        throw "No se puede anunciar el barco "+ to_string(this->id)+" en el puerto "+ to_string(this->harbour) ;
    }
    this->writeInHarbourFile(fd,this->id);
    close(fd);
    cout<<"Ship::arrivalAnnouncement()"<<endl;
}

void Ship::departureAnnouncement(){
    const int DEPARTUREVALUE = -1;
    int fd = open(Harbour::entranceLockName(id).c_str(), O_WRONLY, 0666);
    if (fd < 0){
        throw "No se puede anunciar partida el barco "+ to_string(this->id)+" en el puerto "+ to_string(this->harbour) ;
    }
    this->writeInHarbourFile(fd, DEPARTUREVALUE);
    close(fd);
}

void Ship::writeInHarbourFile(int fd, int value){
    ssize_t writedBytes = 0;
    while((size_t)writedBytes < sizeof(value)){
        writedBytes += write(fd, to_string(value).c_str() + writedBytes , sizeof(int) - writedBytes);
        if(writedBytes == -1){throw "Error hip::writeInHarbourFile(value) =" + to_string(value);}
    }
}


void Ship::unloadPeople(){
    FifoLectura fl(); 
    // pero aca el hardboud deberia tener un lfifo de escritura y not
}

void Ship::loadPeople(){
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
    cout<<getpid()<< "Ship::~Ship() ==> " << to_string(this->id)<<endl;
    this->freeResources(); 
    // unlink(Ship::getShmName(this->id).c_str());
}
