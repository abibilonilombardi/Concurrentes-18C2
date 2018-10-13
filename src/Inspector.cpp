#include "Inspector.h"
#include "errno.h"
#define MAX_SLEEP_TIME 60 //Ver cuanto conviene

Inspector::Inspector():Process(){

}

void Inspector::behave(int maxHarbours){
    try{
		//TODO BORRAR
		if(maxHarbours==0){}//TODO BORRAR

    	srand(1);//TODO:srand(time(NULL));
    	int buffer;
        std::cout << "ENTRO AL BEHAVE" << std::endl;
    	while(this->running()){
        	//int sleepTime = rand() % MAX_SLEEP_TIME;
        	//sleep(sleepTime);
            sleep(2);
        	// int harbourToInspect = rand() % maxHarbours;
            // std::cout << "max harbours: " << maxHarbours << std::endl;
            int harbourToInspect = 1;
        	//Accede a archivo de lock
        	int fd = open(Harbour::entranceLockName(harbourToInspect).c_str(), O_CREAT|O_RDWR, 0644);
        	if(fd < 0){
        		throw std::string("File error ") + std::string(strerror(errno));
        	}
        	ExclusiveLock l(fd);
        	read(fd, &buffer, sizeof(int));
        	std::cout << "Archivo muelle: " << buffer << std::endl;
        	if(buffer != -1){
        		//si lo que hay en el archivo es == a -1 chau
        		//si no deberia acceder al archivo de memoria comp del barco
                std::cout << "Va a tomar lock del mem compartida de barco: " << Ship::getShmName(buffer) << std::endl;
                ExclusiveLock l_ship(Ship::getShmName(buffer));
    	        SharedMemoryShip sharedMemoryShip(Ship::getShmName(buffer));
    	        SharedMemoryPassenger sharedMemoryPassenger;
    	        inspect(harbourToInspect, sharedMemoryShip, sharedMemoryPassenger);
                l_ship.unlock();
        	}
            // cout << "Antes del unlock inspector" << endl;
            l.unlock();
            close(fd);
    	}
    }catch(string error){
        cout<< "Error del inspector: "<< error<<endl;
        throw string("inspector: ") +error;
    }
    catch(char* error){
        cout<< "Error del inspector: "<< error<<endl;
        throw string("inspector: ") +error;
    }
}

// void Inspector::unloadShipPassenger(int passengerId, int harbourToInspect, SharedMemoryPassenger &sharedMemoryPassenger){
//     //cambiar ubicacion actual
//     sharedMemoryPassenger.updateLocation(passengerIds[i], harbourToInspect);
//     //borrar de mem de barco al tipo
//     passengerIds[i] = NO_PASSENGER;
//     //hacerlo mas lindo
//     // sharedMemoryShip.removePassenger(passengerIds[i]);
//     //v al semaforo de pasajero
//     tuple<string,char> semTuple = Passenger::getSemaphore(passengerIds[i]);
//     Semaphore passSemaphore = new Semaphore(0, get<0>(semTuple), get<1>(semTuple));
//     passSemaphore.signal();
//     delete passSemaphore;
// }

Inspector::~Inspector(){
    //Free id?
    //delete this->semTravel;
}
