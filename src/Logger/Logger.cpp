#include "../Logger/Logger.h"
#include "../Lock/ExclusiveLock.h"
#include <cerrno>
#include <cstring>

Logger* Logger::instance = NULL;
int Logger::fd = -1;
// char* Logger::LOG_FILE_NAME = "ShipsTrevelingSimulation.log";

Logger::Logger () {
}

Logger* Logger::getInstance(){
    if (instance){
        return instance;
    }
    fd = open("ShipsTrevelingSimulation.log", O_CREAT|O_WRONLY,0666);
    std::cout << "get instance FD: "<< fd <<std::endl;
    instance->fdCheking();
    instance = new Logger();
    return instance;
}

void Logger::fdCheking(){
    if (fd == -1){
        std::cerr << strerror(errno) << std::endl;
        // TODO: definir nuestras excepciones o ver como las vamos a tratar
        // definir mensaje
        //trow new std::execption();
    }
}

void Logger::log(const std::string& eventDescription){
    ExclusiveLock mylock(fd);
    
    std::cout << " FD: "<< fd <<"el proceso "<< getpid()<< " escribe " << std::endl;
    std::string logMessage =  getCurrentTime() + " PID: " + std::to_string(getpid()) + " - " + eventDescription;
    int messegeSize = logMessage.length();

    ssize_t writedBytes = write(fd, logMessage.c_str(), messegeSize);
    while( writedBytes < messegeSize){
        writedBytes = write(fd, logMessage.c_str() + writedBytes , messegeSize - writedBytes);
    }

    sleep(1);
    
    writedBytes = write(fd, logMessage.c_str(), messegeSize);
    while( writedBytes < messegeSize){
        writedBytes = write(fd, logMessage.c_str() + writedBytes , messegeSize - writedBytes);
    }

}

std::string Logger::getCurrentTime(){
    char buff[20];
    strftime (buff, sizeof(buff), "%Y/%m/%d %H:%M:%S", localtime(&now));
    return std::string(buff);
}


void Logger::destroy(){
    fd = close(fd);
    instance->fdCheking();
}
