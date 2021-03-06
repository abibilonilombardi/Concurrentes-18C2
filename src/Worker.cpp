#include "Worker.h"

Worker::Worker(SharedMemoryPassenger &sharedMem, int maxHarbours):
Passenger(sharedMem){
	srand(getpid());
	this->locationStart = RANDOM(maxHarbours);//rand() % maxHarbours;
	this->hasTicket = RANDOM(2);
	this->locationEnd = RANDOM(maxHarbours);


	while (locationStart==locationEnd){
		this->locationEnd = rand() % maxHarbours;
	}
	//Write passenger data to shared memory:
	this->id = this->sharedMem.addPassenger(this->locationStart, this->locationEnd, this->hasTicket);
	tuple<string,char> s = Passenger::getSemaphore(this->id);
	this->semTravel = new Semaphore(0, get<0>(s), get<1>(s));

	string logMessage = string("PASSENGER-") + to_string(this->id) + string(" CREATED. WORKER, HAS TICKET: ") + to_string(this->hasTicket);
    Logger::getInstance().log(logMessage,'d');
}


void Worker::travel(){
	Logger::getInstance().log(string("PASSENGER-" + to_string(this->id) + string(" STARTED TO TRAVEL. TRAVELING FROM ") + to_string(this->locationStart)+ " TO "+ to_string(this->locationEnd)));
	try{
		//Get harbour FIFO name, for harbour at locationStart and open it:
		FifoEscritura entrance(Harbour::entranceName(this->locationStart));
		entrance.abrir();
		if(!this->running()){
			return;
		}
		//Write my id:
		entrance.escribir(static_cast<const void*>(&this->id),sizeof(int));
		if(this->failedBoard()){
			Logger::getInstance().log(string("PASSENGER-")  +to_string(this->id) + " FAILED BOARD!");
			entrance.cerrar();
			return;
		}
		Logger::getInstance().log(string("PASSENGER-")  +to_string(this->id) + " QUEUED AT HARBOUR-" + to_string(this->locationStart));
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
		int loc = this->sharedMem.getLocation(this->id);

		if (loc != this->locationEnd){
			Logger::getInstance().log("PASSENGER-" +to_string(this->id) + " WAS FORCED TO GET OFF AT HARBOUR-" + to_string(loc));
		}else{
			Logger::getInstance().log("PASSENGER-" +to_string(this->id) + " ARRIVED AT DESTINATION!");
		    sleep(10); //spend 10 seconds working...
			Logger::getInstance().log("PASSENGER-" +to_string(this->id) + " FINISHED WORK!");
		}
		entrance.cerrar();
	}catch(string error){
		Logger::getInstance().log("PASSENGER-" +to_string(this->id) + " - "+  string(strerror(errno)),'e');
		throw string("Worker::travel() ") + error;
	}
}
