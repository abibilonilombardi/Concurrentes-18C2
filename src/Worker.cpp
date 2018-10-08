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
	try{
		cout << "Passenger with id " << this->id << " is traveling from " << this->locationStart << " to " << this->locationEnd << endl;
		//Get harbour FIFO name, for harbour at locationStart:
		string hb = Harbour::entranceName(this->locationStart);
		//Now open it:
		//FifoEscritura entrance(hb);
		//entrance.abrir();
		//Write my id:
		//entrance.escribir(static_cast<const void*>(&this->id),sizeof(int));
		//TODO: lock semaphore until I arrive
	    sleep(8); //spend 8hs working...
	}catch(string error){
		cerr << "ERROR " << error <<endl;
	}
}
