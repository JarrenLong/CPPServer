#include "time.h"

void __declspec(dllexport) GetPlugin( IPlugin **pOut) {
    *pOut = new Time;
}

CReturn *Time::Process(std::string data) {
    time_t rawtime;
    char buffer[4];
    time(&rawtime);
    sprintf(buffer,"%ld", rawtime);
    return new CReturn(0,std::string(buffer));
}
