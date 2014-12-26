#ifndef CPLUGIN_H
#define CPLUGIN_H

#include "../xPlatform.h"
#include "IPlugin.h"

class CPlugin {
public:
    CPlugin() : m_pPlugin(0), m_hLibrary(0) {}
    ~CPlugin() { }
    IPlugin* Load(const char* name) {
        LOADPLUGIN pfnLoad;
        m_hLibrary = (HINSTANCE)::LoadLibrary( name );
        if ( m_hLibrary == NULL )
            return 0;
        pfnLoad = (LOADPLUGIN)::GetProcAddress( m_hLibrary, "GetPlugin");
        if ( pfnLoad )
            pfnLoad( &m_pPlugin );
        return m_pPlugin;
    }
    void Init() {
        m_pPlugin->Init();
    }
    CReturn *Process(std::string data) {
        return m_pPlugin->Process(data);
    }
    void Kill() {
        if ( m_pPlugin == 0 )
            return;
        m_pPlugin->Kill();
        ::FreeLibrary( m_hLibrary );
    }
private:
    IPlugin* m_pPlugin;
    typedef void (*LOADPLUGIN)( IPlugin** );
    HINSTANCE m_hLibrary;
};

#endif // CPLUGIN_H
