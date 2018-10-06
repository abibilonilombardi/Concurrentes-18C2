#include "Logger.h"

Logger* Logger::instance = NULL;
int Logger::fd = -1;
// char* Logger::LOG_FILE_NAME = "ShipsTrevelingSimulation.log";

Logger::Logger () {
}

Logger* Logger::getInstance(){
    if (instance){
        return instance;
    }
    fd = open("ShipsTrevelingSimulation.log", O_CREAT|O_WRONLY);
    instance->fdCheking();
    instance = new Logger();
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

    std::string logMessage =  getCurrentTime() + " PID: " + std::to_string(getpid()) + " - " + eventDescription;

    int messegeSize = logMessage.length();
    
    std::cout<< "el proceso "<< getpid() << " tomo el log " << std::endl;

    ssize_t writedBytes = write(fd, logMessage.c_str(), messegeSize);
    while( writedBytes < messegeSize){
        writedBytes = write(fd, logMessage.c_str() + writedBytes , messegeSize - writedBytes);
    }

    std::cout<< "el proceso "<< getpid() << " escribio en el log" << std::endl;
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