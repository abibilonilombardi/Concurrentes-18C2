#include "Lock.h"


void Lock::initializeFlock(){
    lock.l_type = F_WRLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = 0;
    lock.l_len = 0;
}

Lock::Lock(int fileDescriptor){
    std::cout << "el proceso "<< getpid()<< " bloqueo archivo" << std::endl;
    initializeFlock();
    fd = fileDescriptor;
    fcntl(fd,F_SETLKW, &lock);
}

Lock::~Lock(){
    std::cout << "el proceso "<< getpid() << " desbloqueo archivo " << std::endl;
    lock.l_type = F_UNLCK;
    fcntl(fd, F_SETLKW, &lock);
}