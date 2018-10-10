#include "ShipInspector.h"

ShipInspector::ShipInspector(): Inspector(){

}

void ShipInspector::inspect(int harbourToInspect, SharedMemoryShip &sharedMemoryShip, SharedMemoryPassenger &sharedMemoryPassenger){
	std::cout << "Entro al inspect " << std::endl;
	if (!sharedMemoryShip.authorizedToSail()){
    	sharedMemoryShip.confiscateShip();        	
    	std::vector<int> passengerIds = sharedMemoryShip.getPassengers();
    	for(size_t i = 0; i < passengerIds.size(); i++){
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