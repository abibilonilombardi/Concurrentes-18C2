#ifndef SHAREDLOCK_H_
#define SHAREDLOCK_H_

#include <iostream>  // para debuguear

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>


class SharedLock {
private:
    struct flock lock;
    int fd;
    bool isInternalFile;
public:
    SharedLock(const std::string& file);
    SharedLock(int fileDescriptor);
    void unlock();
    ~SharedLock();   
private:
    void initializeFlock(); 
    void openFile(const std::string file);
    void closeFile();
};


#endif /* SHAREDLOCK_H_ */
