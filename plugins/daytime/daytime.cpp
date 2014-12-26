#include "daytime.h"

void __declspec(dllexport) GetPlugin( IPlugin **pOut) {
    *pOut = new Daytime;
}

CReturn *Daytime::Process(std::string data) {
    time_t rawtime;
    struct tm *timeinfo;
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    std::string time = asctime(timeinfo);
    //Strip newline at end
    int f = time.find('\n',0);
    if(f!=-1) {
        time.erase(f,1);
    }
    //return new CReturn(0,std::string(asctime(timeinfo)));
    return new CReturn(0,time);
}
