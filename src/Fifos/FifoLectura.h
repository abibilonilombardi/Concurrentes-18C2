#ifndef FIFOLECTURA_H_
#define FIFOLECTURA_H_

#include "Fifo.h"

class FifoLectura : public Fifo {
public:
	FifoLectura(const std::string nombre);
	~FifoLectura();

	void abrir();
	void leer(void* buffer,const ssize_t buffsize) const;

	int leerId() const;

};

#endif /* FIFOLECTURA_H_ */
