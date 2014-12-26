#define WINDOWS

#ifdef WINDOWS
 #include <windows.h>
 #include <winsock2.h>
#else //*nix
 #include <sys/types.h>
 #include <sys/socket.h>
 #include <netinet/in.h>
 #include <netdb.h>
 #include <arpa/inet.h>
#endif

#include <tinyxml.h>
