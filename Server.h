#include "./plugins/CPlugin.h"
#include "CLog.h"
#include "Socket.h"

class Server {
public:
    Server(const char *plugin, unsigned int port, unsigned int maxConn);
    ~Server();

    void Unpause();
    void Pause();

    void Init();
    void Run();
    void Kill();
private:
    bool isRunning;
    unsigned int pNum, cMax;
    const char* plugName;
    CLog *log;
    CPlugin *plug;
    CReturn *data;
    Socket sock, newSock;
};
