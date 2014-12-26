#include "http.h"

void __declspec(dllexport) GetPlugin( IPlugin **pOut) {
    *pOut = new Http;
}

CReturn *Http::Process(std::string data) {
    //Check for valid input
    if (!Clean(data)) {
        return new CReturn(-1,"Dirty Input!");
    }
    std::string command, options, protocol;

    //Tokenize input string
    std::vector<std::string> tokens;
    Tokenize(data,tokens);

    command = tokens.at(0);
    options = tokens.at(1);
    protocol = tokens.at(2);

    if (protocol != "HTTP/1.0" & protocol != "HTTP/1.1") {
        return new CReturn(-1,"Bad Protocol");
    }

    if (command == "HEAD") {
        return Head(options);
    } else if (command == "GET")  {
        return Get(options);
    } else if (command == "POST") {
        return Post(options);
    } else if (command == "PUT") {
        return Put(options);
    } else if (command == "DELETE") {
        return Delete(options);
    } else if (command == "TRACE") {
        return Trace(options);
    } else if (command == "OPTIONS") {
        return Options(options);
    } else if (command == "CONNECT") {
        return Connect(options);
    } else { //No Command
        return new CReturn(-1,"Invalid Command!");
    }
    //Shouldn't ever get here
    return new CReturn(-1,"End of Switch");
}

bool Http::Clean(std::string data) {
    int len = data.length();
    //Check input length
    if (len < 0 | len > 1024) {
        return false;
    }
    //Check for invalid characters & duplicates
    int i = 0, c = 0;
    char tmp = 0;
    while (i<len) {
        if (data[i] == '*' |
                data[i] == ':' |
                data[i] == ';' |
                data[i] == '<' |
                data[i] == '>' |
                data[i] == '|') {
            return false;
        }
        //Check for duplicate characters
        if (data[i] == tmp) {
            c++;
        } else {
            c = 0;
        }
        //If more than 4 duplicates in a row, exit
        if (c > 4) {
            return false;
        }
        tmp = data[i];
        i++;
    }
    return true;
}

void Http::Tokenize(const std::string& str, std::vector<std::string>& tokens, const std::string& delimiters) {
    std::string::size_type lastPos = str.find_first_not_of(delimiters, 0);
    std::string::size_type pos = str.find_first_of(delimiters, lastPos);

    while (std::string::npos != pos || std::string::npos != lastPos) {
        tokens.push_back(str.substr(lastPos, pos - lastPos));
        lastPos = str.find_first_not_of(delimiters, pos);
        pos = str.find_first_of(delimiters, lastPos);
    }
}

CReturn *Http::Head(std::string opt) {
    return new CReturn(0,"HEAD");
}

CReturn *Http::Get(std::string opt) {
    return new CReturn(1,"GET");
}

CReturn *Http::Put(std::string opt) {
    return new CReturn(2,"PUT");
}

CReturn *Http::Post(std::string opt) {
    return new CReturn(3,"POST");
}

CReturn *Http::Delete(std::string opt) {
    return new CReturn(4,"DELETE");
}

CReturn *Http::Trace(std::string opt) {
    return new CReturn(5,"TRACE");
}

CReturn *Http::Options(std::string opt) {
    return new CReturn(6,"OPTIONS");
}

CReturn *Http::Connect(std::string opt) {
    return new CReturn(7,"CONNECT");
}
