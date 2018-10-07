#include "Logger.h"

Logger* Logger::instance = NULL;
int Logger::fd = -1;
std::string Logger::LOG_FILE_NAME = "ShipsTrevelingSimulation.log";

Logger::Logger () {}

Logger* Logger::getInstance(){
    if (instance){
        return instance;
    }

    instance->fdCheking();
    instance = new Logger();
    return instance;
}

void Logger::fdCheking(){
    if (fd == -1){
        // TODO: definir nuestras excepciones o ver como las vamos a tratar
        // definir mensaje
        //trow new std::execption();
    }
}

void Logger::log(const std::string& eventDescription){
    Lock mylock(fd);
    fd = open(Logger::LOG_FILE_NAME.c_str(), O_CREAT|O_RDWR, S_IRWXG);

    std::string logMessage =  getCurrentTime() + " PID: " + std::to_string(getpid()) + " - " + eventDescription;

    int messegeSize = logMessage.length();

    //std::cout<< "el proceso "<< getpid() << " tomo el log " << std::endl;

    ssize_t writedBytes = write(fd, logMessage.c_str(), messegeSize);
    while( writedBytes < messegeSize){
        writedBytes = write(fd, logMessage.c_str() + writedBytes , messegeSize - writedBytes);
    }
    close(fd);
    //std::cout<< "el proceso "<< getpid() << " escribio en el log" << std::endl;
}

std::string Logger::getCurrentTime(){
    char buff[20];
    strftime (buff, sizeof(buff), "%Y/%m/%d %H:%M:%S", localtime(&now));
    return std::string(buff);
}


void Logger::destroy(){
    instance->fdCheking();
    delete instance;
}
