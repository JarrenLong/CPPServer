#ifndef MAX_PATH
#define MAX_PATH 256
#endif

#include <iostream>
#include <list>
#include <string>
#include <iostream>

#include "../IPlugin.h"
#include "../globals.h"

char g_szDomain[100];
char g_szDirectoryPath[300];
char g_szDomainPath[400];

//Port 110 POP3 Protocol
class CPop3Message {
    char m_szMessagePath[MAX_PATH];
    int m_nStatus;
    long m_dwSize;
public:
    CPop3Message *m_pNextMessage;
    CPop3Message(int nStatus=POP3_MSG_STATUS_UNDEFINED,long nSize=0, char *szMessagepath="") {
        m_nStatus=nStatus;
        m_dwSize=nSize;
        strcpy(m_szMessagePath,szMessagepath);
        m_pNextMessage=NULL;
    }
    void SetParams(int nStatus=POP3_MSG_STATUS_UNDEFINED,long nSize=0, char *szMessagepath="") {
        m_nStatus=nStatus;
        m_dwSize=nSize;
        strcpy(m_szMessagePath,szMessagepath);
    }
    void SetParams(CPop3Message *pMsg) {
        m_nStatus=pMsg->GetStatus();
        m_dwSize=pMsg->GetSize();
        strcpy(m_szMessagePath,pMsg->GetPath());
    }
    void Delete() {
        m_nStatus|=POP3_MSG_STATUS_DELETED;
    }
    void Reset() {
        m_nStatus&= ~POP3_MSG_STATUS_DELETED;
    }
    long GetSize() {
        return m_dwSize;
    }
    int GetStatus() {
        return m_nStatus;
    }
    char *GetPath() {
        return m_szMessagePath;
    }
};

class Pop3 : public IPlugin {
    CPop3Message *m_pPop3MessageHead, *m_pPop3MessageList;
    unsigned int m_nState;
    unsigned int m_nLastMsg;
    char m_szUserHome[MAX_PATH];
    char m_szUserName[MAX_PATH];
    char m_szPassword[MAX_PATH];
    int m_nTotalMailCount, m_dwTotalMailSize;
public:
    void Init() {}
    void Kill() {}
    CReturn *Process(std::string data);

    Pop3();
    virtual ~Pop3(void);
    int ProcessCMD(char *buf, int len);
    int SendResponse(int nResponseType, char *msg="");
    int SendResponse(char *buf);
    bool LockMailDrop(void);
    void UpdateMails(void);
    bool SetHomePath(char *szUserName);
    int SendMessageFile(char* szFilePath);
    int ProcessRPOP(char* buf, int len);
    int ProcessTOP(char* buf, int len);
protected:
    int ProcessUSER(char* buf, int len);
    int ProcessPASS(char* buf, int len);
    int ProcessQUIT(char* buf, int len);
    int ProcessRETR(char* buf, int len);
    int ProcessDELE(char* buf, int len);
    int ProcessNOOP(char* buf, int len);
    int ProcessLAST(char* buf, int len);
    int ProcessRSET(char* buf, int len);
    int ProcessSTAT(char* buf, int len);
    int ProcessLIST(char* buf, int len);
    bool Login(char* szName, char* szPassword);

    CReturn *User(std::string opt) {return new CReturn(0,"NULL");}
    CReturn *Pass(std::string opt) {return new CReturn(0,"NULL");}
    CReturn *Quit(std::string opt) {return new CReturn(0,"NULL");}
    CReturn *Retr(std::string opt) {return new CReturn(0,"NULL");}
    CReturn *Dele(std::string opt) {return new CReturn(0,"NULL");}
    CReturn *Noop(std::string opt) {return new CReturn(0,"NULL");}
    CReturn *Last(std::string opt) {return new CReturn(0,"NULL");}
    CReturn *Rset(std::string opt) {return new CReturn(0,"NULL");}
    CReturn *Stat(std::string opt) {return new CReturn(0,"NULL");}
    CReturn *List(std::string opt) {return new CReturn(0,"NULL");}
    CReturn *Rpop(std::string opt) {return new CReturn(0,"NULL");}
    CReturn *Top(std::string opt) {return new CReturn(0,"NULL");}
};
