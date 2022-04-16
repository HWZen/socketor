#include "server.h"
#include <memory>

#ifdef I_OS_LINUX
void WSACleanup(){}
#endif

namespace mysock
{
    void Accept_call_back(socketor s)
    {
        closesocket(s.getRawSocket());
    }

    Server::Server(uint16_t Port)
    {
#ifdef I_OS_WIN
        wsa_mutex.lock();
        if(wsaStartupCount == 0)
        {
            WSAStartup(0x0202, &wsaData);
        }
        ++wsaStartupCount;
        wsa_mutex.unlock();
#endif
        Socket_info.sin_family = AF_INET;
        Socket_info.sin_port = htons(Port);
        Socket_info.sin_addr.s_addr = htonl(INADDR_ANY);
        Socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

        hasListened = std::make_shared<bool>(false);
    }

    int Server::Listen()
    {
        if (!hasListened)
            return EMPTY_OBJ;
        if (*hasListened)
            return LISTEN_SUCESS;


        if (int err = bind(Socket, (struct sockaddr*)&Socket_info, sizeof(SOCKADDR_IN)); err == -1)
        {
            WSACleanup();
            return BIND_FAIL;
        }
        if (listen(Socket, 20) == -1)
        {
            WSACleanup();
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

    void Server::close_server()
    {
        if (hasListened && *hasListened == true)
        {
            closesocket(Socket);
            *hasListened = false;
        }
    }

    void Server::close_connect(socketor s)
    {
        closesocket(s.getRawSocket());
    }

    Server::Server(uint16_t Port) noexcept
    {
        this->Socket = Port.Socket;
        Port.Socket = 0;
        this->Socket_info = Port.Socket_info;
        Port.Socket_info = SOCKADDR_IN{};

        this->hasListened = Port.hasListened;
        Port.hasListened = nullptr;
    }

    Server::~Server()
    {
        if (hasListened.use_count() == 1 && *hasListened)
        {
            closesocket(Socket);
#ifdef I_OS_WIN
            wsa_mutex.lock();
            if (--wsaStartupCount == 0)
                WSACleanup();
#endif
        }

    }

    int Server::rawAccept(socketor& socketBuf)
    {
        if (!hasListened)
            return EMPTY_OBJ;
        if (!*hasListened)
            return NO_LISTENTED;

        SOCKADDR_IN rawClient;

        // ��ʼ������������accept()����Ϻ������ȳɹ����ܺ�Ż�������г���
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



}// namespace mysock
