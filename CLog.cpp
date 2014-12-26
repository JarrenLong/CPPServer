#include "CLog.h"

std::string GetLocalTime() {
    time_t rawtime;
    struct tm * timeinfo;
    char buffer [20];

    time ( &rawtime );
    timeinfo = localtime ( &rawtime );
    strftime (buffer,20,"%Y-%m-%dT%H:%M:%S",timeinfo);
    return std::string(buffer);
}

CLog::CLog(const std::string& sFileName): m_sFileName(sFileName) {
    m_iIndent = 0;
    std::ofstream log(m_sFileName.c_str(), std::ios::app);
    log.close();
    Write("<?xml version=\"1.0\" encoding=\"iso-8859-1\" ?>\n");
}

CLog::~CLog() {
    while (m_vsTags.size() > 0) {
        CloseTagTree();
    }
    Write("\n</xml>\n");
}

template <typename T> bool CLog::Write(const T& Data) {
    try {
        std::ofstream log(m_sFileName.c_str(), std::ios::app);
        log << Data;
        log.close();

        return true;
    } catch (...) {
        return false;
    }
}

void CLog::AddTagTree	(const std::string& TagName) {
    Write(std::string(m_iIndent, ' ') + "<" + TagName + ">\n");
    m_vsTags.push_back(TagName);
    m_iIndent += 1;
}

void CLog::AddTag(const std::string& TagName, const std::string& TagValue) {
    Write(std::string(m_iIndent, ' ') + "<" + TagName + " timestamp=\"" + GetLocalTime() + "\">");
    Write(TagValue);
    Write("</" + TagName + ">\n");
}

void CLog::CloseTagTree() {
    if (m_vsTags.size() > 0) {
        if (m_iIndent > 0) {
            m_iIndent -= 1;
        }
        Write(std::string(m_iIndent, ' ') + std::string("</") + m_vsTags[m_vsTags.size()-1] + std::string(">\n\n"));
        m_vsTags.pop_back();
    }
}
