#include "echo.h"

void __declspec(dllexport) GetPlugin( IPlugin **pOut) {
    *pOut = new Echo;
}

CReturn *Echo::Process(std::string data) {
    return new CReturn(0,data);
}
