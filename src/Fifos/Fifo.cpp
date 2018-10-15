#include "Fifo.h"
#include "errno.h"
#include <string.h>
#include "Logger/Logger.h"


Fifo::Fifo(const std::string nombre) : nombre(nombre), fd(-1) {
	mknod ( static_cast<const char*>(this->nombre.c_str()), S_IFIFO|0666,0 );
	// if (result==-1){
	// 	throw std::string("Error Fifo::Fifo(const std::string nombre)") + std::string(strerror(errno));
	// }
	// Logger::getInstance().log(" FIFO  "+ nombre + " creado");  
}

void Fifo::cerrar() {
	if(this->fd != -1){
		close ( this->fd );
		this->fd = -1;
	}
}

void Fifo::eliminar() const {
	unlink(this->nombre.c_str() );
	// Logger::getInstance().log(" FIFO  "+ nombre + " eliminado"); 
}

Fifo::~Fifo() {
}