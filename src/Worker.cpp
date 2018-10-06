#include "Worker.h"

Worker::Worker(int id, SharedMemoryPassenger &sharedMem, int maxHarbours):Passenger(id, sharedMem, maxHarbours){
	this->sharedMem.addPassenger(0, 0, 3, true);
}

void Worker::travel(){
    std::cout << "Ubicacion inicial: " << this->sharedMem.getLocation(0) << std::endl;
    //Get origin harbour FIFO name
    //string hb = Harbour::entranceName(this->originHarbour);
    //TODO:Open harbour FIFO to Write
    //and then proceed to write PID
    //this->travel.p(); //Lock until arrival.
    //sleep(8); //spend 8hs working...
    this->sharedMem.updateLocation(0, 2);
    std::cout << "Ubicacion final: " << this->sharedMem.getLocation(0) << std::endl;
}

Worker::~Worker(){}
