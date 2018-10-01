#include <assert.h>
#include <iostream>

#ifndef SIGINT_HANDLER
#define SIGINT_HANDLER

class SIGINT_Handler : public EventHandler {
private:
	sig_atomic_t gracefulQuit;
public:
    SIGINT_Handler():gracefulQuit(0) {}

    ~SIGINT_Handler() {}

    virtual int handleSignal(int signum) {
        assert ( signum == SIGINT );
        this->gracefulQuit = 1;
        return 0;
    }

    sig_atomic_t getGracefulQuit() const {
        return this->gracefulQuit;
    }

};

#endif //SIGINT_HANDLER
