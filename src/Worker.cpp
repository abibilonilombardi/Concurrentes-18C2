#include "Worker.h"

Worker::Worker(size_t id, SharedMemoryPassenger &sharedMem, int maxPassengers, int maxHarbours):
Passenger(id, sharedMem),
maxPassengers(maxPassengers){
	srand(1);//TODO:srand(time(NULL));
	int locationStart = rand() % maxHarbours;
	int locationEnd = rand() % maxHarbours;
	bool hasTicket = rand() % 2;
	if (locationStart==locationEnd){
		locationEnd = (locationEnd+1)% maxHarbours;
	}
	this->sharedMem.addPassenger(0, locationStart, locationEnd, hasTicket);
}

void Worker::travel(){
	std::string ticket = this->sharedMem.hasTicket(this->id)? "YES" : "NO";
	std::cout << "Pass " << id <<" has ticket?:"<< ticket << std::endl;

    //Get origin harbour FIFO name
    //string hb = Harbour::entranceName(this->originHarbour);
    //TODO:Open harbour FIFO to Write
    //and then proceed to write PID
    //this->travel.p(); //Lock until arrival.
    sleep(3); //spend 8hs working...


}
