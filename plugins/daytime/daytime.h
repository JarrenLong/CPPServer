#ifndef DAYTIME_H
#define DAYTIME_H

#include <windows.h>
#include <time.h>
#include "../IPlugin.h"

//Port 13 Time Protocol

class Daytime : public IPlugin {
public:
    CReturn *Process(std::string data);
};

#endif // DAYTIME_H
