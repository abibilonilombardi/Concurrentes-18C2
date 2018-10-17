#include "Inspector.h"
#include "errno.h"
#include "string.h"


#define MAX_SLEEP_TIME 60 //Ver cuanto conviene


using namespace std;

Inspector::Inspector():Process(){

}

void Inspector::behave(int maxHarbours, int maxPassengers){
	string logMessage;
    try{
		//TODO BORRAR
		if(maxHarbours==0){}//TODO BORRAR

    	srand(getpid());//TODO:srand(time(NULL));
    	int buffer;
    	while(this->running()){
        	//int sleepTime = rand() % MAX_SLEEP_TIME;
        	//sleep(sleepTime);
            sleep(2);
        	int harbourToInspect = rand() % maxHarbours;
            // int harbourToInspect = 1;
            logMessage = string("INSPECTOR: ") + string(" ABOUT TO LOCK HARBOUR-FILE: ") + Harbour::entranceLockName(harbourToInspect);
            Logger::getInstance().log(logMessage);
        	//Accede a archivo de lock
        	int fd = open(Harbour::entranceLockName(harbourToInspect).c_str(), O_RDWR, 0644);
        	if(fd < 0){
        		throw std::string("File error in Inspector") + std::string(strerror(errno));
			}
			
        	ExclusiveLock l(fd);
        	read(fd, &buffer, sizeof(int));
        	// std::cout << "Leido en archivo muelle: " << Harbour::entranceLockName(harbourToInspect) << " valor leido: " << buffer << std::endl;
        	if(buffer != -1){
				// logMessage = string("INSPECTOR: ") + string(" FOUND SHIP-") + to_string(buffer) + string("ON HARBOUR-") + to_string(harbourToInspect);
				// Logger::getInstance().log(logMessage);
        		//si lo que hay en el archivo es == a -1 chau
        		//si no deberia acceder al archivo de memoria comp del barco
                // std::cout << "Va a tomar lock del mem compartida de barco: " << Ship::getShmName(buffer) << std::endl;
				// logMessage = string("INSPECTOR: ") + string(" ABOUT TO LOCK SHIP SHARED MEMORY: ") + Ship::getShmName(buffer);
				// Logger::getInstance().log(logMessage);
				SharedMemoryShip sharedMemoryShip(Ship::getShmName(buffer));
                // std::cout << "Instancio mem compartida del barco" << std::endl;
                ExclusiveLock l_ship(Ship::getShmName(buffer));

                logMessage = string("INSPECTOR: ") + string(" LOCKED SHIP SHARED MEMORY: ") + Ship::getShmName(buffer);
                Logger::getInstance().log(logMessage);
                // std::cout << "Lockeo mem comaprtida barco" << std::endl;
    	        SharedMemoryPassenger sharedMemoryPassenger(maxPassengers);
    	        inspect(harbourToInspect, sharedMemoryShip, sharedMemoryPassenger);
                l_ship.unlock();
        	}
            // cout << "Antes del unlock inspector" << endl;
            l.unlock();
            close(fd);
    	}
    }catch(std::string error){
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

}
