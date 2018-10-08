#include <iostream>
#include <unistd.h>
#include <sys/types.h>

#include "../Logger/Logger.h"
#include "../Logger/LogMessages.h"  //ver si es h o cpp



using namespace std;

/*int main () {

    Logger* logger = Logger::getInstance();

    pid_t pid = getpid();

    for (int i; i<10; i++){
        if (pid!=0)
            pid = fork();
    }

    cout << "pid: "<< getpid() << endl; 
    logger->log(CREACION_BARCO_EXITO);


    Logger::destroy();
    cout << "pid: "<< getpid()<<" MURIO" << endl; 
	return 0;
}*/
