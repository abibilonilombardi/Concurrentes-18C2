#ifndef LOGGER_H_
#define LOGGER_H_

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string>
#include <ctime>

#include "Lock/Lock.h"


// TODO: VER DONDE DECARAMOS EL NOMBRE DEL ARCHIVO DE LOG
// TODO: DOCUMENTAR

class Logger {
private:
    // static const char* LOG_FILE_NAME;
    static Logger* instance;
    static int fd;
    time_t now;
public:
    static Logger* getInstance();
    static void destroy();
    void log(std::string eventDescription);
private:
    Logger();
    void fdCheking();
};


#endif /* LOGGER_H_ */
