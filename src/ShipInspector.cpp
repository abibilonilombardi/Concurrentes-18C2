#include "ShipInspector.h"

ShipInspector::ShipInspector(): Inspector(){

}

ShipInspector::inspect(SharedMemoryShip &sharedMemoryShip, SharedMemoryPassenger &sharedMemoryPassenger){
	if (!sharedMemoryShip.authorizedToSail()){
    	sharedMemoryShip.confiscateShip();        	
    	std::vector<int> passengerIds = sharedMemoryShip.getPassengers();
    	for(int i = 0; i < passengerIds.size(); i++){
    		//cambiar ubicacion actual
    		sharedMemoryPassenger.updateLocation(passengerIds[i], harbourToInspect);
    		//borrar de mem de barco al tipo
    		//hacerlo mas lindo
    		sharedMemoryShip.removePassenger(passengerIds[i]);
    		//v al semaforo de pasajero
    	}
    }
}

ShipInspector::~ShipInspector(){}