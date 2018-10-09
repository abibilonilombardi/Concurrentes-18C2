#include "TicketInspector.h"

TicketInspector::TicketInspector(): Inspector(){

}

void TicketInspector::inspect(int harbourToInspect, SharedMemoryShip &sharedMemoryShip, SharedMemoryPassenger &sharedMemoryPassenger){
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
    
TicketInspector::~TicketInspector(){}