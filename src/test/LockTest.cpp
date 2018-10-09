#include <iostream>
#include <unistd.h>
#include <sys/types.h>

#include "../Lock/SharedLock.h"
#include "../Lock/ExclusiveLock.h"
// PRUEBA LOCK DE LECTURA

using namespace std;

void testLocks(pid_t pid){
    if (pid%2 == 0){
        SharedLock lock("pruebaSharedLock.txt");

        for (int i=0 ; i< 5; i++){
            sleep(1);
            cout << "pid: "<< getpid() << " LECTURA"<< i << endl;
        }

        lock.unlock();
    }
    else{
        ExclusiveLock lock("pruebaSharedLock.txt");

        for (int i=0 ; i< 5; i++){
            sleep(1);
            cout << "pid: "<< getpid() << " ESCRITURA"<< i << endl;
        }

        lock.unlock();
    }
}

/*int main () {

    pid_t pid = getpid();


    for (int i; i<100; i++){
        if (pid!=0)
            pid = fork();
    }

    testLocks(getpid());

	return 0;
}*/
