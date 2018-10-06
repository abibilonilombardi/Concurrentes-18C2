#ifndef LOCK_H_
#define LOCK_H_

#include <iostream>  // para debuguear

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>


class Lock {
private:
    struct flock lock;
    int fd;
public:
    Lock(int fileDescriptor);
    ~Lock();   
private:
    void initializeFlock(); 
};


#endif /* LOCK_H_ */
