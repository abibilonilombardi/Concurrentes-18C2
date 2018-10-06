#include "Lock.h"


void Lock::initializeFlock(){
    lock.l_type = F_WRLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = 0;
    lock.l_len = 0;
}

void Lock::openFile(const std::string file){
    fd = open(file.c_str(), O_CREAT|O_RDWR);
    if (fd == -1) {
        //exception
    }
}

void Lock::closeFile(){
    fd = close(fd);
    if(fd == -1){
        //exception
    }      
}

Lock::Lock(int fileDescriptor):isInternFile(true){
    std::cout << "el proceso "<< getpid()<< " bloqueo archivo" << std::endl;
    initializeFlock();
    fd = fileDescriptor;
    fcntl(fd,F_SETLKW, &lock);
}

Lock::Lock(const std::string& file):isInternFile(true){
    openFile(file);
    initializeFlock();
    fcntl(fd,F_SETLKW, &lock);
}

Lock::~Lock(){
    std::cout << "el proceso "<< getpid() << " desbloqueo archivo " << std::endl;
    lock.l_type = F_UNLCK;
    fcntl(fd, F_SETLKW, &lock);
    if (isInternFile){
        closeFile();
    }
}