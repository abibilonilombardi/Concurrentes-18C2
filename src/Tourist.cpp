#include "Tourist.h"

Tourist::Tourist(SharedMemoryPassenger &sharedMem, int maxHarbours):
Passenger(sharedMem){
    Logger *l = Logger::getInstance();
    srand(1);//TODO:srand(time(NULL));

    int start = rand() % ( int(maxHarbours/2) + 1 ) ; //generate random between [0, maxHarbours/2)
    int end = int(maxHarbours/2) + rand() % ((maxHarbours + 1) - int(maxHarbours/2)); //generate random between [maxHarbours/2, maxHarbours)
    //pick random destinations between start and end:
    for(size_t i=hbA+1; i<hbB;i++){
        if ((rand()%2)==1){
            this->destinations.push_back(i);
        }
    }
    this->hasTicket = rand() % 2;
    l->log("Passenger with id " +string(this->id) + " created!");
}

void Tourist::travel(){
    Logger *l = Logger::getInstance();
	try{
        vector<int>::iterator it = this->destinations.begin();
        int start = *it, end = -1;
        this->destinations.erase(it); //erase first
        for(it = this->destinations.begin(); it != this->destinations.end()-1; ++it){
            end = *it;
            if ((rand()%2)==1){
                //walk
                sleep(5); //TODO: make this proportional to the distnace
                this->sharedMem.updateLocation(end);
                l->log("Tourist with id " +string(this->id) + " walked from " + string(start)+ " to "+ string(end));
            }else{
                //Get harbour FIFO name, for harbour at start:
                string hb = Harbour::entranceName(start);
                //Now open it:
                FifoEscritura entrance(hb);
                entrance.abrir();
                //Write my id:
                entrance.escribir(static_cast<const void*>(&this->id),sizeof(int));
                entrance.cerrar();
                l->log("Tourist with id " +string(this->id) + " queued at " + string(start));
                //lock semaphore until I arrive
                this->semTravel.wait();
                int loc = this->sharedMem.getLocation(this->id);
                if (loc != end){
                    l->log("Tourist with id " +string(this->id) + " was forced to get off at harbour " + string(loc));
                    return 0;
                }else{
                    l->log("Tourist with id " +string(this->id) + " arrived at harbour " + string(end));
                    if ((rand()%2)==1){
                        //Tourist decides they want to visit the area:
                        sleep((rand()%10));
                    }
                }
                start = end;
            }
            l->log("Tourist with id " +string(this->id) + " arrived at destination!");
            return 0;
        }

	}catch(string error){
		l->log("ERROR! " + "Tourist with id " +string(this->id) + " :"+ string(strerror(errno)));
		cerr << "ERROR! " << string(strerror(errno));
	}
	return 0;
}
