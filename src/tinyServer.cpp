//
// Created by HWZ on 2022/8/10.
//
#include "tinyServer.h"
#include <iostream>

namespace mysock{
    TinyServer::TinyServer(uint16_t Port) : Server(Port)
    {
#ifdef I_OS_WIN
        u_long ul = 1;
        if(ioctlsocket(getRawSocket(), FIONBIO, &ul) != 0)
            std::cerr << "set socket fd failed: " << WSAGetLastError() << "\n";
#elif defined(I_OS_LINUX)
        if(fcntl(getRawSocket(), F_SETFL, fcntl(getRawSocket(), F_GETFL, 0) | O_NONBLOCK) != 0)
            perror("set socket fd failed");
#endif
    }
}