#include "server.h"
#include <memory>

#ifdef I_OS_LINUX
void WSACleanup(){}
#endif

namespace mysock
{

    Server::Server(uint16_t Port)
    {

        Socket_info.sin_family = AF_INET;
        Socket_info.sin_port = htons(Port);
        Socket_info.sin_addr.s_addr = htonl(INADDR_ANY);
        socketor::Port = Port;
    }

    int Server::Listen()
    {
        if (!hasListened)
            return EMPTY_OBJ;
        if (*hasListened)
            return LISTEN_SUCESS;


        Socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (int err = bind(Socket, (struct sockaddr*)&Socket_info, sizeof(SOCKADDR_IN)); err == -1)
        {
            return BIND_FAIL;
        }
        if (listen(Socket, 20) == -1)
        {
            return LISTEN_FAIL;
        }
        *hasListened = true;
        return LISTEN_SUCESS;
    }

    int Server::Accept(void (* call_back)(socketor))
    {
        socketor client;
        int err = rawAccept(client);
        if (err != SUCESS)
            return err;
        call_back(client);
        return SUCESS;

    }

    int Server::Accept(socketor& client)
    {
        return rawAccept(client);
    }

    void Server::CloseConnect(socketor s)
    {
        closesocket(s.getRawSocket());
    }

    Server::~Server()
    {
        if (hasListened.use_count() == 1 && *hasListened)
        {
            closesocket(Socket);
            *hasListened = false;
        }


    }

    int Server::rawAccept(socketor& socketBuf)
    {
        if (!hasListened)
            return EMPTY_OBJ;
        if (!*hasListened)
            return NO_LISTENTED;

        SOCKADDR_IN rawClient;

        // 开始接受握手请求，accept()是阻断函数，等成功接受后才会继续进行程序
#ifdef I_OS_WIN
        int iaddrSize = sizeof(SOCKADDR_IN);
        SOCKET temp = accept(Socket, (struct sockaddr*)&rawClient, &iaddrSize);
        socketor client(temp, rawClient);

#endif

#ifdef I_OS_LINUX
        socklen_t length = sizeof(rawClient);
        socketor client(accept(Socket, (struct sockaddr *)&rawClient, &length), rawClient);
#endif
        socketBuf = client;
        return SUCESS;
    }

    uint16_t Server::getPort() const
    {
        return socketor::Port;
    }

    bool Server::setPort(uint16_t Port)
    {
        if(hasListened && *hasListened)
            return false;
        socketor::Port = Port;
        return true;
    }

    bool Server::isListen() const
    {
        return hasListened && *hasListened;
    }


}// namespace mysock
