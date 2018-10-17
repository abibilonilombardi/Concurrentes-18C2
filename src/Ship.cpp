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

    SignalHandler::getInstance()->registrarHandler(SIGALRM, &this->sigalrm_handler);

    string logMessage = string("SHIP: ") + to_string(this->id) + string(" CREATED");
    Logger::getInstance().log(logMessage);
}

void Ship::freeResources(){
    if(this->fdShip > 0){
        close(this->fdShip);
        Logger::getInstance().log(string("SHIP: ") + to_string(this->id) + string(" CERRO FILE DESCRIPTOR") );
    }
    if(this->shmship != NULL){
        delete this->shmship;
        // unlink(Ship::getShmName(this->id).c_str());
        Logger::getInstance().log(string("SHIP: ") + to_string(this->id) + string(" eLIMINO MEMORIA COKMPARTID") );
    }
}

void Ship::initialize(){
    size_t totalHarbours = this->map.size();
    if (this->harbour >= (int)totalHarbours){
        string message = string("ERROR! TOO MANY HARBOURS AT Ship::initialize()!");
        Logger::getInstance().log(message);
        throw message;
    }

    this->fdShip = open(Ship::getShmName(id).c_str(), O_CREAT|O_WRONLY|O_TRUNC, 0666);

    if (fdShip < 0 ){
        string message = string("ERROR! SHIP COULD NOT OPEN SHM FILE!");
        Logger::getInstance().log(message);
        throw message + string(strerror(errno));
    }

    bool authorized = rand() % 2;
    // bool authorized = 0;//TODO SOF: CAMBIAR
    this->shmship = new SharedMemoryShip(Ship::getShmName(id), authorized);
}

void Ship::sail(){
    string logMessage = string("SHIP: ") + to_string(this->id) + string(" STARTED TO SAIL");
    Logger::getInstance().log(logMessage);

    while(this->running()){
        this->harbour = (this->harbour+1) % this->map.size();
        int dstNextHarbour = map.at(this->harbour)->distanceNextHarbour();

        sleep(dstNextHarbour);

        ExclusiveLock lockHarbour(Harbour::harbourLockName(this->harbour));
        logMessage = string("SHIP: ") + to_string(this->id) + string(" ENTRANCE TO HARBOUR ") + to_string(this->harbour);
        Logger::getInstance().log(logMessage);

        ExclusiveLock lockShmShip(Ship::getShmName(this->id));

        ExclusiveLock lockEntrance(Harbour::entranceLockName(this->harbour));
        this->arrivalAnnouncement(lockEntrance.getfd());
        lockEntrance.unlock();

        this->unloadPeople();

        if(!this->running()){
            return;
        }

        this->unblockSigAlarm();
        this->loadPeople();
        this->blockSigAlarm();

        if(!this->running()){
            return;
        }

        lockShmShip.unlock();

        ExclusiveLock lockExit(Harbour::entranceLockName(this->harbour));
        //ver si meterle el metodo  scribir al lock
        this->departureAnnouncement(lockExit.getfd());
        lockExit.unlock();

        lockHarbour.unlock();

        if(this->shmship->confiscated()){
            Logger::getInstance().log(string("SHIP: ") + to_string(this->id) + string(" WAS CONFISCATED AT HARBOUR ") + to_string(this->harbour));
            // exit(0);
            return;
        }
    }
}

void Ship::arrivalAnnouncement(int fd){
    this->writeInHarbourFile(fd,this->id);
    // close(fd);
    Logger::getInstance().log(string("SHIP: ") + to_string(this->id) + string(" ANNOUCED IN THE HARRBOUR ") + to_string(this->harbour));
}

void Ship::departureAnnouncement(int fd){
    const int DEPARTUREVALUE = -1;
    
    this->writeInHarbourFile(fd, DEPARTUREVALUE);
    // close(fd);

    Logger::getInstance().log(string("SHIP: ") + to_string(this->id) + string(" DEPARTURE TO HARBOUR ") + to_string(this->harbour));
}

void Ship::writeInHarbourFile(int fd, int value){
    ssize_t writedBytes = 0;
    while( writedBytes < (ssize_t)sizeof(value)){
        writedBytes += write(fd, (char*)&value + writedBytes , sizeof(value) - writedBytes);
        if(writedBytes == -1){throw std::string("Error hip::writeInHarbourFile(value) =") + to_string(value);}
    }
}


void Ship::unloadPeople(){
    Logger::getInstance().log(string("SHIP: ") + to_string(this->id) + string(" STARTS UNLOADING PEOPLE AT HARBOUR ") + to_string(this->harbour));
    vector<int> shipPassengers = this->shmship->getPassengers();
    
    int unloded = 0;
    vector<int>::iterator it;
    for (it=shipPassengers.begin(); it!=shipPassengers.end(); ++it){
        if (*it < 0) {continue;}

        // string logMessage = "Passenger: " + to_string(*it) + " getNextStop: " + to_string(this->shmPassenger.getNextStop(*it)) + " Harbour actual del ship: " + to_string(this->harbour);
        // Logger::getInstance().log(logMessage);

        if(this->shmPassenger.getNextStop(*it) == this->harbour ){
            Logger::getInstance().log(string("SHIP: ") + to_string(this->id) + string(" UNLOADING PASSENGER ") + to_string(*it));

            this->shmPassenger.updateLocation(*it,this->harbour);
            if(!this->running()){
                    return;
                }
            tuple<string,char> passSemData = Passenger::getSemaphore(*it);
            Semaphore passengerArrived(0, get<0>(passSemData), get<1>(passSemData));
            //Let passenger know he has arrived at his destination:
            passengerArrived.signal();
            passengerArrived.remove();
            *it = -1;
            unloded++;
        }
    }
    if (unloded == 0){
        Logger::getInstance().log(string("SHIP: ") + to_string(this->id) + string(" NO PASSENGER UNLOADED AT HARBOUR ") + to_string(this->harbour));   
        return;
    }
    this->shmship->updatePassengers(shipPassengers);
    Logger::getInstance().log(string("SHIP: ") + to_string(this->id) + string(" FINISHED UNLOAD PEOPLE AT HARBOUR ") + to_string(this->harbour));
}

void Ship::loadPeople(){
    int idPassenger;
    FifoLectura fifito(Harbour::entranceName(this->harbour));

    Logger::getInstance().log(string("SHIP: ") + to_string(this->id) + string(" STARTS LOADING PEOPLE AT HARBOUR ") + to_string(this->harbour));
    
    int currentNumberOfPassengers = 0;
    for (unsigned int i = 0;i< this->shmship->getPassengers().size(); i++){
        if (this->shmship->getPassengers()[i] >=0){
            currentNumberOfPassengers++;
        }
    }
    
    alarm(5); // espera por si llega pasajero
    try{
        fifito.abrir();
    }catch(string err){
        alarm(0);
        this->sigalrm_handler.restartAlarm();
        Logger::getInstance().log(string("SHIP: ") + to_string(this->id) + string(" NO PASSENGER AT HARBOUR ") + to_string(this->harbour) );
        return;
    }
    while(currentNumberOfPassengers < this->capacity && !this->sigalrm_handler.isActivate()){
        alarm(15);
        idPassenger = fifito.leerId();
        
        if(this->sigalrm_handler.isActivate()){
            Logger::getInstance().log(string("SHIP: ") + to_string(this->id) + string(" ALARM SOUNDED AT HARBOUR ") + to_string(this->harbour));
            sigalrm_handler.restartAlarm();
            break;
        }else{
            if(!this->running() || idPassenger == -1 ){
                Logger::getInstance().log(string(" interrumpido o EOF ") + to_string(this->harbour));
                break;
            }
            alarm(0);
            Logger::getInstance().log(string("SHIP: ") + to_string(this->id) + string(" READ ") + to_string(idPassenger));
            this->shmship->addPassenger(idPassenger);
            Logger::getInstance().log(string("PASSENGER: ") + to_string(idPassenger) + string(" GET ON SHIP ") + to_string(this->id));
            currentNumberOfPassengers++;
        }
    }
    fifito.cerrar();
    Logger::getInstance().log(string("SHIP: ") + to_string(this->id) + string(" FINISHED LOAD PEOPLE AT HARBOUR ") + to_string(this->harbour) );
}

Ship::~Ship(){
    Logger::getInstance().log(string("SHIP: ") + to_string(this->id) + string(" DESTRUCTOR")  );
    this->freeResources();
    unlink(Ship::getShmName(this->id).c_str());
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
