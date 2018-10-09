#ifndef SHMPASSENGER_H
#define SHMPASSENGER_H

#include "MemoriaCompartida.h"
#include "Lock/ExclusiveLock.h"
#include "Lock/SharedLock.h"
#include "Harbour.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

using namespace std;


class SharedMemoryPassenger: public MemoriaCompartida<int> {
private:
	int fd;
	size_t getStartingPosition(int passengerId);
	static size_t maxPassengers;
	std::string pathname;

	int getFreeId();
public:
    SharedMemoryPassenger(const std::string pathname, int maxPassengers);
    SharedMemoryPassenger(const std::string pathname);
    int addPassenger(int location, int nextStop, bool hasTicket);
    void updateLocation(int passengerId, int location);
    int getLocation(int passengerId);

	void freePassengerId(int passengerId);
    void updateNextStop(int passengerId, int nextStop);
    int getNextStop(int passengerId);
    bool hasTicket(int passengerId);
	static string shmFileName();
	static string shmLockName();
    ~SharedMemoryPassenger();
};

#endif //SHMMAP_H
