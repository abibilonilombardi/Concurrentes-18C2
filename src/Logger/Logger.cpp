#include "../Logger/Logger.h"
#include "../Lock/ExclusiveLock.h"
#include <cerrno>
#include <cstring>

int Logger::fd = -1;
std::string Logger::LOG_FILE_NAME = "ShipsTrevelingSimulation.log";

Logger::Logger () {}

Logger& Logger::getInstance(){
    static Logger instance;
    
    fd = open("ShipsTrevelingSimulation.log", O_CREAT|O_WRONLY, 0666);
    instance.fdCheking();
    return instance;
}

void Logger::fdCheking(){
    if (fd == -1){
        throw "Error at LoggerFile creation! " + std::string(strerror(errno)) ;
    }
}

void Logger::log(const std::string& eventDescription){
    ExclusiveLock mylock(fd);
    
    std::string logMessage =  getCurrentTime() + " PID: " + std::to_string(getpid()) + " - " + eventDescription;
    int messegeSize = logMessage.length();

    ssize_t writedBytes = 0;
    while( writedBytes < messegeSize){
        writedBytes += write(fd, logMessage.c_str() + writedBytes , messegeSize - writedBytes);
        if(writedBytes == -1){throw "Error Logger::log(const std::string& eventDescription) =" + eventDescription;}
    }
    mylock.unlock();
}

std::string Logger::getCurrentTime(){
    char buff[20];
    strftime (buff, sizeof(buff), "%Y/%m/%d %H:%M:%S", localtime(&now));
    return std::string(buff);
}


void Logger::destroy(){
}
