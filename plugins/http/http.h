#ifndef HTTP_H
#define HTTP_H

#include <windows.h>
#include <vector>
#include <iostream>

#include "../IPlugin.h"

#include "CRequest.h"
#include "CResponse.h"

//Port 80 HTTP Protocol
class Http : public IPlugin {
public:
    CReturn *Process(std::string data); //Get/set the command flag, handle command
protected:
    //Housekeeping
    bool Clean(std::string data);
    void Tokenize(const std::string& str, std::vector<std::string>& tokens, const std::string& delimiters = " ");
    //Command Handlers
    CReturn *Head(std::string opt);
    CReturn *Get(std::string opt);
    CReturn *Put(std::string opt);
    CReturn *Post(std::string opt);
    CReturn *Delete(std::string opt);
    CReturn *Trace(std::string opt);
    CReturn *Options(std::string opt);
    CReturn *Connect(std::string opt);
};

#endif // HTTP_H
