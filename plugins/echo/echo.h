#ifndef ECHO_H
#define ECHO_H

#include <windows.h>
#include "../IPlugin.h"

class Echo : public IPlugin {
public:
    CReturn *Process(std::string data);
};

#endif // ECHO_H
