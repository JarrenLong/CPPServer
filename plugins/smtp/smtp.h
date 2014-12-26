#ifndef SMTP_H
#define SMTP_H

#include <windows.h>
#include <winsock2.h>
#include <shlwapi.h>
#include "../IPlugin.h"
#include "../globals.h"
#include "../MailAddress.h"

char g_szDirectoryPath[300];
char g_szDomainPath[300];
char g_szDomain[300];

#define DIRECTORY_ROOT g_szDirectoryPath

//Port 25 SMTP Protocol
class CMailSession : public IPlugin {
public:
    void Init() {}
    void Kill() {}
    CReturn *Process(std::string data) {return new CReturn(0,"SMTP_Process");}
    char m_szFileName[MAX_PATH+1];
    void *m_pszData;
    int data_len;
    HANDLE m_hMsgFile;
    unsigned int m_nStatus;
    int m_nRcptCount;
    CMailAddress m_FromAddress, m_ToAddress[MAX_RCPT_ALLOWED+1];
    CMailSession(){}
    SOCKET m_socConnection;
    CMailSession(SOCKET client_soc) {
        m_nStatus=SMTP_STATUS_INIT;
        m_socConnection=client_soc;
        m_pszData=NULL;
        data_len=0;
        m_nRcptCount=0;
    }
    int ProcessCMD(char *buf, int len);
    int SendResponse(int nResponseType);
    int ProcessDATAEnd(void);
    bool CreateNewMessage(void);
private:
    int ProcessHELO(char *buf, int len);
    int ProcessRCPT(char *buf, int len);
    int ProcessMAIL(char *buf, int len);
    int ProcessRSET(char *buf, int len);
    int ProcessNOOP(char *buf, int len);
    int ProcessQUIT(char *buf, int len);
    int ProcessDATA(char *buf, int len);
    int ProcessNotImplemented(bool bParam=false);
};

#endif //SMTP_H
