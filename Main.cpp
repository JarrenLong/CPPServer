#include <vector>

#include "Server.h"

int main() {
    /*
    Read in config
    Set up environment
    */
    std::vector<Server*> apps;
    std::vector<Server*>::iterator it;

    apps.push_back(new Server("echo",7,10));
    apps.push_back(new Server("daytime",13,10));
    apps.push_back(new Server("smtp",25,10));
    apps.push_back(new Server("time",37,10));
    apps.push_back(new Server("http",80,10));
    apps.push_back(new Server("pop3",110,10));

    for(it = apps.begin(); it != apps.end(); it++) {
        (*it)->Init();
    }
    //for(it = apps.begin(); it != apps.end(); it++) {
        //(*it)->Run();
    //}
    for(it = apps.begin(); it != apps.end(); it++) {
        (*it)->Kill();
    }

    apps.clear();

    return 0;
}
