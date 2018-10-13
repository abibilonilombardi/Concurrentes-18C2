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

    string logMessage = string("SHIP: ") + to_string(this->id) + string(" CREATED");
    Logger::getInstance().log(logMessage);
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
    
    this->fdShip = open(Ship::getShmName(id).c_str(), O_CREAT|O_WRONLY|O_TRUNC, 0666); 
    
    if (fdShip < 0 ){
        //TODO:log
        throw "Error al abrir el archivo" + string(strerror(errno));
    }

    bool authorized = rand() % 2;
    this->shmship = new SharedMemoryShip(Ship::getShmName(id), authorized);       
}

void Ship::sail(){
    string logMessage = string("SHIP: ") + to_string(this->id) + string(" STARTED TO SAIL");
    Logger::getInstance().log(logMessage);

    while(this->running()){
        int nextHarbour = (this->harbour+1) % this->map.size();

        int dstNextHarbour = map.at(this->harbour)->distanceNextHarbour();
        // cout << getpid()<<" I'm Ship " << getpid() << " leaving for harbour "<< to_string(nextHarbour) << " at distance " <<  to_string(dstNextHarbour) <<"!\n";
        
        sleep(dstNextHarbour); 
        
        this->harbour = nextHarbour;
        // cout << "I'm Ship " << getpid() << " leaving for harbour "<< nextHarbour << " at distance " <<  dstNextHarbour <<"!\n";
        //unload passengers (update their locations and Unblock semaphores)
        
        ExclusiveLock lockHarbour(Harbour::harbourLockName(this->harbour));
        // cout<<getpid()<< " Ship" << to_string(this->id)<< " ENTRA AL PUERTO "<< to_string(this->harbour) <<endl;
        logMessage = string("SHIP: ") + to_string(this->id) + string(" ENTRANCE TO HARBOURR ") + to_string(this->harbour);
        cout << logMessage << endl;
        Logger::getInstance().log(logMessage);

        cout << "Lockea archivo de entrada al muelle" << endl;
        ExclusiveLock lockEntrance(Harbour::entranceLockName(this->harbour));
        this->arrivalAnnouncement(lockEntrance.getfd());
        lockEntrance.unlock();

        cout << "Lockea mem barco" << endl;
        ExclusiveLock lockShmShip(Ship::getShmName(this->id));// -------------------

        sleep(10);

        this->unblockSigAlarm();
        //while (vble_cambiada por SIGALRM){
        this->loadPeople();
        //}
        this->blockSigAlarm();
        
        // this->unloadPeople();

        lockShmShip.unlock(); // -------------------------
        
        ExclusiveLock lockExit(Harbour::entranceLockName(this->harbour));
        //ver si meterle el metodo para escribir al lock
        this->departureAnnouncement(lockExit.getfd());
        lockExit.unlock();
        
        //cout<< getpid()<<" Ship" << to_string(this->id)<< " SALE DEL PUERTO "<< to_string(this->harbour) <<endl;
        // lockHarbour.unlock();
        //check if ship was confiscated and if so exit process
        
        if(this->shmship->confiscated()){
            logMessage = string("SHIP: ") + to_string(this->id) + string(" WAS CONFISCATED AT HARBOUR ") + to_string(this->harbour);
            Logger::getInstance().log(logMessage);
            exit(1); // cuando el barco lo confiscan muere
        }
        
        this->harbour = nextHarbour;
    }
}

void Ship::arrivalAnnouncement(int fd){
    // int fd = open(Harbour::entranceLockName(id).c_str(), O_CREAT|O_WRONLY, 0666);    
    // if (fd < 0){
    //     throw "No se puede anunciar el barco "+ to_string(this->id)+" en el puerto "+ to_string(this->harbour) + strerror(errno) ;
    // }
    this->writeInHarbourFile(fd,this->id);
    // close(fd);

    string logMessage = string("SHIP: ") + to_string(this->id) + string(" ANNOUCED IN THE HARRBOUR ") + to_string(this->harbour);
    Logger::getInstance().log(logMessage);
}

void Ship::departureAnnouncement(int fd){
    const int DEPARTUREVALUE = -1;
    // int fd = open(Harbour::entranceLockName(id).c_str(), O_WRONLY, 0666);
    // if (fd < 0){
    //     throw "No se puede anunciar partida el barco "+ to_string(this->id)+" en el puerto "+ to_string(this->harbour) ;
    // }
    this->writeInHarbourFile(fd, DEPARTUREVALUE);
    // close(fd);

    string logMessage = string("SHIP: ") + to_string(this->id) + string(" DEPARTURE TO HARBOUR ") + to_string(this->harbour);
    Logger::getInstance().log(logMessage);
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
    
    // buscar en el passenger
    // ver si quiere bajar
    // bajarlo
    //elminarlo de la MCship
}


void Ship::loadPeople(){
    string logMessage;

    setDepartureAlarm();

    FifoLectura lp(Harbour::entranceName(this->harbour)); // es atomica
    lp.abrir();

    // int idPassenger = this->shmship->leer(i);
    
    int idPassenger = 0 ; //TODO: sacar puse para probar
    //TODO: matar esto y hacerlo bien en sms
    int currentNumberOfPassengers = 0;
    for (unsigned int i = 0;i< this->shmship->getPassengers().size(); i++){
        // cout<<this->shmship->getPassengers()[i]<<endl;
        if (this->shmship->getPassengers()[i] >=0){currentNumberOfPassengers++;}
    }
    
    logMessage = string("SHIP: ") + to_string(this->id) + string(" CURRENT NUMBER OF PASSENGERS ") + to_string(currentNumberOfPassengers);
    cout<<logMessage<<endl;
    Logger::getInstance().log(logMessage);

    while ( idPassenger >= 0 && currentNumberOfPassengers < this->capacity){
        // logMessage = string("SHIP: ") + to_string(this->id) + string(" STARTS LOADING PEOPLE AT HARBOUR ") + to_string(this->harbour);
        // cout<<logMessage<<endl;
        // Logger::getInstance().log(logMessage);

        int leidos = lp.leer(&idPassenger, sizeof(int));
        cout<<"leidos: "<<leidos<<endl;

        if(sigalrm_handler.isActivate() ){
            logMessage = string("SHIP: ") + to_string(idPassenger) + string(" ALARM SOUNDED AT HARBOUR ") + to_string(this->harbour);
            cout<<logMessage<<endl;
            Logger::getInstance().log(logMessage);

            
            break;
        }
        else{
            this->shmship->addPassenger(idPassenger);
            
            logMessage = string("PASSENGER: ") + to_string(idPassenger) + string(" GET ON SHIP ") + to_string(this->id);
            cout<<logMessage<<endl;
            Logger::getInstance().log(logMessage);
            //TODO actualizar la posicion actual del pasajero en la memoria compartida de pasajeros
        }
        sigalrm_handler.restartAlarm();
        break;
    }

    lp.cerrar();

    logMessage = string("SHIP: ") + to_string(this->id) + string(" FINISHED LOAD PEOPLE AT HARBOUR ") + to_string(this->harbour);
    cout<<logMessage<<endl;
    Logger::getInstance().log(logMessage);
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
    // unsigned int time = 
    alarm(2);
    // cout<< "se activo la alarma del barco "<<this->id<<"  puerto "<<this->harbour << " TIEMPO"<< time<<endl;
    // Logger::getInstance().log("Se activo la alarma");
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
