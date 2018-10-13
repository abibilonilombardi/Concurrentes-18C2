#include "ProcessGenerator.h"
#include "Logger/LogMessages.h"

#define MAX_HARBOURS 10 //max amount of harbours total.
#define MAX_PASSENGERS 10 //max amount of passengers total.


ProcessGenerator::ProcessGenerator():Process() {
    Logger::getInstance().log(" --- START TO CREATE HARBOURS ---");
    srand(1);//TODO:srand(time(NULL));

    this->harbourQty = (rand() % MAX_HARBOURS) + 1;

    cout<<"Cantidad de "<<this->harbourQty<<endl;
    for(size_t i=0; i < this->harbourQty; i++){
        this->harbours.push_back(new Harbour(i));
        Logger::getInstance().log(CREACION_PUERTO_EXITO(i));
    }
    this->passengersMem = new SharedMemoryPassenger(SharedMemoryPassenger::shmFileName(), MAX_PASSENGERS);
}

pid_t ProcessGenerator::spawnShips(int quantity, int capacity){
    Logger::getInstance().log(" --- START TO CREATE SHIPS ---");
    SharedMemoryShip::setShipCapacity(capacity);
    SharedMemoryPassenger passMem(MAX_PASSENGERS);
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
    Logger::getInstance().log(" --- START TO CREATE PASSENGERS ---");
    pid_t pid = 0;
    //Instanciar inspectores y pasarles la referencia de la memoria
    SharedMemoryPassenger passMem(MAX_PASSENGERS);
    try{
        pid = fork();
        if (pid < 0){
            Logger::getInstance().log("FORK FAILED AT ProcessGenerator::spawnPassenger()!");
            throw "ProcessGenerator::spawnPassenger() failed at fork!";
        }
        if (pid==0){
            //tirar random de 0 a 1 para ver si es turista o worker
            Worker w(*this->passengersMem, this->harbourQty);
            w.travel();
            return 0;
        }else{
            this->processes.insert(pid);
        }
        return pid;
    }catch(string error){
        throw  string(" ProcessGenerator::spawnPassenger() ") + error;
    }
}

int ProcessGenerator::beginSimulation(){
    Logger::getInstance().log(" --- BEGIN SIMULATION ---");


    int status;
    // ShipInspector inspector;
    // inspector.behave(MAX_HARBOURS);
    // //Logger *l = Logger::getInstance();

    //spawn passanger processes...
    try{
        Semaphore s(MAX_PASSENGERS, "/bin/ls",'A'); //???? no se si quiere esto o o A++?  ++++++++
        
        for (int i=0; i < MAX_PASSENGERS; i++){
            // while(this->running()){
                s.wait();// ++++++++++++++++++++++++
                Logger::getInstance().log("EMPIEZA  spawnPassenger");  
                if(this->running()){
                    
                    if(spawnPassenger()==0){
                    s.signal(); //++++++++++++++++
                    return 0;
                }
            }
        }
        s.remove();  // ++++++++++++++++++++++++++

        // cout << "Signaling all child processes to end\n";
        std::set<int>::iterator it;
        for (it=this->processes.begin(); it!=this->processes.end(); ++it){
            //signal all child processes to end in orderly fashion:
            kill(*it, SIGINT);
        }
        // cout << "Waiting for all child processes to end\n";
        size_t sz = this->processes.size();
        for (size_t i=0; i < sz; i++){
            //wait for all child processes to end:
            pid_t pid = wait(&status);
            this->processes.erase(pid);
        }
        s.remove();

        // cout << "All child processes ended, now exiting main loop...\n";
        return 0;
    }catch(string error){
        throw string( "ProcessGenerator::beginSimulation()" +error);
    }
}

ProcessGenerator::~ProcessGenerator(){
    cout<< getpid() <<" ProcessGenerator::~ProcessGenerator()" <<endl;

    cout<<"CANTIDAD DE PROCESOS CREADOS " <<this->processes.size() <<endl;

    this->processes.clear();
    vector<Harbour*>::iterator it;
    for (it=this->harbours.begin(); it!=this->harbours.end(); ++it){
        delete(*it);
    }
}
