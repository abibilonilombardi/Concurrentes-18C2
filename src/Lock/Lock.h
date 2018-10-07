#ifndef LOCK_H_
#define LOCK_H_

#include <iostream>  // para debuguear

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <string.h>
class Lock {
private:
    struct flock lock;
    int fd;
    std::string fileName;
    bool isInternFile;
public:
    Lock(const std::string& file);
    Lock(const std::string& file, size_t start, size_t end);
    Lock(int fileDescriptor);
    ~Lock();
private:
    void initializeFlock(size_t start, size_t end);
    void openFile(const std::string file);
    void closeFile();
};


#endif /* LOCK_H_ */
