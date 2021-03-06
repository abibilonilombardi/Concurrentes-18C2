#ifndef SEMAPHORE_H
#define SEMAPHORE_H

#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <errno.h>
#include <string.h>
#include <iostream>

using namespace std;

class Semaphore {
    key_t key;
    int setId;
    int initialValue;
    const string &pathname;
public:
    Semaphore(const int &initialValue, const string &pathname, const char letter);
    int getId();
    void wait();
    void signal();
    void remove();
    ~Semaphore();
private:
    void createKey(const string& pathname, const char letter);
    void initialize();
    void checkingInitialValue();
};

#endif //SEMAPHORE_H
