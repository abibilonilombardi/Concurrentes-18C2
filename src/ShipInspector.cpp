#include "ShipInspector.h"
#include "string.h"


#define NO_PASSENGER -1

using namespace std;

ShipInspector::ShipInspector(): Inspector(){
	Logger::getInstance().log(string("SHIPINSPECTOR-") + to_string(getpid())+ string(" CREATED"),'d');
}

void ShipInspector::inspect(int harbourToInspect, SharedMemoryShip &sharedMemoryShip, SharedMemoryPassenger &sharedMemoryPassenger){
	// std::cout << "Barco autorizado: " << sharedMemoryShip.authorizedToSail() << std::endl;
	string logMessage;
	if (this->running() && !sharedMemoryShip.authorizedToSail()){
		Logger::getInstance().log(string("SHIPINSPECTOR-") + to_string(getpid())+ string(" ABOUT TO CONFISCATE SHIP IN HARBOUR-")+ to_string(harbourToInspect));
		
    	sharedMemoryShip.confiscateShip();
		std::vector<int> passengerIds = sharedMemoryShip.getPassengers();

    	for(size_t i = 0; i < passengerIds.size(); i++){
    		if(this->running() && passengerIds[i] != NO_PASSENGER){
	    		//cambiar ubicacion actual
	    		sharedMemoryPassenger.updateLocation(passengerIds[i], harbourToInspect);
	    		//v al semaforo de pasajero
	    		if(!this->running()){
                    return;
                }
	    		tuple<string,char> semTuple = Passenger::getSemaphore(passengerIds[i]);
	    		Semaphore passSemaphore(0, get<0>(semTuple), get<1>(semTuple));
				passSemaphore.signal();
				// passSemaphore.remove();
				Logger::getInstance().log(string("SHIPINSPECTOR-") + to_string(getpid())+ string(" UNLOADED PASSENGER-") + to_string(passengerIds[i]) + string(" AT HARDBOUR-") + to_string(harbourToInspect));
	    		passengerIds[i] = NO_PASSENGER;
	    	}
    	}
    	sharedMemoryShip.updatePassengers(passengerIds);
    }
}

ShipInspector::~ShipInspector(){
	Logger::getInstance().log(string("SHIPINSPECTOR-") + to_string(getpid())+ string(" DELETED"),'d');
}
