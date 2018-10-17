#include "TicketInspector.h"
#define NO_PASSENGER -1

TicketInspector::TicketInspector(): Inspector(){
    Logger::getInstance().log(string("TICKETINSPECTOR-") +to_string(getpid())+ string(" CREATED"),'d');
}

void TicketInspector::inspect(int harbourToInspect, SharedMemoryShip &sharedMemoryShip, SharedMemoryPassenger &sharedMemoryPassenger){
    if(!this->running()){
        return;
    }

    std::vector<int> passengerIds = sharedMemoryShip.getPassengers();
    for(size_t i = 0; i < passengerIds.size(); i++){
        string logMessage = string("TICKET INSPECTOR: ") + string(" ABOUT TO UNLOAD PASSENGER: ") + to_string(passengerIds[i]);
        Logger::getInstance().log(logMessage);
        if(this->running() && passengerIds[i] != NO_PASSENGER){
            if(!sharedMemoryPassenger.hasTicket(passengerIds[i])){
                //cambiar ubicacion actual
                sharedMemoryPassenger.updateLocation(passengerIds[i], harbourToInspect);                
                //v al semaforo de pasajero
                if(!this->running()){
                    return;
                }
                tuple<string,char> semTuple = Passenger::getSemaphore(passengerIds[i]);
                Semaphore passSemaphore(0, get<0>(semTuple), get<1>(semTuple));
                passSemaphore.signal();
                passSemaphore.remove();
                //borrar de mem de barco al tipo
                passengerIds[i] = NO_PASSENGER;
                
                logMessage = string("TICKET INSPECTOR: ") + string(" UNLOADED PASSENGER: ") + to_string(passengerIds[i]);
                Logger::getInstance().log(logMessage);
            }
        }
    }
    sharedMemoryShip.updatePassengers(passengerIds);
}
    
TicketInspector::~TicketInspector(){
    Logger::getInstance().log(string("TICKETINSPECTOR-") + to_string(getpid())+ string(" DELETED"),'d');
}