#ifndef _CLOG_H_
#define _CLOG_H_

#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <tinyxml.h>
std::string GetLocalTime();

class CLog {
private:
    unsigned int m_iIndent;
    std::vector<std::string> m_vsTags;
    std::string	m_sFileName;
public:
    CLog(const std::string& sFileName);
    virtual ~CLog();
    template <typename T> bool Write(const T& Data);
    void AddTagTree	(const std::string& TagName);
    void AddTag(const std::string& TagName, const std::string& TagValue);
    void CloseTagTree();
};

#endif //_CLOG_H_
