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
    bool isInternFile;
public:
    Lock(const std::string& file);
    Lock(int fileDescriptor);
    ~Lock();   
private:
    void initializeFlock(); 
    void openFile(const std::string file);
    void closeFile();
};


#endif /* LOCK_H_ */
