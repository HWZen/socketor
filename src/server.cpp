#include "server.h"
#include <memory>

#ifdef I_OS_LINUX
void WSACleanup(){}
#endif

namespace mysock {

    Server::Server(uint16_t Port) {

        Socket_info.sin_family = AF_INET;
        Socket_info.sin_port = htons(Port);
        Socket_info.sin_addr.s_addr = htonl(INADDR_ANY);
        socketor::Port = Port;
    }

    int Server::listen() {
        if (hasListened)
            return LISTEN_SUCESS;

#ifdef I_OS_WIN
        socketor::wsaInit();
#endif // I_OS_WIN

        Socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (int err = bind(Socket, (struct sockaddr *) &Socket_info, sizeof(SOCKADDR_IN)); err == -1) {
            return BIND_FAIL;
        }
        if (::listen(Socket, 20) == -1) {
            return LISTEN_FAIL;
        }
        hasListened = true;
        return LISTEN_SUCESS;
    }

    Server::~Server() {
        closesocket(Socket);
        hasListened = false;
    }

    int Server::rawAccept(client &socketBuf) {
        if (!hasListened)
            return NO_LISTENED;

        SOCKADDR_IN rawClient;

        // 开始接受握手请求，accept()是阻断函数，等成功接受后才会继续进行程序
#ifdef I_OS_WIN
        int iaddrSize = sizeof(SOCKADDR_IN);
        SOCKET temp = ::accept(Socket, (struct sockaddr *) &rawClient, &iaddrSize);
        Server::client cclient(temp, rawClient);


#endif

#ifdef I_OS_LINUX
        socklen_t length = sizeof(rawClient);
        Server::client cclient(::accept(Socket, (struct sockaddr *)&rawClient, &length), rawClient);
#endif
        socketBuf = std::move(cclient);
        return SUCESS;
    }

    constexpr bool Server::setPort(uint16_t Port) {
        if (hasListened)
            return false;
        socketor::Port = Port;
        return true;
    }

    bool Server::isListen() const {
        return hasListened;
    }

    int Server::closeListen() {
        if(hasListened)
            return closesocket(socketor::getRawSocket());
        else
            return mysock::flag::NO_LISTENED;
    }

    int Server::accept() {
        Server::client cclient;
        cclient.setServerPort(port());
        return rawAccept(cclient);
    }


    void Server::client::setServerPort(uint16_t port) {
        m_serverPort = port;
    }

    uint16_t Server::client::getServerPort() {
        return m_serverPort;
    }

    int64_t Server::client::Send(const void *dataBuf, size_t len) const {
        if(!m_hasConnected)
            return 0;
        return socketor::Send(dataBuf, len);
    }

    int64_t Server::client::Send(const std::string &str) const {
        if(!m_hasConnected)
            return 0;
        return socketor::Send(str);
    }

    int Server::client::closeConnect() {
        if(m_hasConnected){
            m_hasConnected = false;
            return closesocket(socketor::getRawSocket());
        }
        else
            return flag::NO_CONNECTED;
    }
}// namespace mysock
