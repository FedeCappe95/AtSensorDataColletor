#ifndef __GICAME__NETWORK_UTILITY_H__
#define __GICAME__NETWORK_UTILITY_H__


/*#ifdef WINDOWS
#pragma comment(lib,"ws2_32.lib")   //winsock2
#pragma comment(lib,"ntdll.lib")    //ip2string.h
#endif*/


#include "../common.h"


#ifdef WINDOWS
#include <winsock2.h>
#include <iphlpapi.h>
#include <stdio.h>
#include <stdlib.h>
#include <ws2tcpip.h>
#else
#include <netinet/in.h>
#include <arpa/inet.h>
#endif


// Help-functions usefull in some classes
namespace Gicame {

    #ifdef WINDOWS
    void startWsaIfNeeded();
    #endif

};

#endif
