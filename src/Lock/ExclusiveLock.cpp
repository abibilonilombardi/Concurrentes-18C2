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
    this->fd = open(file.c_str(), O_CREAT|O_WRONLY, 0644);
    if (this->fd == -1) {
        // std::cout<<"ExclusiveLock::openFile " << file << "  " << this->fd << std::string(strerror(errno))<<std::endl;
        throw "ExclusiveLock::openFile file: " + file + " " + std::string(strerror(errno));
    }
}

void ExclusiveLock::closeFile(){
    this->fd = close(this->fd);
    if(this->fd == -1){
        throw "ExclusiveLock::closeFile " + std::string(strerror(errno));
    }
}

ExclusiveLock::ExclusiveLock(int fileDescriptor):isInternalFile(false){
    initializeFlock();
    this->fd = fileDescriptor;
    int result = fcntl(this->fd,F_SETLKW, &lock);
    if(result == -1 && (errno!=EINTR)){
        throw "ExclusiveLock::ExclusiveLock(int fileDescriptor) " + std::string(strerror(errno));
    }
    //std::cout << "ExclusiveLock el proceso "<< getpid() << " TOMO lock " << std::endl;
}

ExclusiveLock::ExclusiveLock(const std::string& file):isInternalFile(true){
    // std::cout << "Por lockear archivo: " << file << std::endl;
    openFile(file);
    initializeFlock();
    int result = fcntl(this->fd,F_SETLKW, &lock);
    if(result == -1 && (errno!=EINTR)){
        throw "ExclusiveLock::ExclusiveLock(const std::string& file) " + std::string(strerror(errno));
    }
}

int ExclusiveLock::getfd(){
    return this->fd;
}

void ExclusiveLock::unlock(){
    lock.l_type = F_UNLCK;
    //std::cout << "ExclusiveLock el proceso "<< getpid() << " LIBERO lock " << std::endl;
    int result = fcntl(this->fd, F_SETLK, &lock);
    if(result == -1){
        throw "ExclusiveLock::unlock() " + std::string(strerror(errno));
    }
}

ExclusiveLock::~ExclusiveLock(){
    if (isInternalFile){
        closeFile();
    }
}
