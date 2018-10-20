#include "Inspector.h"
#include "errno.h"
#include "string.h"


#define MAX_SLEEP_TIME 60 //Ver cuanto conviene


using namespace std;

Inspector::Inspector():Process(){

}

void Inspector::behave(int maxHarbours, int maxPassengers){
    try{
    	int buffer;
    	while(this->running()){
        	//int sleepTime = rand() % MAX_SLEEP_TIME;
        	//sleep(sleepTime);
            sleep(15);
        	int harbourToInspect = RANDOM(maxHarbours);
            // int harbourToInspect = 1;
        	//Accede a archivo de lock
        	int fd = open(Harbour::entranceLockName(harbourToInspect).c_str(), O_RDWR, 0644);
        	if(fd < 0){
                Logger::getInstance().log(string("void Inspector::behave(int maxHarbours, int maxPassengers)") + std::string(strerror(errno)), 'e');
        		throw std::string("File error in Inspector") + std::string(strerror(errno));
			}
			
        	ExclusiveLock l(fd);
        	read(fd, &buffer, sizeof(int));
        	if(buffer != -1 && this->running()){
                SharedMemoryShip sharedMemoryShip(Ship::getShmName(buffer));
                ExclusiveLock l_ship(Ship::getShmName(buffer));
                SharedMemoryPassenger sharedMemoryPassenger(maxPassengers);
                inspect(harbourToInspect, sharedMemoryShip, sharedMemoryPassenger);
                Logger::getInstance().log(string("INSPECTOR-") + to_string(getpid()) + string(" INSPECTED IN THE HARDBOUR-") + to_string(harbourToInspect)+ string(" THE SHIP-")+ to_string(buffer));
                l_ship.unlock();
        	}
            l.unlock();
            close(fd);
    	}
    }catch(std::string error){
        Logger::getInstance().log(string("void Inspector::behave(int maxHarbours, int maxPassengers)") + error, 'e');
        throw string("inspector: ") +error;
    }
    catch(char* error){
        Logger::getInstance().log(string("void Inspector::behave(int maxHarbours, int maxPassengers)") + error, 'e');
        throw string("inspector: ") +error;
    }
}

Inspector::~Inspector(){

}
