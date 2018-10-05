#include "Worker.h"

void Worker::travel(){
    //Get origin harbour FIFO name
    string hb = Harbour::entranceName(this->originHarbour);
    //TODO:Open harbour FIFO to Write
    //and then proceed to write PID
    this->travel.p(); //Lock until arrival.
    sleep(8); //spend 8hs working...
}
