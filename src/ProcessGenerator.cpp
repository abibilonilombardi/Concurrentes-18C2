#include "ProcessGenerator.h"
#include "Logger/LogMessages.h"

#define MAX_HARBOURS 10 //max amount of harbours total.
#define MAX_PASSENGERS 1 //max amount of passengers total.


ProcessGenerator::ProcessGenerator():Process() {
    Logger::getInstance().log(" --- START TO CREATE HARBOURS ---");
    srand(1);//TODO:srand(time(NULL));

    this->harbourQty = (rand() % MAX_HARBOURS) + 1;

    cout<<"Cantidad de "<<this->harbourQty<<endl;
    for(size_t i=0; i < this->harbourQty; i++){
        this->harbours.push_back(new Harbour(i));
        // cout<< " va a entrar al logger"<< i<<endl;
        Logger::getInstance().log(CREACION_PUERTO_EXITO(i));
    }
    // cout<< " va a crear la memoria"<<endl;
    //this->passengersMem = new SharedMemoryPassenger(SharedMemoryPassenger::shmFileName(), MAX_PASSENGERS);

    cout<< "termina de crearse ProcessGenerator"<<endl;
}

pid_t ProcessGenerator::spawnShips(int quantity, int capacity){
    Logger::getInstance().log(" --- START TO CREATE SHIPS ---");
    SharedMemoryShip::setShipCapacity(capacity);
    SharedMemoryPassenger passMem(MAX_PASSENGERS);
    pid_t pid = 0;

    for (int i=0; i < quantity; i++){
        pid = fork();
        if (pid < 0){
            Logger::getInstance().log("FORK FAILED AT ProcessGenerator::spawnShips()!");
            throw "ProcessGenerator::spawnShips() failed at fork!";
        }
        if (pid==0){
            srand(i);
            int starting_hb = (rand() % this->harbourQty);
            Ship ship(i, this->harbours, starting_hb, capacity, passMem);
            // cout<< getpid() <<"Barco" << i << " se creo con la Memoria"<< this->passengersMem << endl;
            // Logger::getInstance().log(CREACION_BARCO_EXITO(i));
            ship.sail();
            return 0;
        }else{
            this->processes.push_back(pid);
        }
    }
    return pid;
}

pid_t ProcessGenerator::spawnPassenger(){
    Logger::getInstance().log(" --- START TO CREATE PASSENGERS ---");
    pid_t pid = 0;
    //Instanciar inspectores y pasarles la referencia de la memoria
    SharedMemoryPassenger passMem(MAX_PASSENGERS);
    passMem.initialize();
    try{
        pid = fork();
        if (pid < 0){
            Logger::getInstance().log("FORK FAILED AT ProcessGenerator::spawnPassenger()!");
            throw "ProcessGenerator::spawnPassenger() failed at fork!";
        }
        if (pid==0){
            if ((rand()%2)==1){
                Worker w(passMem, this->harbourQty);
                w.travel();
                sleep(1);
            }else{
                //Tourist t(passMem, this->harbourQty);
                //t.travel();
                sleep(1);
            }
            return 0;
        }else{
            this->processes.push_back(pid);
        }
        return pid;
    }catch(string error){
        throw error + " ProcessGenerator::spawnPassenger() ";
    }
}

int ProcessGenerator::beginSimulation(){
    Logger::getInstance().log(" --- BEGIN SIMULATION ---");


    int status;
    //ShipInspector inspector;
    //inspector.behave(MAX_HARBOURS);

    //spawn passanger processes...
    try{
        Semaphore s(MAX_PASSENGERS, "/bin/ls",'a'); //???? no se si quiere esto o o A++?

        while(this->running()){
            s.wait();
            if(this->running()){
                if(spawnPassenger()==0){
                    s.signal();
                    return 0;
                }
            }
        }

        // cout << "Signaling all child processes to end\n";
        vector<int>::iterator procIt;
        for (procIt=this->processes.begin(); procIt!=this->processes.end(); ++procIt){
            //signal all child processes to end in orderly fashion:
            kill(*procIt, SIGINT);
        }
        size_t sz = this->processes.size();
        cout << "Signaling all child processes to end " << to_string(sz) << endl;
        for (size_t i=0; i < sz; i++){
            //wait for all child processes to end:
            wait(&status);
            cout << "Child " << to_string(i+1) << " ended!" << endl;
        }
        s.remove();

        this->processes.clear();
        vector<Harbour*>::iterator hbIt;
        for (hbIt=this->harbours.begin(); hbIt!=this->harbours.end(); ++hbIt){
            delete(*hbIt);
        }
        // cout << "All child processes ended, now exiting main loop...\n";
        return 0;
    }catch(string error){
        throw string( "ProcessGenerator::beginSimulation()" +error);
    }
}

ProcessGenerator::~ProcessGenerator(){}
