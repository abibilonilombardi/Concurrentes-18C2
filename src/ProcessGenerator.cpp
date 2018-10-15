#include "ProcessGenerator.h"
#include "Logger/LogMessages.h"

#define MAX_HARBOURS 3  //max amount of harbours total.
#define MAX_PASSENGERS 5 //max amount of passengers total.


ProcessGenerator::ProcessGenerator():Process() {
    Logger::getInstance().log(" --- START TO CREATE HARBOURS ---");
    srand(1);//TODO:srand(time(NULL));

    this->harbourQty = (rand() % MAX_HARBOURS) + 1;

    // cout<<"Cantidad de harbours: "<<this->harbourQty<<endl;
    for(size_t i=0; i < this->harbourQty; i++){
        this->harbours.push_back(new Harbour(i));
        Logger::getInstance().log(CREACION_PUERTO_EXITO(i));
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
            this->processes.push_back(pid);
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
            // if ((rand()%2)==1){
                Worker w(*this->passengersMem, this->harbourQty);
                w.travel();
                sleep(1);
            // }else{
                //Tourist t(passMem, this->harbourQty);
                //t.travel();
                sleep(1);
            // }
            return 0;
        }else{
            this->processes.push_back(pid);
        }
        return pid;
    }catch(string error){
        throw  string(" ProcessGenerator::spawnPassenger() ") + error;
    }
}

//SpawnInspectors

pid_t ProcessGenerator::spawnShipInspector(){
    Logger::getInstance().log(" --- CREATE SHIP INSPECTOR ---");
    pid_t pid = 0;
    //Instanciar inspectores y pasarles la referencia de la memoria
    try{
        pid = fork();
        if (pid < 0){ exit(-1); } //TODO: aca lanzar una excepcion;
        if (pid==0){
            //tirar random de 0 a 1 para ver si es turista o worker
            ShipInspector inspector;
            inspector.behave(this->harbourQty, MAX_PASSENGERS);
            return 0;
        }else{
            this->processes.push_back(pid);
        }
        return pid;
    }catch(string error){
        throw error + " ProcessGenerator::spawnShipInspector() ";
    }
}

pid_t ProcessGenerator::spawnTicketInspector(){
    Logger::getInstance().log(" --- CREATE TICKET INSPECTOR ---");
    pid_t pid = 0;
    //Instanciar inspectores y pasarles la referencia de la memoria
    try{
        pid = fork();
        if (pid < 0){ exit(-1); } //TODO: aca lanzar una excepcion;
        if (pid==0){
            //tirar random de 0 a 1 para ver si es turista o worker
            TicketInspector inspector;
            inspector.behave(this->harbourQty, MAX_PASSENGERS);
            return 0;
        }else{
            this->processes.push_back(pid);
        }
        return pid;
    }catch(string error){
        throw error + " ProcessGenerator::spawnTicketInspector() ";
    }
}

int ProcessGenerator::beginSimulation(){
    int status;
    // ShipInspector inspector;
    // inspector.behave(MAX_HARBOURS);
    // //Logger *l = Logger::getInstance();

    //spawn passanger processes...
    try{
        Semaphore s(MAX_PASSENGERS, "/bin/ls",'A'); //???? no se si quiere esto o o A++?  ++++++++

        while(this->running()){
            s.wait();
            if(this->running()){
                if(spawnPassenger()==0){
                    s.signal();
                    return 0;
                }
            }
        }

        Logger::getInstance().log(" --- CTRL + C ---");

        vector<int>::iterator procIt;
        for (procIt=this->processes.begin(); procIt!=this->processes.end(); ++procIt){
            //signal all child processes to end in orderly fashion:
            kill(*procIt, SIGINT);
        }
        size_t sz = this->processes.size();
        //cout << "Signaling all child processes to end " << to_string(sz) << endl;
        for (size_t i=0; i < sz; i++){
            //wait for all child processes to end:
            wait(&status);
            //cout << "Child " << to_string(i+1) << " ended!" << endl;
        }
        s.remove();

        this->processes.clear();
        vector<Harbour*>::iterator hbIt;
        for (hbIt=this->harbours.begin(); hbIt!=this->harbours.end(); ++hbIt){
            delete(*hbIt);
        }
        // cout << "All child processes ended, now exiting main loop...\n";
        return 0;
    }catch(string error){
        throw string( "ProcessGenerator::beginSimulation()" +error);
    }
}

ProcessGenerator::~ProcessGenerator(){
    this->passengersMem->liberar();
}
