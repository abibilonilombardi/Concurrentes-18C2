#include "Worker.h"

Worker::Worker(SharedMemoryPassenger &sharedMem, int maxHarbours):
Passenger(sharedMem){
	srand(this->id);//TODO:srand(time(NULL));
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
	string logMessage = string("WORKER: ") + to_string(this->id) + string(" STARTED TO TRAVEL");
	Logger::getInstance().log(logMessage);

	try{
		logMessage = string("WORKER: " + to_string(this->id) + string(" TRAVELING FROM ") + to_string(this->locationStart)+ " TO "+ to_string(this->locationEnd));;
		Logger::getInstance().log(logMessage);
		//Get harbour FIFO name, for harbour at locationStart:
		// string hb = Harbour::entranceName(this->locationStart);
		//Now open it:
		Logger::getInstance().log("Worker with id " +to_string(this->id) + " va a CREAR fifo escritura " );
		FifoEscritura entrance(Harbour::entranceName(this->locationStart));		
		Logger::getInstance().log("Worker with id " +to_string(this->id) + " va a ABRIR fifo escritura " );
		entrance.abrir();
		//Write my id:
		Logger::getInstance().log("Worker with id " +to_string(this->id) + " va a ESCRIBIR fifo escritura " );
		entrance.escribir(static_cast<const void*>(&this->id),sizeof(int));
		entrance.cerrar();
		Logger::getInstance().log("WORKER: " +to_string(this->id) + " QUEUED AT " + to_string(this->locationStart));

		//lock semaphore until I arrive
		this->semTravel->wait();
		int loc = this->sharedMem.getLocation(this->id);
		if (loc != this->locationEnd){
			Logger::getInstance().log("WORKER: " +to_string(this->id) + " WAS FORCED TO GET OFF AT HARBOUR " + to_string(loc));
		}else{
			Logger::getInstance().log("WORKER: " +to_string(this->id) + " ARRIVED AT DESTINATION!");
		    sleep(8); //spend 8hs working...
			Logger::getInstance().log("WORKER: " +to_string(this->id) + " FINISHED WORK!");
		}
	}catch(string error){
		Logger::getInstance().log("ERROR! WORKER: " +to_string(this->id) + " - "+  string(strerror(errno)));
		cerr << "ERROR! " << string(strerror(errno));
		throw string("Worker::travel() ") + error;
	}
}
