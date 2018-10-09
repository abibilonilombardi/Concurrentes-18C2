#include "Worker.h"

Worker::Worker(SharedMemoryPassenger &sharedMem, int maxHarbours):
Passenger(sharedMem){
	Logger *l = Logger::getInstance();
	srand(1);//TODO:srand(time(NULL));
	this->locationStart = rand() % maxHarbours;
	this->hasTicket = rand() % 2;
	this->locationEnd = rand() % maxHarbours;
	while (locationStart==locationEnd){
		this->locationEnd = rand() % maxHarbours;
	}
	l->log("Passenger with id " +to_string(this->id) + " created!");
}


void Worker::travel(){
	Logger *l = Logger::getInstance();
	try{
		l->log("Worker with id " +to_string(this->id) + " is traveling from " + to_string(this->locationStart)+ " to "+ to_string(this->locationEnd));
		//Get harbour FIFO name, for harbour at locationStart:
		string hb = Harbour::entranceName(this->locationStart);
		//Now open it:
		FifoEscritura entrance(hb);
		entrance.abrir();
		//Write my id:
		entrance.escribir(static_cast<const void*>(&this->id),sizeof(int));
		entrance.cerrar();
		l->log("Worker with id " +to_string(this->id) + " queued at " + to_string(this->locationStart));

		//lock semaphore until I arrive
		this->semTravel->wait();
		int loc = this->sharedMem.getLocation(this->id);
		if (loc != this->locationEnd){
			l->log("Worker with id " +to_string(this->id) + " was forced to get off at harbour " + to_string(loc));
		}else{
			l->log("Worker with id " +to_string(this->id) + " arrived at destination!");
		    sleep(8); //spend 8hs working...
		}
	}catch(string error){
		l->log("ERROR! Worker with id " +to_string(this->id) + " :"+  string(strerror(errno)));
		cerr << "ERROR! " << string(strerror(errno));
	}
}
