#ifndef SOCKET_H
#define SOCKET_H

#include "xPlatform.h"

class Socket {
public:
    Socket( void );
    virtual ~Socket( void );

    virtual bool Create( int af = AF_INET );
    virtual bool Connect( const char *pszHost, int nPort );
    virtual bool Bind( int nLocalPort );
    virtual bool Accept( Socket *pSocket );
    virtual bool Listen( int nBacklog = SOMAXCONN );
    virtual int Send( const void *pData, int nDataLen, int nFlags = 0 );
    virtual int SendText( const char *pszText );
    virtual int Recv( void *pData, int nDataLen, int nFlags = 0 );
    virtual int RecvLine( char *pszBuf, int nLen, bool bEcho = false );
    virtual bool Shutdown( int nHow );
    virtual bool Close( void );

    SOCKET m_hSocket;
private:
    static long refCount;
};

#endif
