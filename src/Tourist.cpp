#include "Tourist.h"

Tourist::Tourist(SharedMemoryPassenger &sharedMem, int maxHarbours):
Passenger(sharedMem){
    srand(getpid());
    this->locationStart = rand() % ( int(maxHarbours/2) + 1 ) ; //generate random between [0, maxHarbours/2)
    this->hasTicket = rand() % 2;
    this->locationEnd = int(maxHarbours/2) + rand() % ((maxHarbours + 1) - int(maxHarbours/2)); //generate random between [maxHarbours/2, maxHarbours)
    while (locationStart==locationEnd){
		this->locationEnd = rand() % maxHarbours;
    }
    
    //pick random destinations between start and end: TODO MIRAR  SON LAS POSIBLES PARADAS DONDE PUEDEN DECIDIR DI BAJAR O NO
    this->destinations.push_back(this->locationStart);
    for(int i=this->locationStart+1; i<this->locationEnd;i++){
        if ((rand()%maxHarbours)==0){
            this->destinations.push_back(i);
        }
    }
    this->destinations.push_back(this->locationEnd);

    this->locationEnd = this->destinations.at(1);
    this->id = this->sharedMem.addPassenger(this->locationStart, this->locationEnd, this->hasTicket);
    
    Logger::getInstance().log(string("TOURIST: ") + to_string(this->id) + string(" CREATED"));
    Logger::getInstance().log(string("TOURIST: ") + to_string(this->id) + string(" HAS ")+ to_string(this->destinations.size()-2) + string(" ADITIONAL STOPS"));
}

void Tourist::travel(){
	Logger::getInstance().log(string("TOURIST: ") + to_string(this->id) + string(" STARTED TO TRAVEL"));

    try{
        Logger::getInstance().log(string("TOURIST: ") + to_string(this->id) + string(" TRAVELING FROM ") + to_string(this->locationStart)+ " TO "+ to_string(this->locationEnd) + string(" BUT HIS FINAL DESTINY IS ")+ to_string(this->destinations[this->destinations.size()-1]));
        while(!this->destinations.empty()){
            this->locationStart = this->destinations.front();
            this->destinations.erase(this->destinations.begin());
            this->locationEnd = this->destinations.front();

            if ((rand()%2)==1){
                //walk
                int secondsWalking = 5;
                sleep(secondsWalking); //TODO: make this proportional to the 
                Logger::getInstance().log(string("TOURIST: " )+ to_string(this->id) + string(" WALKING BY  ") + to_string(secondsWalking)+ string(" SECONDS " ));
                // this->sharedMem.updateLocation(this->id, end);
                // //l.log("Tourist with id " +to_string(this->id) + " walked from " + to_string(start)+ " to "+ to_string(end));
                // cout << "Tourist with id " << to_string(this->id) << " walked from " << to_string(start) << " to " << to_string(end);
            }else{
                FifoEscritura entrance(Harbour::entranceName(this->locationStart));
                entrance.abrir();
                if(!this->running()){
                    return;
                }
                entrance.escribir(static_cast<const void*>(&this->id),sizeof(int));
                Logger::getInstance().log("TOURIST: " +to_string(this->id) + " QUEUED AT " + to_string(this->locationStart));
                if(!this->running()){
                    return;
                }
                //lock semaphore until I arrive
		        this->semTravel->wait();
		        if(!this->running()){
		        	return;
		        }
		        entrance.cerrar();
		        int loc = this->sharedMem.getLocation(this->id);
            
		        if (loc != this->locationEnd){
		        	Logger::getInstance().log("TOURIST: " +to_string(this->id) + " WAS FORCED TO GET OFF AT HARBOUR " + to_string(loc));
		        }else{
		        	Logger::getInstance().log("TOURIST: " +to_string(this->id) + " ARRIVED AT DESTINATION!");
		        }
		        entrance.cerrar();

            }
        }
        Logger::getInstance().log("Tourist with id " +to_string(this->id) + " arrived at destination!");

	}catch(string error){
        Logger::getInstance().log("ERROR! TOURIST: " +to_string(this->id) + " - "+  string(strerror(errno)));
        throw string("Tourist::travel() ") + error;
	}
}
