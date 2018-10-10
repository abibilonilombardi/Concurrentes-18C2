#include "TicketInspector.h"
#define NO_PASSENGER -1

TicketInspector::TicketInspector(): Inspector(){

}

void TicketInspector::inspect(int harbourToInspect, SharedMemoryShip &sharedMemoryShip, SharedMemoryPassenger &sharedMemoryPassenger){
    if (!sharedMemoryShip.authorizedToSail()){
        sharedMemoryShip.confiscateShip();          
        std::vector<int> passengerIds = sharedMemoryShip.getPassengers();
        for(size_t i = 0; i < passengerIds.size(); i++){
            if(passengerIds[i] != NO_PASSENGER){
                if(!sharedMemoryPassenger.hasTicket(passengerIds[i])){
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
                }
            }
        }
    }
}
    
TicketInspector::~TicketInspector(){}