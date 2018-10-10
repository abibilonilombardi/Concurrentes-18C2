#include "Inspector.h"
#include "errno.h"
#define MAX_SLEEP_TIME 60 //Ver cuanto conviene

Inspector::Inspector():Process(){

}

void Inspector::behave(int maxHarbours){
    try{
    	srand(1);//TODO:srand(time(NULL));
    	int buffer;
        std::cout << "ENTRO AL BEHAVE" << std::endl;
    	while(this->running()){
        	//int sleepTime = rand() % MAX_SLEEP_TIME;
        	//sleep(sleepTime);
            sleep(2);
        	int harbourToInspect = rand() % maxHarbours;
        	//Accede a archivo de lock
        	int fd = open(Harbour::entranceLockName(harbourToInspect).c_str(), O_CREAT|O_RDWR, 0777);
        	if(fd < 0){
        		throw std::string("File error ") + std::string(strerror(errno));
        	}
        	ExclusiveLock l(fd);
        	read(fd, &buffer, sizeof(int));
        	std::cout << "Archivo muelle: " << buffer << std::endl;
        	if(buffer != -1){
        		//si lo que hay en el archivo es == a -1 chau
        		//si no deberia acceder al archivo de memoria comp del barco
    	        SharedMemoryShip sharedMemoryShip(Ship::getShmName(buffer));
    	        SharedMemoryPassenger sharedMemoryPassenger(SharedMemoryPassenger::shmFileName());
    	        inspect(harbourToInspect, sharedMemoryShip, sharedMemoryPassenger);
        	}  
            cout << "Antes del unlock inspector" << endl;
            close(fd);            
            l.unlock();
    	}
    }catch(string error){
        cout<< "Error del inspector: "<< error<<endl;
        throw error;
    }
    catch(char* error){
        cout<< "Error del inspector: "<< error<<endl;
        throw error;
    } 
}

Inspector::~Inspector(){
    //Free id?
    //delete this->semTravel;
}
