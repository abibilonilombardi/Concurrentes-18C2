#include "ProcessGenerator.h"
#include "Logger/LogMessages.h"

#define MAX_HARBOURS 10 //max amount of harbours total.
#define MAX_PASSENGERS 1 //max amount of passengers total.


ProcessGenerator::ProcessGenerator():Process() {
    srand(1);//TODO:srand(time(NULL));
    
    this->harbourQty = (rand() % MAX_HARBOURS) + 1;

    cout<<"Cantidad de harbours: "<<this->harbourQty<<endl;
    for(size_t i=0; i < this->harbourQty; i++){
        this->harbours.push_back(new Harbour(i));
        cout<< " va a entrar al logger"<< i<<endl;
        Logger::getInstance().log(CREACION_PUERTO_EXITO(i));
    }
    cout<< " va a crear la memoria"<<endl;
    this->passengersMem = new SharedMemoryPassenger(SharedMemoryPassenger::shmFileName(), MAX_PASSENGERS);
    
    cout<< "termina de crearse ProcessGenerator"<<endl;
}

pid_t ProcessGenerator::spawnShips(int quantity, int capacity){
    SharedMemoryShip::setShipCapacity(capacity);
    pid_t pid = 0;

    for (int i=0; i < quantity; i++){
        pid = fork();
        if (pid < 0){ throw "Fallo fork de creacion de barco"; } //TODO
        if (pid==0){
            srand(i);
            // int starting_hb = (rand() % this->harbourQty);
            int starting_hb = 1;
            Ship ship(i, this->harbours, starting_hb, capacity, *this->passengersMem);
            cout<< getpid() <<"Barco" << i << " se creo con la Memoria"<< this->passengersMem << "para el harbour: " << starting_hb << endl;
            // Logger::getInstance().log(CREACION_BARCO_EXITO(i));
            ship.sail();
            return 0;
        }else{
            this->processes.insert(pid);
        }
    }
    return pid;
}

pid_t ProcessGenerator::spawnPassenger(){
    pid_t pid = 0;
    //Instanciar inspectores y pasarles la referencia de la memoria
    try{
        pid = fork();
        if (pid < 0){ exit(-1); } //TODO: aca lanzar una excepcion;
        if (pid==0){
            //tirar random de 0 a 1 para ver si es turista o worker
            Worker w(*this->passengersMem, this->harbourQty);
            Logger::getInstance().log("Pasajero creado con exitoS");
            w.travel();
            return 0;
        }else{
            this->processes.insert(pid);
        }
        return pid;
    }catch(string error){
        throw error + " ProcessGenerator::spawnPassenger() ";
    }
}

//SpawnInspectors

int ProcessGenerator::beginSimulation(){
    int status;
    ShipInspector inspector;
    // inspector.behave(MAX_HARBOURS);
    //Logger *l = Logger::getInstance();

    //spawn passanger processes...
    //l->log("Spawn people process\n");
    try{
        Semaphore s(MAX_PASSENGERS, "/bin/cat",'A'); //???? no se si quiere esto o o A++? 

        //Create shared memory segment for passengers.
        // SharedMemoryPassenger passengersMem(SharedMemoryPassenger::shmFileName(), MAX_PASSENGERS);
        while(this->running()){
            s.wait();
            if(this->running()){
                if(spawnPassenger()==0){
                    s.signal();
                    return 0;
                }
            }
        }
        s.remove();

        //l->log("Signaling all child processes to end\n");
        cout << "Signaling all child processes to end\n";
        std::set<int>::iterator it;
        for (it=this->processes.begin(); it!=this->processes.end(); ++it){
            //signal all child processes to end in orderly fashion:
            kill(*it, SIGINT);
        }
        //l->log("Waiting for all child processes to end\n");
        cout << "Waiting for all child processes to end\n";
        size_t sz = this->processes.size();
        for (size_t i=0; i < sz; i++){
            //wait for all child processes to end:
            pid_t pid = wait(&status);
            this->processes.erase(pid);
        }

        //l->log("All child processes ended, now exiting main loop...\n");
        cout << "All child processes ended, now exiting main loop...\n";
        return 0;
    }catch(string error){
        throw error;
    }
}

ProcessGenerator::~ProcessGenerator(){
    cout<< getpid() <<"ProcessGenerator::~ProcessGenerator()" <<endl;

    this->processes.clear();
    vector<Harbour*>::iterator it;
    for (it=this->harbours.begin(); it!=this->harbours.end(); ++it){
        delete(*it);       
    }
    delete(this->passengersMem);
}
