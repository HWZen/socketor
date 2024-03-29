#include "server.h"
#include <memory>

#ifdef I_OS_LINUX
void WSACleanup(){}
#endif

namespace mysock {

    Server::Server(uint16_t Port) :socketor(makeSocketor()) {
        Socket_info.sin_port = htons(Port);
        socketor::Port = Port;
    }

    int Server::listen() {
        if (m_hasListened)
            return LISTEN_SUCESS;

        if (int err = bind(Socket, (struct sockaddr *) &Socket_info, sizeof(SOCKADDR_IN)); err == -1) {
            return BIND_FAIL;
        }
        if (::listen(Socket, 20) == -1) {
            return LISTEN_FAIL;
        }
        m_hasListened = true;
        return LISTEN_SUCESS;
    }

    Server::~Server() {
        closesocket(Socket);
        m_hasListened = false;
    }

    int Server::rawAccept(Client &socketBuf) {
        if (!m_hasListened)
            return NO_LISTENED;

        SOCKADDR_IN rawClient;

#ifdef I_OS_WIN
        int iaddrSize = sizeof(SOCKADDR_IN);
        auto temp = ::accept(Socket, (struct sockaddr *) &rawClient, &iaddrSize);
        if(temp == INVALID_SOCKET)
            return ACCEPT_FAIL;
        Server::Client client(temp, rawClient);

#endif

#ifdef I_OS_LINUX
        socklen_t length = sizeof(rawClient);
        auto temp = ::accept(Socket, (struct sockaddr *)&rawClient, &length);
        if(temp == INVALID_SOCKET)
            return ACCEPT_FAIL;
        Server::Client client(temp, rawClient);
#endif
        socketBuf = std::move(client);
        return SOCKETOR_SUCESS;
    }

    constexpr bool Server::setPort(uint16_t Port) {
        if (m_hasListened)
            return false;
        socketor::Port = Port;
        return true;
    }

    bool Server::isListen() const {
        return m_hasListened;
    }

    int Server::closeListen() {
        if(m_hasListened)
            return closesocket(socketor::getRawSocket());
        else
            return mysock::flag::NO_LISTENED;
    }

    Server::Client Server::accept() {
        Server::Client client;
        client.setServerPort(port());
        if(rawAccept(client) != SOCKETOR_SUCESS)
            client.m_hasConnected = false;
        return client;
    }

    Server::Server(Server && other) noexcept : socketor(std::move(other)) {
        m_hasListened = other.m_hasListened;
        other.m_hasListened = false;
    }

    void Server::Client::setServerPort(uint16_t port) {
        m_serverPort = port;
    }

    uint16_t Server::Client::getServerPort() {
        return m_serverPort;
    }

    int64_t Server::Client::Send(const void *dataBuf, size_t len) const {
        if(!m_hasConnected)
            return 0;
        return socketor::Send(dataBuf, len);
    }

    int64_t Server::Client::Send(const std::string &str) const {
        if(!m_hasConnected)
            return 0;
        return socketor::Send(str);
    }

    int Server::Client::closeConnect() {
        if(m_hasConnected){
            m_hasConnected = false;
            return closesocket(socketor::getRawSocket());
        }
        else
            return flag::NO_CONNECTED;
    }
}// namespace mysock
