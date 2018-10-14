#include "Tourist.h"

Tourist::Tourist(SharedMemoryPassenger &sharedMem, int maxHarbours):
Passenger(sharedMem){
    Logger l = Logger::getInstance();
    srand(time(NULL));

    int start = rand() % ( int(maxHarbours/2) + 1 ) ; //generate random between [0, maxHarbours/2)
    int end = int(maxHarbours/2) + rand() % ((maxHarbours + 1) - int(maxHarbours/2)); //generate random between [maxHarbours/2, maxHarbours)
    this->destinations.push(start);
    //pick random destinations between start and end:
    cout << "Destinations \n";
    cout << to_string(start) << endl;
    for(int i=start+1; i<end;i++){
        if ((rand()%2)==1){
            this->destinations.push(i);
            cout << to_string(i) << endl;
        }
    }
    cout << to_string(end) << endl;
    this->destinations.push(end);
    this->hasTicket = rand() % 2;
    l.log("Passenger with id " +to_string(this->id) + " created!");
}

void Tourist::travel(){
    Logger l = Logger::getInstance();
	// try{
        int start = this->destinations.front();
        this->destinations.pop();
        int end = -1;
        while(!this->destinations.empty()){
            end = this->destinations.front();
            this->destinations.pop();
            if ((rand()%2)==1){
                //walk
                sleep(5); //TODO: make this proportional to the distnace
                this->sharedMem.updateLocation(this->id, end);
                //l.log("Tourist with id " +to_string(this->id) + " walked from " + to_string(start)+ " to "+ to_string(end));
                cout << "Tourist with id " << to_string(this->id) << " walked from " << to_string(start) << " to " << to_string(end);
            }else{
                //Get harbour FIFO name, for harbour at start:
                //string hb = Harbour::entranceName(start);
                //Now open it:
                //FifoEscritura entrance(hb);
                //entrance.abrir();
                //Write my id:
                //entrance.escribir(static_cast<const void*>(&this->id),sizeof(int));
                //entrance.cerrar();
                l.log("Tourist with id " +to_string(this->id) + " queued at " + to_string(start));
                sleep(5); //TODO: make this proportional to the distnace
                //lock semaphore until I arrive
                //this->semTravel->wait();
                //int loc = this->sharedMem.getLocation(this->id);
                //if (loc != end){
                //    l.log("Tourist with id " +to_string(this->id) + " was forced to get off at harbour " + to_string(loc));
                //    return;
                //}else{
                cout << "Tourist with id " << to_string(this->id) << " arrived at harbour " << to_string(end);
                l.log("Tourist with id " +to_string(this->id) + " arrived at harbour " + to_string(end));

                //Tourist visits the area:
                cout << "Tourist with id " << to_string(this->id) << " visiting harbour " << to_string(end);
                l.log("Tourist with id " + to_string(this->id) + " visiting harbour " + to_string(end));
                sleep((rand()%10));

                }
            //}
            start = end;
        }
        cout << "Tourist with id " << to_string(this->id) << " arrived at destination!";
        l.log("Tourist with id " +to_string(this->id) + " arrived at destination!");

	// }catch(string error){
	// 	l.log("ERROR! Tourist with id " +to_string(this->id) + " :"+ string(strerror(errno)));
	// 	cerr << "ERROR! " << string(strerror(errno));
	// }
}
