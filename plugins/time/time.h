#ifndef TIME_H
#define TIME_H

#include <windows.h>
#include <time.h>
#include "../IPlugin.h"

//Port 37 Time Protocol

class Time : public IPlugin {
public:
    CReturn *Process(std::string data);
};

#endif // TIME_H
