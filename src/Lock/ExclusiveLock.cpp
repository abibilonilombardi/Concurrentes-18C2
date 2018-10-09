#include "ExclusiveLock.h"


void ExclusiveLock::initializeFlock(){
    lock.l_type = F_WRLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = 0;
    lock.l_len = 0;
}

void ExclusiveLock::openFile(const std::string file){
    fd = open(file.c_str(), O_CREAT|O_WRONLY);
    if (fd == -1) {
        //exception
    }
}

void ExclusiveLock::closeFile(){
    fd = close(fd);
    if(fd == -1){
        //exception
    }
}

ExclusiveLock::ExclusiveLock(int fileDescriptor):isInternalFile(false){
    initializeFlock();
    fd = fileDescriptor;
    fcntl(fd,F_SETLKW, &lock);
    //std::cout << "ExclusiveLock el proceso "<< getpid() << " TOMO lock " << std::endl;
}

ExclusiveLock::ExclusiveLock(const std::string& file):isInternalFile(true){
    openFile(file);
    initializeFlock();
    fcntl(fd,F_SETLKW, &lock);
}

void ExclusiveLock::unlock(){
    lock.l_type = F_UNLCK;
    //std::cout << "ExclusiveLock el proceso "<< getpid() << " LIBERO lock " << std::endl;
    fcntl(fd, F_SETLK, &lock);
}

ExclusiveLock::~ExclusiveLock(){
    if (isInternalFile){
        closeFile();
    }
}
