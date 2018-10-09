#include "Inspector.h"
#include "errno.h"
#define MAX_SLEEP_TIME 60 //Ver cuanto conviene

Inspector::Inspector():Process(){
    //string path = string("/tmp/pass")+to_string(id);
    //this->semTravel = new Semaphore(path, 'p');
    //Can't travel unless you've boarded a ship:
    //this->semTravel->initialize(0);
    //TODO: write to shm passenger data.
}

void Inspector::behave(int maxHarbours){
	srand(1);//TODO:srand(time(NULL));
	int *buffer;
	while(this->running()){
    	int sleepTime = rand() % MAX_SLEEP_TIME;
    	sleep(sleepTime);
    	int harbourToInspect = rand() % maxHarbours;
    	//Accede a archivo de lock
    	int fd = open(entranceLockName(harbourToInspect).c_str(), 0777|O_CREAT|O_WRONLY);
    	if(fd < 0){
    		throw "File error " + sterror(errno);
    	}
    	ExclusiveLock l(fd);
    	read(fd, buffer, 1);
    	if(*buffer != -1){
    		//si lo que hay en el archivo es == a -1 chau
    		//si no deberia acceder al archivo de memoria comp del barco
	        SharedMemoryShip sharedMemoryShip(Ship::getShmName(id));
	        SharedMemoryPassenger sharedMemoryPassenger(SharedMemoryPassenger::shmFileName());
	        inspect(sharedMemoryShip, sharedMemoryPassenger);
    	}    	
        l.unlock();
	}
}

Inspector::~Inspector(){
    //Free id?
    //delete this->semTravel;
}
