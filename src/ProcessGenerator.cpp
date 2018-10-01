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

void ProcessGenerator::spawnShips(int quantity, int capacity){
    pid_t pid;

    //TODO: aca, wrappear esto en una clase Mapa, que genere el MAPA
    //con las distancias y que tenga una interfaz para leerlo.
    //Discutir si vale la pena ponerle un lock de lectura
    //al mapa o no.
    this->initializeMap();

    for (int i=0; i < quantity; i++){
        pid = fork();
        if (pid < 0){ exit(-1); } //TODO: aca lanzar una excepcion;
        if (pid==0){
            srand(i);
            int starting_hb = (rand() % this->harbourQty);
            Ship ship(this->map, starting_hb, capacity);
            ship.sail();
            exit(0);
        }else{
            this->processes.push_back(pid);
        }
    }
}

void ProcessGenerator::spawnHarbours(){
    pid_t pid;
    for (int i=1; i <= this->harbourQty; i++){
        pid = fork();
        if (pid < 0){ exit(-1); } //TODO: aca lanzar una excepcion;
        if (pid==0){
            Harbour hb(i);
            hb.openHarbour();
            exit(0);
        }else{
            this->processes.push_back(pid);
        }
    }
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
    for (procIter it = this->processes.begin(); it != this->processes.end(); ++it){
        //signal all child processes to end in orderly fashion:
        kill(*it, SIGINT);
    }
    cout << "Waiting for all child processes to end\n";
    for (size_t i=0; i<this->processes.size(); i++){
        //wait for all child processes to end:
        wait(&status);
    }
    cout << "All child processes ended, now exiting main loop...\n";
    return 0;
}

ProcessGenerator::~ProcessGenerator(){

}
