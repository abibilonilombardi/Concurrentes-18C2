#ifndef SIGPIPE_HANDLER
#define SIGPIPE_HANDLER

#include <assert.h>
#include <iostream>

class SIGPIPE_Handler : public EventHandler {
private:
	sig_atomic_t failedBoard;
public:
    SIGPIPE_Handler():failedBoard(0) {}

    ~SIGPIPE_Handler() {}

    virtual int handleSignal(int signum) {
        if( signum != SIGPIPE ){return -1;}
        this->failedBoard = 1;
        return 0;
    }

    sig_atomic_t getFailedBoard() const {
        return this->failedBoard;
    }

};

#endif //SIGPIPE_HANDLER
