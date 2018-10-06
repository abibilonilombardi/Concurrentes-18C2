#include "ProcessGenerator.h"

#define MAX_HARBOURS 32 //max amount of harbours total.
#define MAX_PASSENGERS 50 //max amount of passengers total.

ProcessGenerator::ProcessGenerator():Process(),
harbourQty(0){
    srand(1);//TODO:srand(time(NULL));
    this->harbourQty = (rand() % MAX_HARBOURS) + 1;
}

pid_t ProcessGenerator::spawnShips(int quantity, int capacity){
    pid_t pid = 0;

    SharedMemoryShip::setShipCapacity(capacity);

    SharedMemoryMap map(this->harbourQty);
    map.initialize();

    for (int i=0; i < quantity; i++){
        pid = fork();
        if (pid < 0){ exit(-1); } //TODO: aca lanzar una excepcion;
        if (pid==0){
            srand(i);
            int starting_hb = (rand() % this->harbourQty);
            Ship ship(i, map, starting_hb, capacity);
            ship.sail();
            return 0;
        }else{
            this->processes.push_back(pid);
        }
    }
    return pid;
}

pid_t ProcessGenerator::spawnHarbours(){
    pid_t pid = 0;
    for (int i=1; i <= this->harbourQty; i++){
        pid = fork();
        if (pid < 0){ exit(-1); } //TODO: aca lanzar una excepcion;
        if (pid==0){
            Harbour hb(i);
            hb.openHarbour();
            return 0;
        }else{
            this->processes.push_back(pid);
        }
    }
    return pid;
}

pid_t ProcessGenerator::spawnPassengers(){
    pid_t pid = 0;
    //TODO:aca seria ideal hacer un wrapper
    //para acceder a la data del pasajero i
    SharedMemoryPassenger passengersMem("passengers_data.bin", MAX_PASSENGERS);
    //Instanciar inspectores y pasarles la referencia de la memoria
    for (int i=0; i <= MAX_PASSENGERS; i++){
        pid = fork();
        if (pid < 0){ exit(-1); } //TODO: aca lanzar una excepcion;
        if (pid==0){
            //tirar random de 0 a 1 para ver si es turista o worker
            Worker w(i, passengersMem, this->harbourQty);
            w.travel();
            return 0;
        }else{
            this->processes.push_back(pid);
        }
    }
    return pid;
}

int ProcessGenerator::beginSimulation(){
    int status;

    while (this->running()){
        //spawn passanger processes...
        cout << "Parent process " << getpid() << " still alive!\n";
        cout << "Spawn people process\n";
        spawnPassengers();
        sleep(3);
    }
    cout << "Signaling all child processes to end\n";
    size_t totalProcesses = this->processes.size();
    while(!this->processes.empty()){
        //signal all child processes to end in orderly fashion:
        pid_t pid = this->processes.back();
        this->processes.pop_back();
        kill(pid, SIGINT);
    }
    cout << "Waiting for all child processes to end\n";
    for (size_t i=0; i<totalProcesses; i++){
        //wait for all child processes to end:
        wait(&status);
    }
    cout << "All child processes ended, now exiting main loop...\n";
    return 0;
}

ProcessGenerator::~ProcessGenerator(){
    this->processes.clear();
}
