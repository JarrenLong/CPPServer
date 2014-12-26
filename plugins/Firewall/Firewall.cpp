#include "Firewall.h"

void __declspec(dllexport) GetPlugin( IPlugin **pOut) {
    *pOut = new Firewall;
}

CReturn *Firewall::Process(std::string data) {
    return new CReturn(0,"Firewall_Process");
}
