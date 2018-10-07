#include "MemoriaCompartida.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#ifndef SHMPASSENGER_H
#define SHMPASSENGER_H

class SharedMemoryPassenger: public MemoriaCompartida<int> {
private:
	int fd;
	size_t getStartingPosition(int passengerId);
	size_t maxPassengers;
	std::string pathname;
public:
    SharedMemoryPassenger(std::string pathname, int maxPassengers);
    void addPassenger(int passengerId, int location, int nextStop, bool hasTicket);
    void updateLocation(int passengerId, int location);
    int getLocation(int passengerId);
    void updateNextStop(int passengerId, int nextStop);
    int getNextStop(int passengerId);
    bool hasTicket(int passengerId);
    ~SharedMemoryPassenger();
};

#endif //SHMMAP_H
