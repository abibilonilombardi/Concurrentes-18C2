#include "Lock.h"


void Lock::initializeFlock(size_t start, size_t end){
    this->lock.l_type = F_WRLCK;
    this->lock.l_whence = SEEK_SET;
    this->lock.l_start = start;
    this->lock.l_len = end;
}

void Lock::openFile(const std::string file){
    this->fileName = file;
    this->fd = open(this->fileName.c_str(), O_CREAT|O_WRONLY, 0777);
    if (this->fd == -1) {
        //exception
        std::cout << "ERROR creating lock file\n";
    }
}

void Lock::closeFile(){
    if(close(this->fd) == -1){
        //exception
    }
}

Lock::Lock(int fileDescriptor):isInternFile(true){
    //std::cout << "el proceso "<< getpid()<< " bloqueo archivo" << std::endl;
    initializeFlock(0, 0);
    this->fd  = fileDescriptor;
    fcntl(this->fd,F_SETLKW, &lock);
}

Lock::Lock(const std::string& file):isInternFile(true){
    //Lock the WHOLE FILE
    initializeFlock(0,0);
	openFile(file);
    fcntl(this->fd, F_SETLKW, &(this->lock));
}

Lock::Lock(const std::string& file, size_t start, size_t end){
    initializeFlock(start, end);
    openFile(file);
    //Lock file between positions 'start' and 'end'.
    fcntl(this->fd, F_SETLKW, &(this->lock));
}

Lock::~Lock(){
    //std::cout << "el proceso "<< getpid() << " desbloqueo archivo " << std::endl;
    lock.l_type = F_UNLCK;
    fcntl(this->fd, F_SETLKW, &lock);
    closeFile();
    //if (isInternFile){
    //    closeFile();
    //}
}
