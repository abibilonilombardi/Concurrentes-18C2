#ifndef EXCLUSIVELOCK_H_
#define EXCLUSIVELOCK_H_

#include <iostream>  // para debuguear

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>


class ExclusiveLock {
private:
    struct flock lock;
    int fd;
    bool isInternalFile;
public:
    ExclusiveLock(const std::string& file);
    ExclusiveLock(int fileDescriptor);
    void unlock();
    ~ExclusiveLock();   
private:
    void initializeFlock(); 
    void openFile(const std::string file);
    void closeFile();
};


#endif /* EXCLUSIVELOCK_H_ */
