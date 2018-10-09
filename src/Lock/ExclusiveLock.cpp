#include "ExclusiveLock.h"
#include <string.h>
#include "errno.h"

void ExclusiveLock::initializeFlock(){
    lock.l_type = F_WRLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = 0;
    lock.l_len = 0;
}

void ExclusiveLock::openFile(const std::string file){
    fd = open(file.c_str(), O_CREAT|O_WRONLY, 0666);
    if (fd == -1) {
        // std::cout<<"ExclusiveLock::openFile " << file << "  " << fd << std::string(strerror(errno))<<std::endl;
        throw "ExclusiveLock::openFile " + std::string(strerror(errno));
    }
}

void ExclusiveLock::closeFile(){
    fd = close(fd);
    if(fd == -1){
        throw "ExclusiveLock::closeFile " + std::string(strerror(errno));
    }
}

ExclusiveLock::ExclusiveLock(int fileDescriptor):isInternalFile(false){
    initializeFlock();
    fd = fileDescriptor;
    int result = fcntl(fd,F_SETLKW, &lock);
    if(result == -1){
        throw "ExclusiveLock::ExclusiveLock(int fileDescriptor) " + std::string(strerror(errno));
    }
    //std::cout << "ExclusiveLock el proceso "<< getpid() << " TOMO lock " << std::endl;
}

ExclusiveLock::ExclusiveLock(const std::string& file):isInternalFile(true){
    openFile(file);
    initializeFlock();
    int result = fcntl(fd,F_SETLKW, &lock);
    if(result == -1){
        throw "ExclusiveLock::ExclusiveLock(const std::string& file) " + std::string(strerror(errno));
    }
}

void ExclusiveLock::unlock(){
    lock.l_type = F_UNLCK;
    //std::cout << "ExclusiveLock el proceso "<< getpid() << " LIBERO lock " << std::endl;
    int result = fcntl(fd, F_SETLK, &lock);
    if(result == -1){
        throw "ExclusiveLock::unlock() " + std::string(strerror(errno));
    }
}

ExclusiveLock::~ExclusiveLock(){
    if (isInternalFile){
        closeFile();
    }
}
