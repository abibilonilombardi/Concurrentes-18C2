#include <stdlib.h>
#include <stdio.h>
#include <iostream>

#include "Handlers/SignalHandler.h"

using namespace std;


int main(int argc, char *argv[]){
    int cant_barcos = atoi(argv[1]);
    int capacidad_barcos = atoi(argv[2]);
	cout << "Hello World "<<cant_barcos<<","<<capacidad_barcos<<"\n";
    return 0;
}
