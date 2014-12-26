#include "Server.h"

Server::Server(const char *plugin, unsigned int port, unsigned int maxConn)
: pNum(port), cMax(maxConn), plugName(plugin) {
    int len = strlen(plugin) + 9;
    char buf[len];
    sprintf(buf, "Logs\\%s.xml",plugin);
    log = new CLog(buf);
    isRunning = false;
    log->AddTag(plugName,"Created Server. Status: Not Running");
}

Server::~Server() {
    log->AddTag(plugName,"Destroyed Server.");
    delete plug;
    delete log;
    delete data;
}

void Server::Init() {
    log->AddTag(plugName,"Begin Init ...");
    int len = strlen(plugName) + 12;
    char buf[len];
    sprintf(buf, "Plugins\\%s.dll",plugName);

    plug = new CPlugin;
    plug->Load(buf);
    plug->Init();
    log->AddTag(plugName,"Plugin Loaded...");

    sock.Create();
    sock.Bind(pNum);
    sock.Listen(cMax);
    log->AddTag(plugName,std::string("Socket Bound to port" + pNum));

    data = new CReturn;
}

void Server::Run() {
    isRunning = true;
    while (isRunning) {
        if (sock.Accept(&newSock)) {
            log->AddTag(plugName,"Connection Accepted.");
            char buf[65535];
            newSock.RecvLine(buf,65535);

            data = plug->Process(buf);
            std::string msg = data->GetStr();

            if (msg == "QUIT") {
                isRunning = false;
                log->AddTag(plugName,"Quitting ...");
            } else {
                log->AddTag(plugName,std::string("Received: " + msg));
            }

            newSock.SendText(msg.c_str());
            newSock.Close();
            log->AddTag(plugName,"Connection Closed.");
        }
    }
}

void Server::Kill() {
    log->AddTag(plugName,"Begin Shutdown ...");
    isRunning = false;
    sock.Close();
    plug->Kill();
}

void Server::Unpause() {
    isRunning = true;
    log->AddTag(plugName,"Server Unpaused");
}

void Server::Pause() {
    isRunning = false;
    log->AddTag(plugName,"Server Paused");
}
