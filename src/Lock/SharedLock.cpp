#include "SharedLock.h"


void SharedLock::initializeFlock(){
    lock.l_type = F_RDLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = 0;
    lock.l_len = 0;
}

void SharedLock::openFile(const std::string file){
    fd = open(file.c_str(), O_RDONLY);
    if (fd == -1) {
        //exception
    }
}

void SharedLock::closeFile(){
    fd = close(fd);
    if(fd == -1){
        //exception
    }
}

SharedLock::SharedLock(int fileDescriptor):isInternalFile(false){
    initializeFlock();
    fd = fileDescriptor;
    fcntl(fd,F_SETLKW, &lock);
    //std::cout << "SharedLock el proceso "<< getpid() << " TOMO lock " << std::endl;
}

SharedLock::SharedLock(const std::string& file):isInternalFile(true){
    openFile(file);
    initializeFlock();
    fcntl(fd,F_SETLKW, &lock);
}

void SharedLock::unlock(){
    lock.l_type = F_UNLCK;
    //std::cout << "SharedLock el proceso "<< getpid() << " LIBERO lock " << std::endl;
    fcntl(fd, F_SETLK, &lock);
}

SharedLock::~SharedLock(){
    if (isInternalFile){
        closeFile();
    }
}
