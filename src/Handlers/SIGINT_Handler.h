#ifndef SIGINT_HANDLER
#define SIGINT_HANDLER

#include <assert.h>
#include <iostream>


class SIGINT_Handler : public EventHandler {
private:
	sig_atomic_t gracefulQuit;
public:
    SIGINT_Handler():gracefulQuit(0) {}

    ~SIGINT_Handler() {}

    virtual int handleSignal(int signum) {
        if( signum != SIGINT ){return -1;}
        this->gracefulQuit = 1;
        return 0;
    }

    sig_atomic_t getGracefulQuit() const {
        return this->gracefulQuit;
    }

};

#endif //SIGINT_HANDLER
