#include "ProcessGenerator.h"

#define MAX_HARBOURS 32
#define MAP "/bin/bash"

ProcessGenerator::ProcessGenerator():processQty(0){
    //ojo! si hago el attach de la mem. compartida aca TODOS los hijos de este
    //proceso van a quedar attacheados a la memoria compartida!
    srand(1);//TODO:srand(time(NULL));
    this->harbourQty = (rand() % MAX_HARBOURS) + 1;
}


void ProcessGenerator::spawnShips(int quantity, int capacity){
    pid_t pid;

    //TODO: aca, wrappear esto en una clase Mapa, que genere el MAPA
    //con las distancias y que tenga una interfaz para leerlo.
    //Discutir si vale la pena ponerle un lock de lectura
    //al mapa o no.
    this->map.crear(MAP, 'm', 1);
    this->map.escribir(this->harbourQty, 0);
    for (int i=0; i < quantity; i++){
        pid = fork();
        if (pid < 0){ exit(-1); } //TODO: aca lanzar una excepcion;
        if (pid==0){
            int starting_hb = rand() % this->harbourQty;
            Ship ship(this->map,starting_hb, capacity);//TODO: tirar un random entre 1 y el puerto maximo;
            ship.sail();
        }else{
            this->processQty++;
        }
        if (pid==0){
            //TODO:por ahora...
            exit(0);
        }
    }
}

void ProcessGenerator::spawnHarbours(){
    pid_t pid;
    //pid_t puertos[cantidad];
    for (int i=0; i < this->harbourQty; i++){
        pid = fork();
        if (pid < 0){ exit(-1); } //TODO: aca lanzar una excepcion;
        if (pid==0){
            Harbour hb(i);
            hb.openHarbour();
        }else{
            this->processQty++;
        }
        if (pid==0){
            exit(0);
        }
    }
}

ProcessGenerator::~ProcessGenerator(){
    pid_t pid = 0;
    int status;
    for (int i=0; i<this->processQty; i++){
        pid = wait(&status);
        cout << "Child process " << pid <<" terminated with exit status" << WEXITSTATUS(status) <<"\n";
    }
}
