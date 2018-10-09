#ifndef LOGGER_H_
#define LOGGER_H_

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string>
#include <ctime>

// TODO: VER DONDE DECARAMOS EL NOMBRE DEL ARCHIVO DE LOG
// TODO: DOCUMENTAR

class Logger {
private:
    static std::string LOG_FILE_NAME;
    static int fd;
    time_t now;
public:
    static Logger& getInstance();
    static void destroy();
    void log(const std::string& eventDescription);
private:
    Logger();
    void fdCheking();
    std::string getCurrentTime();
};


#endif /* LOGGER_H_ */
