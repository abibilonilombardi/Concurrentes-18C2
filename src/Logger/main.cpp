#include <iostream>
#include <unistd.h>
#include <sys/types.h>

#include "Logger.h"
#include "LogMessages.cpp"  //ver si es h o cpp



using namespace std;

/*int main () {

    Logger* logger = Logger::getInstance();

    pid_t pid = getpid();

    for (int i; i<1000; i++){
        if (pid!=0)
            pid = fork();
    }

    cout << "pid: "<< getpid() << endl; 
    logger->log(CREACION_BARCO_EXITO);

    sleep(5);

    Logger::destroy();

	return 0;
}*/
