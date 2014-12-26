#include "pop3.h"

#include <shlwapi.h>

void __declspec(dllexport) GetPlugin( IPlugin **pOut) {
    *pOut = new Pop3;
}

CReturn *Pop3::Process(std::string data) {
    return new CReturn(0,"POP3_Process");
}

Pop3::Pop3() {
    m_nState=POP3_STATE_AUTHORIZATION;
    m_szUserName[0]=0;
    m_szUserHome[0]=0;
    m_pPop3MessageHead=NULL;
    m_pPop3MessageList=NULL;
    m_nLastMsg=0;
}

Pop3::~Pop3(void) {
    if (m_pPop3MessageHead) {
        //TODO: Delete the entire list;
        //delete(m_pPop3MessageList);
    }
}

int Pop3::ProcessCMD(char *buf, int len) {
    if (_strnicmp(buf,"USER",4)==0) {
        return ProcessUSER(buf, len);
    } else if (_strnicmp(buf,"PASS",4)==0) {
        return ProcessPASS(buf, len);
    } else if (_strnicmp(buf,"QUIT",4)==0) {
        return ProcessQUIT(buf, len);
    } else if (_strnicmp(buf,"STAT",4)==0) {
        return ProcessSTAT(buf, len);
    } else if (_strnicmp(buf,"LIST",4)==0) {
        return ProcessLIST(buf, len);
    } else if (_strnicmp(buf,"RETR",4)==0) {
        return ProcessRETR(buf, len);
    } else if (_strnicmp(buf,"DELE",4)==0) {
        return ProcessDELE(buf, len);
    } else if (_strnicmp(buf,"NOOP",4)==0) {
        return ProcessNOOP(buf, len);
    } else if (_strnicmp(buf,"LAST",4)==0) {
        return ProcessLAST(buf, len);
    } else if (_strnicmp(buf,"RSET",4)==0) {
        return ProcessRSET(buf, len);
    } else if (_strnicmp(buf,"RPOP",4)==0) {
        return ProcessRSET(buf, len);
    } else if (_strnicmp(buf,"TOP",4)==0) {
        return ProcessRSET(buf, len);
    }
    return SendResponse(POP3_DEFAULT_NEGATIVE_RESPONSE);
    return 0;
}

int Pop3::SendResponse(char *buf) {
    //int len=(int)strlen(buf);

    printf("Direct Sending: %s",buf);

    //send(m_socConnection,buf,len,0);

    return 0;
}

int Pop3::SendResponse(int nResponseType, char *msg) {
    char buf[100];
    int len;

    if (nResponseType==POP3_DEFAULT_AFFERMATIVE_RESPONSE) {
        if (strlen(msg))
            sprintf(buf,"+OK %s\r\n",msg);
        else
            sprintf(buf,"+OK %s\r\n","Action performed");
    } else if (nResponseType==POP3_DEFAULT_NEGATIVE_RESPONSE)
        sprintf(buf,"-ERR %s\r\n","An error occured");
    else if (nResponseType==POP3_WELCOME_RESPONSE)
        sprintf(buf,"+OK %s %s POP3 Server ready on %s\r\n",APP_TITLE, APP_VERSION,DOMAIN_NAME);
    else if (nResponseType==POP3_STAT_RESPONSE)
        sprintf(buf,"+OK %i %d\r\n",m_nTotalMailCount, m_dwTotalMailSize);

    len=(int)strlen(buf);

    printf("Sending: %s",buf);
    //send(m_socConnection,buf,len,0);

    return nResponseType;
}

int Pop3::ProcessUSER(char* buf, int len) {
    printf("ProcessUSER\n");
    buf[len-2]=0;
    buf+=5;

    strcpy(m_szUserName,buf);
    sprintf(m_szUserHome,"%s\\%s",DOMAIN_ROOT_PATH,buf);

    if (1/* !PathFileExists(m_szUserHome) */) {
        printf("User %s's Home '%s' not found\n",m_szUserName, m_szUserHome);
        return SendResponse(POP3_DEFAULT_NEGATIVE_RESPONSE);
    }

    printf("OK User %s Home %s\n",m_szUserName, m_szUserHome);

    if (m_nState!=POP3_STATE_AUTHORIZATION) {
        return SendResponse(POP3_DEFAULT_NEGATIVE_RESPONSE);
    }

    return SendResponse(POP3_DEFAULT_AFFERMATIVE_RESPONSE);
}

int Pop3::ProcessPASS(char* buf, int len) {
    printf("ProcessPASS\n");
    buf[len-2]=0;
    buf+=5;
    if (buf[len-2]==10) buf[len-2]=0;

    strcpy(m_szPassword,buf);

    if (m_nState!=POP3_STATE_AUTHORIZATION || strlen(m_szUserName)<1) {
        return SendResponse(POP3_DEFAULT_NEGATIVE_RESPONSE);
    }

    if (Login(m_szUserName, m_szPassword))
        return SendResponse(POP3_DEFAULT_AFFERMATIVE_RESPONSE);
    else
        return SendResponse(POP3_DEFAULT_NEGATIVE_RESPONSE);

    return 0;
}

int Pop3::ProcessQUIT(char* buf, int len) {
    printf("ProcessQUIT\n");
    if (m_nState==POP3_STATE_TRANSACTION)
        m_nState=POP3_STATE_UPDATE;

    SendResponse(POP3_DEFAULT_AFFERMATIVE_RESPONSE,"Goodbye");

    UpdateMails();

    return -1;
}

bool Pop3::Login(char* szName, char* szPassword) {
    printf("Login: ");
    printf("user= [%s] password = [%s]\n",m_szUserName,m_szPassword);

    char lpPwdFile[300], lpUserHome[300];

    sprintf(lpPwdFile,"%s\\%s\\%s.pwd",g_szDomainPath,szName,szPassword);
    printf("Pwd file: %s\n",lpPwdFile);
    if (1/* PathFileExists(lpPwdFile) */) {
        printf("Password ok\n");
        m_nState=POP3_STATE_TRANSACTION;

        sprintf(lpUserHome,"%s\\%s",g_szDomainPath,szName);
        SetHomePath(lpUserHome);

        LockMailDrop();
        return true;
    }
    return false;
}

int Pop3::ProcessSTAT(char* buf, int len) {
    printf("ProcessSTAT\n");
    if (m_nState!=POP3_STATE_TRANSACTION) {
        return SendResponse(POP3_DEFAULT_NEGATIVE_RESPONSE);
    }
    m_nLastMsg=1;
    return SendResponse(POP3_STAT_RESPONSE);
}

int Pop3::ProcessLIST(char* buf, int len) {
    buf+=4;
    buf[4]='0';
    buf[len-2]=0;
    int msg_id=atol(buf);
    printf("ProcessLIST %d\n",msg_id);

    if (m_nState!=POP3_STATE_TRANSACTION) {
        return SendResponse(POP3_DEFAULT_NEGATIVE_RESPONSE);
    }

    if (msg_id>0) {
        char resp[100];
        sprintf(resp, "+OK %d %ld\r\n",msg_id, m_pPop3MessageList[msg_id-1].GetSize());
        return SendResponse(resp);
    } else {
        SendResponse("+OK \r\n");

        for (int i=0; i < m_nTotalMailCount; i++) {
            char resp[100];
            sprintf(resp, "%d %ld\r\n",i+1, m_pPop3MessageList[i].GetSize());
            SendResponse(resp);
        }

        SendResponse(".\r\n");
    }

    return 0;
}

int Pop3::ProcessRETR(char* buf, int len) {
    buf+=4;
    buf[4]='0';
    buf[len-2]=0;
    int msg_id=atol(buf);

    printf("ProcessRETR %d\n", msg_id);
    if (m_nState!=POP3_STATE_TRANSACTION) {
        return SendResponse(POP3_DEFAULT_NEGATIVE_RESPONSE);
    }

    if (msg_id>m_nTotalMailCount) {
        return SendResponse("-ERR Invalid message number\r\n");
    }

    if (m_nLastMsg<(unsigned int)msg_id) m_nLastMsg=msg_id;

    char resp[25];
    sprintf(resp,"+OK %ld octets\r\n",m_pPop3MessageList[msg_id-1].GetSize());
    SendResponse(resp);
    SendMessageFile(m_pPop3MessageList[msg_id-1].GetPath());
    SendResponse("\r\n.\r\n");

    return 0;
    //return SendResponse(POP3_DEFAULT_NEGATIVE_RESPONSE);
}

int Pop3::ProcessDELE(char* buf, int len) {
    buf+=4;
    buf[4]='0';
    buf[len-2]=0;
    int msg_id=atol(buf);

    printf("ProcessDELE %d\n",msg_id);

    if (m_nState!=POP3_STATE_TRANSACTION || msg_id>m_nTotalMailCount) {
        return SendResponse(POP3_DEFAULT_NEGATIVE_RESPONSE);
    }

    m_pPop3MessageList[msg_id-1].Delete();
    return SendResponse(POP3_DEFAULT_AFFERMATIVE_RESPONSE);
}

int Pop3::ProcessNOOP(char* buf, int len) {
    printf("ProcessNOOP\n");
    return SendResponse(POP3_DEFAULT_NEGATIVE_RESPONSE);
}

int Pop3::ProcessLAST(char* buf, int len) {
    if (m_nState!=POP3_STATE_TRANSACTION) {
        return SendResponse(POP3_DEFAULT_NEGATIVE_RESPONSE);
    }

    printf("ProcessLAST\n");
    char resp[25];

    sprintf(resp, "+OK %d\r\n",m_nLastMsg);
    return SendResponse(resp);
}

int Pop3::ProcessRSET(char* buf, int len) {
    printf("ProcessRSET\n");

    if (m_nState!=POP3_STATE_TRANSACTION) {
        return SendResponse(POP3_DEFAULT_NEGATIVE_RESPONSE);
    }
    for (int i=0; i < m_nTotalMailCount; i++) {
        m_pPop3MessageList[i].Reset();
        //printf("Message %d: %ld %s\n",i+1,m_pPop3MessageList[i].GetSize(), m_pPop3MessageList[i].GetPath());
    }
    return SendResponse(POP3_DEFAULT_NEGATIVE_RESPONSE);
}

void Pop3::UpdateMails(void) {
    printf("Updating mails\n");

    if (m_nState!=POP3_STATE_UPDATE) {
        printf("Called update but state is not POP3_STATE_UPDATE (%d)\n",POP3_STATE_UPDATE);
        return;
    }

    for (int i=0; i < m_nTotalMailCount; i++) {
        if (m_pPop3MessageList[i].GetStatus()& POP3_MSG_STATUS_DELETED) {
            printf("Delete file %s\n",m_pPop3MessageList[i].GetPath());
            DeleteFile(m_pPop3MessageList[i].GetPath());
        }
        //printf("Message %d: %ld %s\n",i+1,m_pPop3MessageList[i].GetSize(), m_pPop3MessageList[i].GetPath());
    }

}

bool Pop3::SetHomePath(char *lpPath) {
    strcpy(m_szUserHome,lpPath);

    return true;
}

int Pop3::SendMessageFile(char* szFilePath) {
    HANDLE findH, fileH;
    WIN32_FIND_DATA findData;
    unsigned long lenRead, len;
    char *buf;

    fileH = CreateFile(szFilePath, GENERIC_READ, FILE_SHARE_READ, NULL,
                       OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (fileH == INVALID_HANDLE_VALUE)
        return 0;
    findH = FindFirstFile(szFilePath, &findData);
    if (findH == INVALID_HANDLE_VALUE) {
        CloseHandle(fileH);
        return 0;
    }
    len = findData.nFileSizeLow;
    buf = (char *)malloc(len+5);
    if (buf != NULL) {
        //ReadFile(fileH, buf, len, &lenRead, NULL);
        if (len != lenRead) {
            free(buf);
            buf = NULL;
            printf("Read error (len!=readlen) file %s\n",szFilePath);
            return 0;
        }
    } else {
        printf("Can not open file %s\n",szFilePath);
        return 0;
    }

    FindClose(findH);
    CloseHandle(fileH);

    printf("Sending: %s\n",szFilePath);
    //send(m_socConnection,buf,len,0);

    return len;
}

int Pop3::ProcessRPOP(char* buf, int len) {
    printf("ProcessRPOP\n");
    return SendResponse(POP3_DEFAULT_NEGATIVE_RESPONSE);
}

int Pop3::ProcessTOP(char* buf, int len) {
    printf("ProcessTOP\n");
    return SendResponse(POP3_DEFAULT_NEGATIVE_RESPONSE);
}

bool Pop3::LockMailDrop(void) {
    printf("Locking maildrop\n");

    WIN32_FIND_DATA FileData;
    HANDLE hSearch;
    //long dwAttrs;
    //char szNewPath[MAX_PATH];

    m_dwTotalMailSize=0;
    m_nTotalMailCount=0;

    char szSearchPath[MAX_PATH];

    sprintf(szSearchPath, "%s\\%s\\%s",m_szUserHome,USER_MAIL_DIRECTORY_NAME,MSG_SEARCH_WILDCARD);

    printf("Search Path: %s", szSearchPath);
    hSearch = FindFirstFile(szSearchPath, &FileData);

    if (hSearch == INVALID_HANDLE_VALUE) {
        printf("No mail message files found.\n");
        return true;
    }

    CPop3Message *pHead=NULL, *pNewMsg=NULL;

    while (true) {
        long dwSize=FileData.nFileSizeLow;

        m_nTotalMailCount++;
        m_dwTotalMailSize+=FileData.nFileSizeLow /*+(FileData.nFileSizeHigh * (MAXlong+1))*/;
        char msgPath[300];

        sprintf(msgPath,"%s\\%s\\%s",m_szUserHome,USER_MAIL_DIRECTORY_NAME,FileData.cFileName);

        pNewMsg=new CPop3Message(dwSize,POP3_MSG_STATUS_NEW,msgPath);

        pNewMsg->m_pNextMessage=pHead;
        pHead=pNewMsg;

        printf("Message %d: %s\n",m_nTotalMailCount, FileData.cFileName);
        if (!FindNextFile(hSearch, &FileData))
            break;
    }

    FindClose(hSearch);
    printf("TotalMailCount %d TotalMailSize %d\n", m_nTotalMailCount, m_dwTotalMailSize);

    if (m_nTotalMailCount) {
        m_pPop3MessageList=new CPop3Message[m_nTotalMailCount];
        if (!m_pPop3MessageList) return false;

        for (int i=0; i < m_nTotalMailCount; i++) {
            pNewMsg=pHead;
            m_pPop3MessageList[i].SetParams(pNewMsg->GetSize(), pNewMsg->GetStatus(), pNewMsg->GetPath());
            pHead=pHead->m_pNextMessage;
        }

        //printf("Total %d messages of %ld octates found\n",m_nTotalMailCount, m_dwTotalMailSize);

        for (int i=0; i < m_nTotalMailCount; i++) {
            printf("Message %d: %ld %s\n",i+1,m_pPop3MessageList[i].GetSize(), m_pPop3MessageList[i].GetPath());
        }

    }

    return true;
}
