#include "ShipInspector.h"
#include "string.h"


#define NO_PASSENGER -1

using namespace std;

ShipInspector::ShipInspector(): Inspector(){

}

void ShipInspector::inspect(int harbourToInspect, SharedMemoryShip &sharedMemoryShip, SharedMemoryPassenger &sharedMemoryPassenger){
	// std::cout << "Barco autorizado: " << sharedMemoryShip.authorizedToSail() << std::endl;
	string logMessage;
	if (this->running() && !sharedMemoryShip.authorizedToSail()){

		logMessage = string("SHIP INSPECTOR: ") + string(" ABOUT TO CONFISCATE SHIP IN HARBOUR: ") + to_string(harbourToInspect);
		Logger::getInstance().log(logMessage);

    	sharedMemoryShip.confiscateShip();
		std::vector<int> passengerIds = sharedMemoryShip.getPassengers();

		logMessage = string("SHIP INSPECTOR: ") + string(" GOT ") + to_string(passengerIds.size()) + string(" PASSENGERS TO UNLOAD FROM SHIP");
		Logger::getInstance().log(logMessage);

    	for(size_t i = 0; i < passengerIds.size(); i++){
    		logMessage = string("SHIP INSPECTOR: ") + string(" ABOUT TO UNLOAD PASSENGER: ") + to_string(passengerIds[i]);
			Logger::getInstance().log(logMessage);
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
				//passSemaphore.remove();
				//borrar de mem de barco al tipo
	    		passengerIds[i] = NO_PASSENGER;

				logMessage = string("SHIP INSPECTOR: ") + string(" UNLOADED PASSENGER: ") + to_string(passengerIds[i]);
				Logger::getInstance().log(logMessage);
	    	}
    	}
    	sharedMemoryShip.updatePassengers(passengerIds);
    }
}

ShipInspector::~ShipInspector(){}
