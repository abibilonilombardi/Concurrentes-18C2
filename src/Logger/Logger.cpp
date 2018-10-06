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
    return instance;
}

void Logger::fdCheking(){
    if (fd == -1){
        // TODO: definir nuestras excepciones o ver como las vamos a tratar
        // definir mensaje
        //trow new std::execption();
    }
}

void Logger::log(std::string eventDescription){
    int messegeSize = eventDescription.length(); // chequear que sea un int y no de otro tipo

    Lock mylock(fd);
    std::cout<< "el proceso "<< getpid() << " tomo el log " << std::endl;
    ssize_t writedBytes = write(fd, eventDescription.c_str(), messegeSize);
    while( writedBytes < messegeSize){
        writedBytes = write(fd, eventDescription.c_str() + writedBytes , eventDescription.length() - writedBytes);
    }

    std::cout<< "el proceso "<< getpid() << " escribio en el log" << std::endl;
}

void Logger::destroy(){
    fd = close(fd);
    instance->fdCheking();
}
