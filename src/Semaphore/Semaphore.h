#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <errno.h>
#include <string.h>
#include <iostream>

using namespace std;

#ifndef SEMAPHORE_H
#define SEMAPHORE_H

class Semaphore {
    key_t key;
    int setId;

public:
    Semaphore(const string &pathname, const char letter);
    int getId();
    void initialize(int value);
    void p();
    void v();
    ~Semaphore();

};

#endif //SEMAPHORE_H
