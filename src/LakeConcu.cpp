#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <iostream>
#include <cstring>

#include "ProcessGenerator.h"

using namespace std;

int main(int argc, char *argv[]){
    int shipQty, shipCty;
    try{
        int createLogFile = open("ShipsTrevelingSimulation.log", O_CREAT, 0666);
        close(createLogFile);

        if (argc == 3){ //TODO SACAR
            shipQty = atoi(argv[1]);
            shipCty = atoi(argv[2]);
        }
        else{
            shipQty = 3;
            shipCty = 5;
            // return -1;
        }
        std::cout<<getpid() << " PARAMETROS cantidadBarcos:"<<shipQty << " capacidadBarcos:"<< shipCty<< std::endl;
        ProcessGenerator pc;

        // spawn ship processes
        if (pc.spawnShips(shipQty, shipCty) == 0){
            return 0;
        }

        if (pc.spawnShipInspector() == 0){
           return 0;
        }

        if (pc.spawnTicketInspector() == 0){
           return 0;
        }

        //begin simulation (spawn people processes)
        pc.beginSimulation();
        return 0;
    }catch(const string &error){
        cout<< error<<endl;
        return -1;
    }
    catch(char* error){
        cout<< error<<endl;
        return -1;
    }
    catch(char const* error){
        cout<< error<<endl;
        return -1;
    }
    catch(...){
        cout << "Catch vacio" << endl;
    }
}
