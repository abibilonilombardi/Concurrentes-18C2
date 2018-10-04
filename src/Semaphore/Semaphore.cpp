#include "Semaphore.h"

Semaphore::Semaphore(const string& pathname, const char letter){
    this->key = ftok(pathname.c_str(), letter);
    //Get semaphore if it exists, otherwise create it:
    this->setId = semget(this->key, 1, 0644|IPC_CREAT);
}

void Semaphore::initialize(int value){
    //Check if initialization value is valid:
    if (value <= 0){
        string msg = string("Error initialized semaphore with invalid value!");
        msg += string(strerror(errno));
        throw(msg);
    }
    //Initialize semaphore 0 from set with id 'setId'
    //to value 'value'.
    union semun {
        int val;
        struct semid_ds *buf;
        unsigned short  *array;
    } initval;
    initval.val = value;
    if (semctl(this->setId, 0, SETVAL, initval) < 0){
        std::string msg = string("Error initializing semaphore at semctl!");
        msg += string(strerror(errno));
        throw(msg);
    }
}

void Semaphore::p(){
    struct sembuf semOperation;
    semOperation.sem_num = 0; //Apply operation to semaphore 0 of the set.
    semOperation.sem_op = -1; //Substract 1 from sem_val or block until substraction is viable.
    semOperation.sem_flg = SEM_UNDO; //Undo operation in case of termination.
    //Perform one operation ('semOperation') on set with id 'setId'.
    if (semop(this->setId, &semOperation, 1) < 0){
        string msg = string("Error at semaphore.p()!");
        msg += string(strerror(errno));
        throw(msg);
    }
}

void Semaphore::v(){
    struct sembuf semOperation;
    semOperation.sem_num = 0; //Apply operation to semaphore 0 of the set.
    semOperation.sem_op = 1; //Add 1 to sem_val.
    semOperation.sem_flg = SEM_UNDO; //Undo operation in case of termination.
    //Perform one operation ('semOperation') on set with id 'setId'.
    if (semop(this->setId, &semOperation, 1) < 0){
        string msg = string("Error at semaphore.v()!");
        msg += string(strerror(errno));
        throw(msg);
    }
}

int Semaphore::getId(){
    return this->setId;
}

Semaphore::~Semaphore(){
    //Remove ALL semaphores associated to
    //the set with id 'setId'.
    semctl(this->setId, 0, IPC_RMID);
}
