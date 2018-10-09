#include "ProcessGenerator.h"

#define MAX_HARBOURS 10 //max amount of harbours total.
#define MAX_PASSENGERS 1 //max amount of passengers total.

ProcessGenerator::ProcessGenerator():Process(){
    srand(1);//TODO:srand(time(NULL));
    this->harbourQty = (rand() % MAX_HARBOURS) + 1;
    for(size_t i=0; i < this->harbourQty; i++){
        this->harbours.push_back(new Harbour(i));
    }
}

pid_t ProcessGenerator::spawnShips(int quantity, int capacity){
    pid_t pid = 0;
    SharedMemoryShip::setShipCapacity(capacity);

    for (int i=0; i < quantity; i++){
        pid = fork();
        if (pid < 0){ throw "Fallo fork de creacion de barco"; } //TODO
        if (pid==0){
            srand(i);
            int starting_hb = (rand() % this->harbourQty);
            Ship ship(i, this->harbours, starting_hb, capacity);
            ship.sail();
            return 0;
        }else{
            this->processes.insert(pid);
        }
    }
    return pid;
}

pid_t ProcessGenerator::spawnPassenger(SharedMemoryPassenger &passengersMem){
    pid_t pid = 0;
    //Instanciar inspectores y pasarles la referencia de la memoria
    try{
        pid = fork();
        if (pid < 0){ exit(-1); } //TODO: aca lanzar una excepcion;
        if (pid==0){
            //tirar random de 0 a 1 para ver si es turista o worker
            Worker w(passengersMem, this->harbourQty);
            w.travel();
            return 0;
        }else{
            this->processes.insert(pid);
        }
    }catch(string error){
        cout << "ERROR " << error << "\n";
    }
    return pid;
}

int ProcessGenerator::beginSimulation(){
    int status;
    ShipInspector inspector;
    inspector.behave(MAX_HARBOURS);
    //Logger *l = Logger::getInstance();

    //spawn passanger processes...
    //l->log("Spawn people process\n");
    try{
        Semaphore s(MAX_PASSENGERS, "/bin/ls",'A');

        //Create shared memory segment for passengers.
        SharedMemoryPassenger passengersMem(SharedMemoryPassenger::shmFileName(), MAX_PASSENGERS);
        while(this->running()){
            s.wait();
            if(this->running()){
                if(spawnPassenger(passengersMem)==0){
                    s.signal();
                    return 0;
                }
            }
        }
        s.remove();

        //l->log("Signaling all child processes to end\n");
        cout << "Signaling all child processes to end\n";
        std::set<int>::iterator it;
        for (it=this->processes.begin(); it!=this->processes.end(); ++it){
            //signal all child processes to end in orderly fashion:
            kill(*it, SIGINT);
        }
        //l->log("Waiting for all child processes to end\n");
        cout << "Waiting for all child processes to end\n";
        size_t sz = this->processes.size();
        for (size_t i=0; i < sz; i++){
            //wait for all child processes to end:
            pid_t pid = wait(&status);
            this->processes.erase(pid);
        }

        //l->log("All child processes ended, now exiting main loop...\n");
        cout << "All child processes ended, now exiting main loop...\n";
    }catch(string error){
        cout << "ERROR " << error << "\n";
    }
    return 0;
}

ProcessGenerator::~ProcessGenerator(){
    this->processes.clear();
    vector<Harbour*>::iterator it;
    for (it=this->harbours.begin(); it!=this->harbours.end(); ++it){
        Harbour * hb = this->harbours.back();
        delete hb;
        this->harbours.pop_back();
    }
}
