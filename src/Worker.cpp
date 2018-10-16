#include "Worker.h"

Worker::Worker(SharedMemoryPassenger &sharedMem, int maxHarbours):
Passenger(sharedMem){
	srand(this->id);//TODO:srand(time(NULL));
	this->locationStart = rand() % maxHarbours;
	this->hasTicket = rand() % 2; //TODO VER POR QUE ES BOOL
	this->locationEnd = rand() % maxHarbours;
	while (locationStart==locationEnd){
		this->locationEnd = rand() % maxHarbours;
	}
	//Write passenger data to shared memory:
	this->id = this->sharedMem.addPassenger(this->locationStart, this->locationEnd, this->hasTicket);
	tuple<string,char> s = Passenger::getSemaphore(this->id);
	this->semTravel = new Semaphore(0, get<0>(s), get<1>(s));

	string logMessage = string("WORKER: ") + to_string(this->id) + string(" CREATED");
    Logger::getInstance().log(logMessage);
}


void Worker::travel(){
	string logMessage = string("WORKER: ") + to_string(this->id) + string(" STARTED TO TRAVEL");
	Logger::getInstance().log(logMessage);

	try{
		logMessage = string("WORKER: " + to_string(this->id) + string(" TRAVELING FROM ") + to_string(this->locationStart)+ " TO "+ to_string(this->locationEnd));
		Logger::getInstance().log(logMessage);
		//Get harbour FIFO name, for harbour at locationStart and open it:
		FifoEscritura entrance(Harbour::entranceName(this->locationStart));
		entrance.abrir();
		if(!this->running()){
			return;
		}
		//Write my id:
		entrance.escribir(static_cast<const void*>(&this->id),sizeof(int));
		Logger::getInstance().log("WORKER: " +to_string(this->id) + " QUEUED AT " + to_string(this->locationStart));
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
			Logger::getInstance().log("WORKER: " +to_string(this->id) + " WAS FORCED TO GET OFF AT HARBOUR " + to_string(loc));
		}else{
			Logger::getInstance().log("WORKER: " +to_string(this->id) + " ARRIVED AT DESTINATION!");
		    sleep(8); //spend 8hs working...
			Logger::getInstance().log("WORKER: " +to_string(this->id) + " FINISHED WORK!");
		}
		entrance.cerrar();
	}catch(string error){
		Logger::getInstance().log("ERROR! WORKER: " +to_string(this->id) + " - "+  string(strerror(errno)));
		throw string("Worker::travel() ") + error;
	}
}
