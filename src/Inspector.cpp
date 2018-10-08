#include "Inspector.h"
#define MAX_SLEEP_TIME 60 //Ver cuanto conviene

Inspector::Inspector(SharedMemoryPassenger &sharedMem):
Process(),
sharedMem(sharedMem)
{
    //string path = string("/tmp/pass")+to_string(id);
    //this->semTravel = new Semaphore(path, 'p');
    //Can't travel unless you've boarded a ship:
    //this->semTravel->initialize(0);
    //TODO: write to shm passenger data.
}

void Inspector::behave(int maxHarbours){
	srand(1);//TODO:srand(time(NULL));
	while(this->running()){
    	int sleepTime = rand() % MAX_SLEEP_TIME;
    	sleep(sleepTime);
    	int harbourToInspect = rand() % maxHarbours;
    	//Accede a archivo de lock
    	//si lo que hay en el archivo es == a -1 chau
    	//si no deberia acceder al archivo de memoria comp del barco
        SharedMemoryShip sharedMemoryShip(Ship::getShmName(id));
        if (sharedMemoryShip.authorizedToSail(id)){
        	sharedMemoryShip.confiscateShip()        	
        }
    	//metodo que reciba el archivo de mem del barco y ahi desactive la cosa y agarre los ids
	}
}

Inspector::~Inspector(){
    //Free id?
    //delete this->semTravel;
}
