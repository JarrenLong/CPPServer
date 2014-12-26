#ifndef IPLUGIN_H
#define IPLUGIN_H

#include <string>

class CReturn {
public:
    CReturn() : retCode(0) {}
    CReturn(long rc, std::string rs) {
        retCode = rc;
        retStr = rs;
    }
    ~CReturn() {}

    inline long GetLong() {
        return retCode;
    }
    inline std::string GetStr() {
        return retStr;
    }
    inline void SetRet(long code) {
        retCode = code;
    }
    inline void SetRet(std::string str) {
        retStr = str;
    }
private:
    long retCode;
    std::string retStr;
};

class IPlugin {
public:
    virtual ~IPlugin() {}
    virtual void Init() {}
    virtual void Kill() {
        delete this;
    }
    virtual CReturn *Process(std::string data) = 0;
};

extern "C" __declspec(dllexport) void GetPlugin( IPlugin **pOut);

#endif // IPLUGIN_H
