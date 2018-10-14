#include "FifoEscritura.h"
#include <iostream>

FifoEscritura::FifoEscritura(const std::string nombre) : Fifo(nombre) {
}

FifoEscritura::~FifoEscritura() {
}

void FifoEscritura::abrir() {
	fd = open ( nombre.c_str(),O_WRONLY , 0644);
	std::cout<<"SE ABRIO EL FIFO "<< nombre<<" PARA ESCRITURA"<<std::endl;
}

ssize_t FifoEscritura::escribir(const void* buffer,const ssize_t buffsize) const {
	return write ( fd,buffer,buffsize ); //ATOMICA
}
