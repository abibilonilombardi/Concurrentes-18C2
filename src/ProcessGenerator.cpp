#include "ProcessGenerator.h"
#include "Logger/LogMessages.h"

#define MAX_HARBOURS 7 //max amount of harbours total.
#define MAX_PASSENGERS 15 //max amount of passengers total.


ProcessGenerator::ProcessGenerator():Process() {
    this->harbourQty = 3 + RANDOM(MAX_HARBOURS);
    Logger::getInstance().log(string("NUMBER OF HARBOURS ") + to_string(this->harbourQty));

    for(size_t i=0; i < this->harbourQty; i++){
        this->harbours.push_back(new Harbour(i));
    }
    this->passengersMem = new SharedMemoryPassenger(MAX_PASSENGERS);
    this->passengersMem->initialize();
}

pid_t ProcessGenerator::spawnShips(int quantity, int capacity){
    SharedMemoryShip::setShipCapacity(capacity);
    pid_t pid = 0;

    for (int i=0; i < quantity; i++){
        pid = fork();
        if (pid < 0){
            Logger::getInstance().log("FORK FAILED AT ProcessGenerator::spawnShips()!");
            throw "ProcessGenerator::spawnShips() failed at fork!";
        }
        if (pid==0){
            srand(i);
            int starting_hb = (rand() % this->harbourQty);
            // int starting_hb = -1; //TODO SOF: CAMBIAR
            Ship ship(i, this->harbours, starting_hb, capacity, *this->passengersMem);
            ship.sail();
            return 0;
        }else{
            this->processes.insert(pid);
        }
    }
    return pid;
}

pid_t ProcessGenerator::spawnPassenger(){
    pid_t pid = 0;
    //Instanciar inspectores y pasarles la referencia de la memoria
    try{
        pid = fork();
        if (pid < 0){
            Logger::getInstance().log("FORK FAILED AT ProcessGenerator::spawnPassenger()!");
            throw "ProcessGenerator::spawnPassenger() failed at fork!";
        }
        if (pid==0){
            if ((RANDOM(2))==1){
                Worker w(*this->passengersMem, this->harbourQty);
                 w.travel();
            }else{
                Tourist t(*this->passengersMem, this->harbourQty);
                t.travel();
            }
            return 0;
        }else{
            this->passengers.insert(pid);
        }
        return pid;
    }catch(string error){
        Logger::getInstance().log(string("ProcessGenerator::spawnPassenger() ") + error,'e');
        throw  string(" ProcessGenerator::spawnPassenger() ") + error;
    }
}

//SpawnInspectors

pid_t ProcessGenerator::spawnShipInspector(){
    pid_t pid = 0;
    //Instanciar inspectores y pasarles la referencia de la memoria
    try{
        pid = fork();
        if (pid < 0){ return -1; } //TODO: aca lanzar una excepcion;
        if (pid==0){
            //tirar random de 0 a 1 para ver si es turista o worker
            ShipInspector inspector;
            inspector.behave(this->harbourQty, MAX_PASSENGERS);
            return 0;
        }else{
            this->processes.insert(pid);
        }
        return pid;
    }catch(string error){
        Logger::getInstance().log("ProcessGenerator::spawnShipInspector():" + error,'e');
        throw error + " ProcessGenerator::spawnShipInspector() ";
    }
}

pid_t ProcessGenerator::spawnTicketInspector(){
    pid_t pid = 0;
    //Instanciar inspectores y pasarles la referencia de la memoria
    try{
        pid = fork();
        if (pid < 0){ return -1; } //TODO: aca lanzar una excepcion;
        if (pid==0){
            //tirar random de 0 a 1 para ver si es turista o worker
            TicketInspector inspector;
            inspector.behave(this->harbourQty, MAX_PASSENGERS);
            return 0;
        }else{
            this->processes.insert(pid);
        }
        return pid;
    }catch(string error){
        throw error + " ProcessGenerator::spawnTicketInspector() ";
    }
}

int ProcessGenerator::beginSimulation(){
    pid_t pid;
    int status;

    //spawn passanger processes...
    try{
        Semaphore s(MAX_PASSENGERS, "/bin/ls",'A'); //???? no se si quiere esto o o A++?  ++++++++
        while(this->running()){
            s.wait();
            if(this->passengers.size()==MAX_PASSENGERS){
                pid = wait(&status);
                this->passengers.erase(pid);
            }
            if(this->running()){
                if(spawnPassenger()==0){
                    s.signal();
                    return 0;
                }
            }
        }

        Logger::getInstance().log(" --- CTRL + C ---");

        set<pid_t>::iterator passIt, procIt;
        for (passIt=this->passengers.begin(); passIt!=this->passengers.end(); ++passIt){
            //signal all passengers to end in orderly fashion:
            kill(*passIt, SIGINT);
        }
        for (procIt=this->processes.begin(); procIt!=this->processes.end(); ++procIt){
            //signal all child processes to end in orderly fashion:
            kill(*procIt, SIGINT);
        }
        for (size_t i=0; i < this->passengers.size(); i++){
            //wait for all passengers to end:
            wait(&status);
        }
        for (size_t i=0; i < this->processes.size(); i++){
            //wait for all child processes to end:
            wait(&status);
        }
        s.remove();

        this->processes.clear();
        this->passengers.clear();
        vector<Harbour*>::iterator hbIt;
        for (hbIt=this->harbours.begin(); hbIt!=this->harbours.end(); ++hbIt){
            delete(*hbIt);
        }
        return 0;
    }catch(string error){
        throw string( "ProcessGenerator::beginSimulation()" +error);
    }
}

ProcessGenerator::~ProcessGenerator(){
    this->passengersMem->liberar();
    unlink(SharedMemoryPassenger::shmLockName().c_str());
    // delete this->passengersMem;
}
