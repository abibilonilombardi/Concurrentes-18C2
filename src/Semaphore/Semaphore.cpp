#include "../Semaphore/Semaphore.h"
// #include "../Lock/ExclusiveLock.h"
#include "../Logger/Logger.h"

Semaphore::Semaphore(const int &initialValue, const string& pathname, const char letter):initialValue(initialValue),
pathname(pathname){
    this->checkingInitialValue();
    // creat(pathname.c_str(), 0644);
    createKey(pathname, letter);

    //lockeo la creacion e inicializacion para que sea atomica
    // ExclusiveLock l("lockcreacioneinicializacionSemaforo.txt");
    this->setId = semget(this->key, 1, 0666|IPC_CREAT); //TODO: ver con ipc_excl si lockear o los creamos en el proceso creador
    if (setId == -1){
        throw "Error al crear al semaforo";
    }
    this->initialize(); // depende de lo anterior si tiene sentido asi o publico
    // l.unlock();

}

void Semaphore::createKey(const string& pathname, const char letter){
    this->key = ftok(pathname.c_str(), letter);
    if (this->key == -1){
        throw "No pudo crear la clave del semaforo para el pathname: " + pathname;
    }
}

void Semaphore::checkingInitialValue(){
    if (this->initialValue < 0){
        throw "Valor inicial del semáforo inválido!";
    }
}

void Semaphore::initialize(){
    union semun {
        int val;
        struct semid_ds *buf;
        unsigned short  *array;
    } initval;

    initval.val = this->initialValue;
    if (semctl(this->setId, 0, SETVAL, initval) < 0){
        throw "error en setVal";
    }
}

void Semaphore::wait(){
    struct sembuf semOperation;
    semOperation.sem_num = 0;
    semOperation.sem_op = -1;
    semOperation.sem_flg = SEM_UNDO;

    if (semop(this->setId, &semOperation, 1) < 0 && (errno != EINTR)){
        Logger::getInstance().log(to_string(getpid())+": ERROR AT WAIT!!!");
        throw string("Error at wait! ")+to_string(getpid())+"-"+string(to_string(errno));
    }

    // Logger::getInstance().log(to_string(getpid())+" Semaforo key: "+ to_string(this->key)+ " wait call");
}

void Semaphore::signal(){
    struct sembuf semOperation;
    semOperation.sem_num = 0;
    semOperation.sem_op = 1;
    semOperation.sem_flg = SEM_UNDO;

    if (semop(this->setId, &semOperation, 1) < 0 && (errno != EINTR)){
        throw "Error at signal!";
    }
    // Logger::getInstance().log(to_string(getpid())+" Semaforo key: "+ to_string(this->key)+ " signal call");
}

int Semaphore::getId(){
    return this->setId;
}

void Semaphore::remove(){
    semctl(this->setId, 0, IPC_RMID);
}

Semaphore::~Semaphore(){
    // unlink(this->pathname.c_str());
}
