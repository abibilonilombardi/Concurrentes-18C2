#include "Ship.h"
#include "Lock/ExclusiveLock.h"
#include "Fifos/FifoEscritura.h"
#include "Fifos/FifoLectura.h"
#include "Logger/Logger.h"
#include "errno.h"
#include <unistd.h>


using namespace std;

const int SIGALRM_= 14;

string Ship::getShmName(int shipId){
    return string("shmship")+to_string(shipId)+string(".bin");
}

Ship::Ship(int id, vector<Harbour*> &map, size_t harbour, int capacity, SharedMemoryPassenger& shmPassenger):Process(),
id(id), map(map), harbour(harbour), capacity(capacity), fdShip(-1), shmPassenger(shmPassenger){
    this->initialize();
    srand(time(NULL));
    cout<< getpid()<<" Se termino de crear el barco "<<to_string(this->id)<<endl;

    SignalHandler::getInstance()->registrarHandler(SIGALRM, &this->sigint_handler);
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
    
    if (fdShip < 0 ){
        //TODO:log
        throw "Error al abrir el archivo" + string(strerror(errno));
    }

    bool authorized = rand() % 2;
    this->shmship = new SharedMemoryShip(Ship::getShmName(id), authorized);       
}

void Ship::sail(){
    while(this->running()){
        int nextHarbour = (this->harbour+1) % this->map.size();

        int dstNextHarbour = map.at(this->harbour)->distanceNextHarbour();
        // cout << getpid()<<" I'm Ship " << getpid() << " leaving for harbour "<< to_string(nextHarbour) << " at distance " <<  to_string(dstNextHarbour) <<"!\n";
        
        sleep(dstNextHarbour); 
        
        // cout << "I'm Ship " << getpid() << " leaving for harbour "<< nextHarbour << " at distance " <<  dstNextHarbour <<"!\n";
        //unload passengers (update their locations and Unblock semaphores)
        
        // cout << "Lockea harbour" << endl;
        // ExclusiveLock lockHarbour(Harbour::harbourLockName(this->harbour));
        
        cout << "Lockea archivo -1" << endl;
        ExclusiveLock lockEntrance(Harbour::entranceLockName(this->harbour));
        this->arrivalAnnouncement();
        lockEntrance.unlock();

        cout << "Lockea mem barco" << endl;
        ExclusiveLock lockShmShip(Ship::getShmName(this->id));

        this->unblockSigAlarm();
        //while (vble_cambiada por SIGALRM){
        this->loadPeople();
        //}
        this->blockSigAlarm();
        
        // this->unloadPeople();

        lockShmShip.unlock();
        
        ExclusiveLock lockExit(Harbour::entranceLockName(this->harbour));
        this->departureAnnouncement();
        lockExit.unlock();
        
        //cout<< getpid()<<" Ship" << to_string(this->id)<< " SALE DEL PUERTO "<< to_string(this->harbour) <<endl;
        // lockHarbour.unlock();
        //check if ship was confiscated and if so exit process
        
        if(this->shmship->confiscated()){
            //TODO: LOG
            exit(1); // cuando el barco lo confiscan muere
        }
        
        this->harbour = nextHarbour;
    }
}

void Ship::arrivalAnnouncement(){
    int fd = open(Harbour::entranceName(id).c_str(), O_CREAT|O_WRONLY, 0666);
    if (fd < 0){
        throw "No se puede anunciar el barco "+ to_string(this->id)+" en el puerto "+ to_string(this->harbour) + strerror(errno) ;
    }
    this->writeInHarbourFile(fd,this->id);
    std::cout << "LLego barco con id: " << this->id << "al puerto: " << this->harbour << std::endl;
    close(fd);
    // cout<<"Ship::arrivalAnnouncement()"<<endl;
}

void Ship::departureAnnouncement(){
    const int DEPARTUREVALUE = -1;
    int fd = open(Harbour::entranceName(id).c_str(), O_WRONLY, 0666);
    if (fd < 0){
        throw "No se puede anunciar partida el barco "+ to_string(this->id)+" en el puerto "+ to_string(this->harbour) ;
    }
    this->writeInHarbourFile(fd, DEPARTUREVALUE);
    std::cout << "Salio barco con id: " << this->id << "del puerto: " << this->harbour << std::endl;
    close(fd);
    // cout<<"Ship::departureAnnouncement()"<<endl;
}

void Ship::writeInHarbourFile(int fd, int value){
    ssize_t writedBytes = 0;
    while( writedBytes < (ssize_t)sizeof(value)){
        writedBytes += write(fd, (char*)&value + writedBytes , sizeof(value) - writedBytes);
        if(writedBytes == -1){throw std::string("Error hip::writeInHarbourFile(value) =") + to_string(value);}
    }
}


void Ship::unloadPeople(){
    // FifoLectura fl(); 
    // pero aca el hardboud deberia tener un lfifo de escritura y not
}

void Ship::loadPeople(){
    setDepartureAlarm();

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


void Ship::blockSigAlarm() {
	sigset_t sa;
	sigemptyset ( &sa );
	sigaddset ( &sa, SIGALRM_ );
	sigprocmask ( SIG_BLOCK,&sa,NULL );
}

void Ship::unblockSigAlarm() {
	sigset_t sa;
	sigemptyset ( &sa );
	sigaddset ( &sa, SIGALRM_ );
	sigprocmask (SIG_UNBLOCK,&sa,NULL );
}

void Ship::setDepartureAlarm(){  //LLAMAR ANTES DE LEER EL FIFO PARA DEFINIRLE EL TIEMPO MAXIMO
    unsigned int time = alarm(5);
    cout<< "se activo la alarma del barco "<<this->id<<"  puerto "<<this->harbour << " TIEMPO"<< time<<endl;
    Logger::getInstance().log("Se activo la alarma");
}

// void Ship::rangAlamr(int signum){
//     if (signum == SIGALRM_){
//         this->alarmRang = true;
//     }
// }

// void Ship::setAlarmAction(){
// struct sigaction sa;
    
//     memset(&sa, 0, sizeof(sa));
//     sa.sa_handler = t.rangAlamr;
//     sigemptyset ( &sa.sa_mask );	
//     sigaddset ( &sa.sa_mask,SIGALRM_ );
    
//     sigaction (SIGALRM_, &sa, 0);
// }
