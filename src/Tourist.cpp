#include "Tourist.h"

Tourist::Tourist(SharedMemoryPassenger &sharedMem, int maxHarbours):
Passenger(sharedMem),qtyHarbours(maxHarbours){
    this->locationStart =RANDOM(maxHarbours);//rand() % ( int(maxHarbours/2) + 1 ) ; //generate random between [0, maxHarbours/2)
    this->hasTicket = RANDOM(2);
    this->locationEnd = RANDOM(maxHarbours);//int(maxHarbours/2) + rand() % ((maxHarbours + 1) - int(maxHarbours/2)); //generate random between [maxHarbours/2, maxHarbours)
    while (locationStart==locationEnd){
		this->locationEnd = RANDOM(maxHarbours);
    }

    //pick random destinations between start and end: TODO MIRAR  SON LAS POSIBLES PARADAS DONDE PUEDEN DECIDIR DI BAJAR O NO
    this->destinations.push_back(this->locationStart);
    int diferencia =  this->locationEnd > this->locationStart? this->locationEnd - this->locationStart : this->qtyHarbours + this->locationEnd - this->locationStart;
    for(int i=1; i<diferencia-1;i++){ 
        if (RANDOM(3) == 0 && this->destinations.back() != (this->locationStart+i-1) % maxHarbours){
            this->destinations.push_back((this->locationStart+i)%maxHarbours);
        }
    }
    this->destinations.push_back(this->locationEnd);
    
    int nextDestination = this->destinations.at(1);
    this->id = this->sharedMem.addPassenger(this->locationStart, nextDestination, this->hasTicket);
    tuple<string,char> s = Passenger::getSemaphore(this->id);
	this->semTravel = new Semaphore(0, get<0>(s), get<1>(s));

    string dest;
    for(int j=0; j<(int)this->destinations.size(); j++){
        dest += to_string(this->destinations[j])+ string(" ");
    }
    Logger::getInstance().log(string("PASSENGER-") + to_string(this->id) + string(" CREATED. TOURIST, HAS TICKET: ") + to_string(this->hasTicket) + string(" AND ") + to_string(this->destinations.size()-2) + string(" ADITIONAL STOPS ") + dest,'d');
}

void Tourist::travel(){
    Logger::getInstance().log(string("PASSENGER-") + to_string(this->id) + string(" STARTED TO TRAVEL. TRAVELING FROM ") + to_string(this->locationStart)+ " TO "+ to_string(this->locationEnd));
    try{
        while(this->destinations.size() >= 2){
            this->locationStart = this->destinations.front();
            this->destinations.erase(this->destinations.begin());
            int nextDestination = this->destinations.front();

            FifoEscritura entrance(Harbour::entranceName(this->locationStart));
            entrance.abrir();
            if(!this->running()){
                return;
            }
            entrance.escribir(static_cast<const void*>(&this->id),sizeof(int));
            if(this->failedBoard()){
                Logger::getInstance().log(string("PASSENGER-")  +to_string(this->id) + " FAILED BOARD!");
                entrance.cerrar();
                return;
            }
            Logger::getInstance().log(string("PASSENGER-") +to_string(this->id) + " QUEUED AT HARBOUR-" + to_string(this->locationStart));
            if(!this->running()){
                entrance.cerrar();
                return;
            }
            //lock semaphore until I arrive
		    this->semTravel->wait();
		    if(!this->running()){
                entrance.cerrar();
                return;
		    }
		    // entrance.cerrar();
            int loc = this->sharedMem.getLocation(this->id);

		    if (loc != nextDestination){
                Logger::getInstance().log("PASSENGER-" +to_string(this->id) + " WAS FORCED TO GET OFF AT HARBOUR-" + to_string(loc));
                return;
		    }else if(loc != this->locationEnd){
                Logger::getInstance().log("PASSENGER-" +to_string(this->id) + " IM GOING TO WALK TO NEXT CITY");
                int secondsWalking = 10 + RANDOM(30);
                sleep(secondsWalking); 
                int newBeginig = (nextDestination + 1) % this->qtyHarbours ;
                Logger::getInstance().log(string("PASSENGER-" )+ to_string(this->id) + string(" WALKING BY  ") + to_string(secondsWalking)+ string(" SECONDS TO HARBOUR-") + to_string(newBeginig));
                this->destinations[0]= newBeginig;
                this->sharedMem.updateNextStop(this->id, this->destinations.at(1));
            }else{
                Logger::getInstance().log("PASSENGER-" +to_string(this->id) + " ARRIVED AT DESTINATION!");
            }
            entrance.cerrar();
        }
	}catch(string error){
        Logger::getInstance().log("ERROR! PASSENGER-" +to_string(this->id) + " - "+  string(strerror(errno)));
        throw string("Tourist::travel() ") + error;
	}
}
