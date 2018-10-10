#include <assert.h>
#include <iostream>

#ifndef SIGALRM_HANDLER
#define SIGALRM_HANDLER

class SIGALRM_Handler : public EventHandler {
private:
	sig_atomic_t activeAlarm;
public:
    SIGALRM_Handler():activeAlarm(0) {}

    ~SIGALRM_Handler() {}

    virtual int handleSignal(int signum) {
        assert ( signum == SIGALRM );
        this->activeAlarm = 1;
        return 0;
    }

    sig_atomic_t getActiveAlarm() const {
        return this->activeAlarm;
    }

    void initilizeAlarm(){
        this->activeAlarm = 0;
    }
};

#endif //SIGALRM_HANDLER
