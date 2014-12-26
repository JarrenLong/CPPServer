#ifndef FIREWALL_H
#define FIREWALL_H

#include "../IPlugin.h"

//Generic Built-in Firewall
class Firewall : public IPlugin {
public:
    CReturn *Process(std::string data);
protected:
private:
};

#endif // FIREWALL_H
