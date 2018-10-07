#include "Worker.h"

Worker::Worker(SharedMemoryPassenger &sharedMem, int maxHarbours):
Passenger(sharedMem){
	srand(1);//TODO:srand(time(NULL));
	this->locationStart = rand() % maxHarbours;
	this->locationEnd = rand() % maxHarbours;
	this->hasTicket = rand() % 2;
	if (locationStart==locationEnd){
		locationEnd = (locationEnd+1)% maxHarbours;
	}
	cout << "Passenger with id " << this->id << " created!\n";
}


void Worker::travel(){

    //Get origin harbour FIFO name
    //string hb = Harbour::entranceName(this->originHarbour);
    //TODO:Open harbour FIFO to Write
    //and then proceed to write PID
    //this->travel.p(); //Lock until arrival.
	cout << "Passenger with id " << this->id << " is traveling!\n";
    sleep(10); //spend 8hs working...


}
