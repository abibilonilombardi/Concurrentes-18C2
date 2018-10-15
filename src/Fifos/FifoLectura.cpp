#include "FifoLectura.h"
#include "errno.h"
#include <string.h>
#include <iostream>

FifoLectura::FifoLectura(const std::string nombre) : Fifo(nombre) {
}

FifoLectura::~FifoLectura() {
}

void FifoLectura::abrir() {
	this->fd = open( this->nombre.c_str(),O_RDONLY ,0644);
	if(this->fd == -1){
		// throw std::string("Error FifoLectura::abrir() ") + std::string(strerror(errno));
	}
}

void FifoLectura::leer(void* buffer,const ssize_t buffsize) const {
	ssize_t readedBytes = 0;
    while( readedBytes < buffsize){
        readedBytes += read(fd, (char*)&buffer + readedBytes , sizeof(buffer) - readedBytes);
        if(readedBytes == -1){throw std::string("Error FifoLectura::leer") + std::string(strerror(errno));}
	}
}


int FifoLectura::leerId() const {
	int id = -1;
	ssize_t readBytes = 0;
    while( readBytes < (ssize_t)sizeof(id)){
        readBytes += read(this->fd, (char*)&id + readBytes , sizeof(int) - readBytes);
        if (readBytes==-1){
			return -1;
		}
	}
	return id;
}
