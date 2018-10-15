#ifndef MEMORIACOMPARTIDA_H_
#define MEMORIACOMPARTIDA_H_

#include "Logger/Logger.h"
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string>
#include <string.h>
#include <iostream>
#include <errno.h>

#define LOG(str) std::cout<< __FILE__ <<":" << __LINE__ << ": " << str << std::endl

template <class T> class MemoriaCompartida {

private:
	int	shmId;
	T*	ptrDatos;
	size_t memsize;

	int	cantidadProcesosAdosados() const;

public:
	MemoriaCompartida();
	void crear(const std::string& archivo, const char letra, size_t memsize);
	void liberar();

	MemoriaCompartida(const std::string& archivo, const char letra, size_t memsize);
	MemoriaCompartida(const MemoriaCompartida& origen);
	~MemoriaCompartida();
	MemoriaCompartida<T>& operator= (const MemoriaCompartida& origen);
	void escribir(const T& dato, size_t pos);
	T leer(size_t pos) const;
	size_t size();

	// TODO: barrar, solo lo uso para debbuger
	int getMemId(){return shmId;}
};

template <class T> MemoriaCompartida<T>::MemoriaCompartida():shmId(0),ptrDatos(NULL),memsize(0){
}

template <class T> void MemoriaCompartida<T>::crear(const std::string& archivo, const char letra, size_t memsize) {
	key_t clave = ftok(archivo.c_str(), letra);

	if (clave > 0){
		this->shmId = shmget(clave, sizeof(T)*memsize, 0644|IPC_CREAT);

		if (this->shmId > 0) {
			void* tmpPtr = shmat(this->shmId, NULL, 0);

			if (tmpPtr != (void*) -1) {
				this->memsize = memsize;
				this->ptrDatos = static_cast<T*> (tmpPtr);
				// std::cout << "ATTACH "<< tmpPtr <<" ID:"<< this->shmId <<std::endl;
			}else{
				std::string mensaje = std::string("Error at shmat(): ") + std::string(strerror(errno));
				throw mensaje;
			}
		}else{
			std::string mensaje = std::string("Error at shmget(): ") + std::string(strerror(errno));
			throw mensaje;
		}
	}else{
		std::string mensaje = std::string("Error at ftok(): ") + std::string(strerror(errno));
		throw mensaje;
	}
}

template <class T> void MemoriaCompartida<T>::liberar(){
	int errorDt = shmdt((void *) this->ptrDatos);

	if (errorDt != -1){
		int procAdosados = this->cantidadProcesosAdosados();
		if (procAdosados == 0) {
			shmctl(this->shmId, IPC_RMID, NULL);
		}
	}else{
		std::string mensaje = std::string("Error at shmdt(): ") + std::string(strerror(errno));
		throw mensaje;
	}
}

template <class T> size_t MemoriaCompartida<T>::size(){
	return this->memsize;
}

template <class T> MemoriaCompartida<T>::MemoriaCompartida(const std::string& archivo, const char letra, size_t memsize):shmId(0),ptrDatos(NULL),memsize(0) {
	key_t clave = ftok(archivo.c_str(), letra);

	if (clave > 0) {
		this->shmId = shmget(clave, sizeof(T)*memsize, 0644|IPC_CREAT);

		if (this->shmId > 0) {
			void* tmpPtr = shmat(this->shmId, NULL, 0);
			if ( tmpPtr != (void*) -1 ) {
				this->memsize = memsize;
				this->ptrDatos = static_cast<T*> (tmpPtr);
			} else {
				std::string mensaje = std::string("Error at shmat(): ") + std::string(strerror(errno));
				throw mensaje;
			}
		} else {
			std::string mensaje = std::string("Error at shmget(): ") + std::string(strerror(errno));
			throw mensaje;
		}
	} else {
		std::string mensaje = std::string("Error at ftok(): ") + std::string(strerror(errno));
		throw mensaje;
	}
}

template <class T> MemoriaCompartida<T>::MemoriaCompartida(const MemoriaCompartida& origen):shmId(origen.shmId) {
	void* tmpPtr = shmat(origen.shmId, NULL, 0);

	if (tmpPtr != (void*) -1) {
		this->memsize = origen.size();
		this->ptrDatos = static_cast<T*> (tmpPtr);
	} else {
		std::string mensaje = std::string("Error at shmat(): ") + std::string(strerror(errno));
		throw mensaje;
	}
}

template <class T> MemoriaCompartida<T>::~MemoriaCompartida(){
	int errorDt = shmdt(static_cast<void*> (this->ptrDatos));
	// std::cout << "DETTACH "<< static_cast<void*> (this->ptrDatos)<<" ID:"<< this->shmId  <<std::endl;
	if (errorDt != -1){
		int procAdosados = this->cantidadProcesosAdosados();
		if (procAdosados == 0) {
			shmctl(this->shmId, IPC_RMID, NULL);
		}
	}else{
		std::string mensaje = std::string(strerror(errno));
		Logger::getInstance().log(mensaje);
	}

}

template <class T> MemoriaCompartida<T>& MemoriaCompartida<T>::operator= (const MemoriaCompartida& origen){
	this->shmId = origen.shmId;
	void* tmpPtr = shmat(this->shmId, NULL, 0);

	if (tmpPtr != (void*) -1) {
		this->memsize = origen.size();
		this->ptrDatos = static_cast<T*> (tmpPtr);
	} else {
		std::string mensaje = std::string("Error at shmat(): ") + std::string(strerror(errno));
		throw mensaje;
	}
	return *this;
}

template <class T> void MemoriaCompartida<T>::escribir(const T& dato, size_t pos) {
	if (pos > this->memsize){
		std::string mensaje = std::string("Attempted to write beyond shared memory limit! Pos: ") + std::to_string(pos) + std::string(" Limit: ") + std::to_string(memsize);
		throw mensaje;
	}
	this->ptrDatos[pos] = dato;
}

template <class T> T MemoriaCompartida<T>::leer(size_t pos) const {
	if (pos > this->memsize){
		std::string mensaje = std::string("Attempted to read beyond shared memory limit! Pos: " + std::to_string(pos) + std::string(" Limit: ") + std::to_string(memsize));
		throw mensaje;
	}
	return this->ptrDatos[pos];
}

template <class T> int MemoriaCompartida<T> :: cantidadProcesosAdosados() const {
	shmid_ds estado;
	shmctl(this->shmId, IPC_STAT, &estado);
	return estado.shm_nattch;
}

#endif
