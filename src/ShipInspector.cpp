#include "ShipInspector.h"
#define NO_PASSENGER -1

ShipInspector::ShipInspector(): Inspector(){

}

void ShipInspector::inspect(int harbourToInspect, SharedMemoryShip &sharedMemoryShip, SharedMemoryPassenger &sharedMemoryPassenger){
	std::cout << "Barco autorizado: " << sharedMemoryShip.authorizedToSail() << std::endl;
	if (!sharedMemoryShip.authorizedToSail()){
    	sharedMemoryShip.confiscateShip();        	
    	std::vector<int> passengerIds = sharedMemoryShip.getPassengers();
    	for(size_t i = 0; i < passengerIds.size(); i++){
    		if(passengerIds[i] != NO_PASSENGER){
	    		//cambiar ubicacion actual
	    		sharedMemoryPassenger.updateLocation(passengerIds[i], harbourToInspect);
	    		//borrar de mem de barco al tipo
	    		passengerIds[i] = NO_PASSENGER;
	    		//hacerlo mas lindo
	    		// sharedMemoryShip.removePassenger(passengerIds[i]);
	    		//v al semaforo de pasajero
	    		tuple<string,char> semTuple = Passenger::getSemaphore(passengerIds[i]);
	    		Semaphore *passSemaphore = new Semaphore(0, get<0>(semTuple), get<1>(semTuple));
	    		passSemaphore->signal();
	    		delete passSemaphore;
	    	}
    	}
    	sharedMemoryShip.updatePassengers(passengerIds);
    }
}

ShipInspector::~ShipInspector(){}