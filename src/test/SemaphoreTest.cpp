// #include <iostream>
// #include <unistd.h>
// #include <stdlib.h>
// #include <sys/wait.h>
// #include <sys/ipc.h>
// #include <sys/shm.h>
// #include <sys/types.h>
// #include <sys/wait.h>

// #include "../Semaphore/Semaphore.h"

// using namespace std;

// int calcularRandom ();

// int main () {
//     // const std::string FILEFTOK("/bin/bash");
//     // const char a('A'), b('B'), c('C';) 
//     const int whiletrue = 12;
//     try{
//     Semaphore semaphoreP(4,"/bin/bash",'A');
//     Semaphore semaphoreC(0,"/bin/bash",'B');
    
//     key_t key = ftok("/bin/bash", 'C');
//     int memID = shmget (key, sizeof(int)*4, 0664|IPC_CREAT );
//     int* memory = (int*)shmat ( memID, NULL, 0 );

//     cout << "CREO LA MEMORIA COMPARTIDA "<< endl;

//     pid_t pid = fork();
    
//     if (pid == 0){     
//         for(int i=0; i<whiletrue; i++){
//             semaphoreC.wait();
//             int value = memory[i%4];
//             cout << "CONSUMIDOR pid: "<< getpid() << " CONSUME la pos "<< i%4 <<" el valor "<<value << endl; 
//             sleep(2);
//             semaphoreP.signal();
//         } 
//         shmdt ( memory ); // dejo de usar la MC
//         exit(0);
//     }
//     else {
//         for(int i=0; i<whiletrue ; i++){
//             semaphoreP.wait();
//             int value = calcularRandom ();
//             memory[i%4] = value;
//             cout << "PRODUCTOR pid: "<< getpid() << " PRODUCE la pos "<< i%4 <<" el valor "<<value << endl; 
//             sleep(1);
//             semaphoreC.signal();
//         } 

//         shmdt ( memory ); // dejo de usar la MC
        
//         // tengo que esperar a que termine el hijo para eliminar las MC y los Sem
//         int pp;
//         wait(&pp);

//         semaphoreC.remove();
//         semaphoreP.remove();

//         shmctl ( memID, IPC_RMID, NULL );
//         cout << getpid() <<" DESTRUYO LA MEMORIA COMPARTIDA "<< endl;
//     }

//     return 0;
//     }

//     catch(const char* err){
//         cout<< err<< endl;
//     }
//     catch(const std::string err){
//         cout<< err<< endl;
//     }
// }

// int calcularRandom () {
// 	srand ( time(NULL) );
// 	int resultado = rand() % 10 + 1;
// 	return resultado;
// }
