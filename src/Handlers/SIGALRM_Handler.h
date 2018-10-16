#include <assert.h>
#include <iostream>

#ifndef SIGALRM_HANDLER
#define SIGALRM_HANDLER
#include "Logger/Logger.h"

class SIGALRM_Handler : public EventHandler {
private:
	sig_atomic_t alarmValue;
public:
    SIGALRM_Handler():alarmValue(0) {}

    ~SIGALRM_Handler() {}

    virtual int handleSignal(int signum) {
        if( signum != SIGALRM ) {
			return -1;
		}
        this->alarmValue = 1;
        return 0;
    }

    sig_atomic_t getActiveAlarm()  const{
        return this->alarmValue;
    }

    bool isActivate() {
        return this->alarmValue == 1;
    }

    void restartAlarm() {
        this->alarmValue = 0;
    }
};

#endif //SIGALRM_HANDLER
