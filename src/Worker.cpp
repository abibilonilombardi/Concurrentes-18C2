#include "Worker.h"

Worker::Worker(SharedMemoryPassenger &sharedMem, int maxHarbours):
Passenger(sharedMem){
	srand(1);//TODO:srand(time(NULL));
	this->locationStart = rand() % maxHarbours;
	this->hasTicket = rand() % 2;
	this->locationEnd = rand() % maxHarbours;
	while (locationStart==locationEnd){
		this->locationEnd = rand() % maxHarbours;
	}
	string logMessage = string("WORKER: ") + to_string(this->id) + string(" CREATED");
    Logger::getInstance().log(logMessage);
}


void Worker::travel(){
	string logMessage = string("WORKER: ") + to_string(this->id) + string(" START TO TRAVELING");
	Logger::getInstance().log(logMessage);
	
	try{
		logMessage = string("WORKER: " + to_string(this->id) + string(" TRAVELING FROM ") + to_string(this->locationStart)+ " TO "+ to_string(this->locationEnd));;
		Logger::getInstance().log(logMessage);
		// l.log("Worker with id " +to_string(this->id) + " is traveling from " + to_string(this->locationStart)+ " to "+ to_string(this->locationEnd));
		//Get harbour FIFO name, for harbour at locationStart:
		string hb = Harbour::entranceName(this->locationStart);
		//Now open it:
		FifoEscritura entrance(hb);
		entrance.abrir();
		//Write my id:
		entrance.escribir(static_cast<const void*>(&this->id),sizeof(int));
		entrance.cerrar();
		Logger::getInstance().log("Worker with id " +to_string(this->id) + " queued at " + to_string(this->locationStart));

		//lock semaphore until I arrive
		this->semTravel->wait();
		int loc = this->sharedMem.getLocation(this->id);
		if (loc != this->locationEnd){
			Logger::getInstance().log("Worker with id " +to_string(this->id) + " was forced to get off at harbour " + to_string(loc));
		}else{
			Logger::getInstance().log("Worker with id " +to_string(this->id) + " arrived at destination!");
		    sleep(8); //spend 8hs working...
		}
	}catch(string error){
		Logger::getInstance().log("ERROR! Worker with id " +to_string(this->id) + " :"+  string(strerror(errno)));
		cerr << "ERROR! " << string(strerror(errno));
		throw string("Worker::travel()") + error;
	}
}
