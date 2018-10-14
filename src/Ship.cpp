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
    }
    if(this->shmship != NULL){
        this->shmship->liberar();
        delete this->shmship;
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
    this->shmship = new SharedMemoryShip(Ship::getShmName(id), authorized);
}

void Ship::sail(){
    string logMessage = string("SHIP: ") + to_string(this->id) + string(" STARTED TO SAIL");
    Logger::getInstance().log(logMessage);

    set<int> passengersGettingOff;
    while(this->running()){
        this->harbour = (this->harbour+1) % this->map.size();
        int dstNextHarbour = map.at(this->harbour)->distanceNextHarbour();

        //Get the list of passengers who are getting off the ship at the next stop:
        this->shmPassenger.getPassangersForDestination(passengersGettingOff, this->harbour);

        sleep(dstNextHarbour);

        //Lock harbour: no other ships can come in
        ExclusiveLock lockHarbour(Harbour::harbourLockName(this->harbour));
        logMessage = string("SHIP: ") + to_string(this->id) + string(" ENTRANCE TO HARBOUR ") + to_string(this->harbour);
        // cout << logMessage << endl;
        Logger::getInstance().log(logMessage);

        //Lock ship shared memory (lock it before the inspector has a chance do it)
        // cout << "Lockea mem barco" << endl;
        ExclusiveLock lockShmShip(Ship::getShmName(this->id));

        //Announce ship arrival
        // cout << "Lockea archivo de entrada al muelle" << endl;
        ExclusiveLock lockEntrance(Harbour::entranceLockName(this->harbour));
        this->arrivalAnnouncement(lockEntrance.getfd());
        lockEntrance.unlock();

        this->unblockSigAlarm();
        this->unloadPeople(passengersGettingOff);

        if(!this->running()){
            return;
        }

        this->loadPeople();

        if(!this->running()){
            return;
        }

        //unload passengers (update their locations and Unblock semaphores)
        this->blockSigAlarm();

        lockShmShip.unlock();

        ExclusiveLock lockExit(Harbour::entranceLockName(this->harbour));
        //ver si meterle el metodo  scribir al lock
        this->departureAnnouncement(lockExit.getfd());
        lockExit.unlock();

        lockHarbour.unlock();

        if(this->shmship->confiscated()){
            logMessage = string("SHIP: ") + to_string(this->id) + string(" WAS CONFISCATED AT HARBOUR ") + to_string(this->harbour);
            Logger::getInstance().log(logMessage);
            //Ship confiscated --> DEATH
            exit(0);
        }
        passengersGettingOff.clear();
    }
}

void Ship::arrivalAnnouncement(int fd){
    // int fd = open(Harbour::entranceLockName(id).c_str(), O_CREAT|O_WRONLY, 0666);
    // if (fd < 0){
    //     throw "No se puede anunciar el barco "+ to_string(this->id)+" en el puerto "+ to_string(this->harbour) + strerror(errno) ;
    // }
    // std::cout << "Ship's about to write its id: " << this->id <<  " on harbour "<< this->harbour << " file" << std::endl;
    this->writeInHarbourFile(fd,this->id);
    // close(fd);

    string logMessage = string("SHIP: ") + to_string(this->id) + string(" ANNOUCED IN THE HARRBOUR ") + to_string(this->harbour);
    Logger::getInstance().log(logMessage);
}

void Ship::departureAnnouncement(int fd){
    const int DEPARTUREVALUE = -1;
    // int fd = open(Harbour::entranceLockName(id).c_str(), O_WRONLY, 0666);
    // if (fd < 0){
    //     if (this->running()){
    //         throw "No se puede anunciar partida el barco "+ to_string(this->id)+" en el puerto "+ to_string(this->harbour) ;
    //     }else{
    //         return;
    //     }
    // }
    // std::cout << "Ship's about to write " << DEPARTUREVALUE <<  " on harbour file" << std::endl;
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


void Ship::unloadPeople(set<int> &passengers){
    this->shmship->removePassengers(passengers);
    set<int>::iterator it;
    for (it=passengers.begin(); it!=passengers.end(); ++it){
        //update passenger location:
        string logMessage = string("SHIP: ") + to_string(this->id) + string(" UNLOADING PASSENGER ") + to_string(*it);
        Logger::getInstance().log(logMessage);

        this->shmPassenger.updateLocation(*it,this->harbour);
        //get passenger semaphore:
        tuple<string,char> passSemData = Passenger::getSemaphore(*it);
        Semaphore passengerArrived(0, get<0>(passSemData), get<1>(passSemData));
        //Let passenger know he has arrived at his destination:
        passengerArrived.signal();
    }
}

void Ship::loadPeople(){
    string logMessage;
    int idPassenger;

    FifoLectura fifito(Harbour::entranceName(this->harbour));

    try{

        logMessage = string("SHIP: ") + to_string(this->id) + string(" STARTS LOADING PEOPLE AT HARBOUR ") + to_string(this->harbour);
        Logger::getInstance().log(logMessage);

        int currentNumberOfPassengers = 0;
        for (unsigned int i = 0;i< this->shmship->getPassengers().size(); i++){
            if (this->shmship->getPassengers()[i] >=0){currentNumberOfPassengers++;}
        }

        while(currentNumberOfPassengers < this->capacity){
            alarm(30);
            fifito.abrir();

            if(!this->running()){
                return;
            }

            idPassenger = fifito.leerId();

            if(!this->running()){
                return;
            }

            fifito.cerrar();

            logMessage = string("SHIP: ") + to_string(this->id) + string(" LEYO ") + to_string(this->id);
            Logger::getInstance().log(logMessage);

            if(sigalrm_handler.isActivate() ){
                logMessage = string("SHIP: ") + to_string(idPassenger) + string(" ALARM SOUNDED AT HARBOUR ") + to_string(this->harbour);
                Logger::getInstance().log(logMessage);
            }
            else{
                this->shmship->addPassenger(idPassenger);

                logMessage = string("PASSENGER: ") + to_string(idPassenger) + string(" GET ON SHIP ") + to_string(this->id);
                Logger::getInstance().log(logMessage);

                //TODO actualizar la posicion actual del pasajero en la memoria compartida de pasajeros
            }
            alarm(0);
            sigalrm_handler.restartAlarm();
        }
    }catch(string err){
            fifito.cerrar();

            logMessage = string("SHIP: ") + to_string(this->id) + string(" FINISHED LOAD PEOPLE AT HARBOUR ") + to_string(this->harbour);
            Logger::getInstance().log(logMessage);

            sigalrm_handler.restartAlarm();
    }
}

Ship::~Ship(){
    cout<<getpid()<< "Ship::~Ship() ==> " << to_string(this->id)<<endl;
    this->freeResources();
    //unlink(Ship::getShmName(this->id).c_str());
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
