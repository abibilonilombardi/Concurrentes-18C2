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
#include <set>

using namespace std;

class SharedMemoryPassenger: public MemoriaCompartida<int> {
private:
	int fd;
	size_t getStartingPosition(int passengerId);
	static size_t maxPassengers;
	int getFreeId();
public:
    SharedMemoryPassenger(int maxPassengers);
    SharedMemoryPassenger();
    void initialize();
    int addPassenger(int location, int nextStop, bool hasTicket);
    void updateLocation(int passengerId, int location);
    int getLocation(int passengerId);

	void freePassengerId(int passengerId);
    void updateNextStop(int passengerId, int nextStop);
    int getNextStop(int passengerId);
    bool hasTicket(int passengerId);

	void getPassangersForDestination(set<int> passengerList, int destination);

	static string shmFileName();
	static string shmLockName();
    ~SharedMemoryPassenger();
};

#endif //SHMMAP_H
