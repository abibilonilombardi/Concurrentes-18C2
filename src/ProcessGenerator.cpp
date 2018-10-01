#include "ProcessGenerator.h"

#define MAX_HARBOURS 32
#define MAX_DST_HARBOURS 10
#define MAP "/bin/bash"

typedef std::vector<pid_t>::iterator procIter;

ProcessGenerator::ProcessGenerator():Process(),harbourQty(0){
    //ojo! si hago el attach de la mem. compartida aca TODOS los hijos de este
    //proceso van a quedar attacheados a la memoria compartida!
    srand(1);//TODO:srand(time(NULL));
    this->harbourQty = (rand() % MAX_HARBOURS) + 1;


}

void ProcessGenerator::initializeMap(){
    int sz = (this->harbourQty)+1;
    this->map.crear(MAP, 'm', sz);
    this->map.escribir(this->harbourQty, 0);
    for(int i=1; i <= this->harbourQty; i++){
        //escribo la distancia del puerto i al siguiente puerto:
        int distanceNextHarbour = (rand() % MAX_DST_HARBOURS)+1;
        this->map.escribir(distanceNextHarbour, i);
    }
}

pid_t ProcessGenerator::spawnShips(int quantity, int capacity){
    pid_t pid = 0;
    this->initializeMap();

    for (int i=0; i < quantity; i++){
        pid = fork();
        if (pid < 0){ exit(-1); } //TODO: aca lanzar una excepcion;
        if (pid==0){
            srand(i);
            int starting_hb = (rand() % this->harbourQty);
            Ship ship(this->map, starting_hb, capacity);
            ship.sail();
        }else{
            this->processes.push_back(pid);
        }
        if (pid==0){
            exit(0);
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
        }else{
            this->processes.push_back(pid);
        }
        if (pid==0){
            exit(0);
        }
    }
    return pid;
}

int ProcessGenerator::beginSimulation(){
    int status;
    while (this->running()){
        //spawn people processes...
        cout << "Parent process " << getpid() << " still alive!\n";
        cout << "Spawn people process\n";
        sleep(5);
    }
    cout << "Signaling all child processes to end\n";
    while(!this->processes.empty()){
        //signal all child processes to end in orderly fashion:
        pid_t pid = this->processes.back();
        this->processes.pop_back();
        kill(pid, SIGINT);
    }
    cout << "Waiting for all child processes to end\n";
    for (size_t i=0; i<this->processes.size(); i++){
        //wait for all child processes to end:
        wait(&status);
    }
    cout << "All child processes ended, now exiting main loop...\n";
    return 0;
}

ProcessGenerator::~ProcessGenerator(){}
