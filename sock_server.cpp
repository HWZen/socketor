#include "sock_server.h"
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

    server::server(int Port)
    {
#ifdef I_OS_WIN

        WSAStartup(0x0202, &wsaData);

#endif

        Socket_info.sin_family = AF_INET;
        Socket_info.sin_port = htons(Port);
        Socket_info.sin_addr.s_addr = htonl(INADDR_ANY);
        Socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

        hasListened = std::make_shared<bool>(false);
    }

    int server::Listen()
    {
        if (!hasListened)
            return EMPTY_OBJ;
        if (*hasListened)
            return LISTEN_SUCESS;

        int err = bind(Socket, (struct sockaddr*)&Socket_info, sizeof(SOCKADDR_IN));
        if (err == -1)
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

    int server::Accept(void (* call_back)(socketor))
    {
        socketor client;
        int err = rawAccept(client);
        if (err != SUCESS)
            return err;
        call_back(client);
        return SUCESS;

    }

    void server::close_server()
    {
        if (hasListened && *hasListened == true)
        {
            closesocket(Socket);
            *hasListened = false;
#ifdef I_OS_WIN
            WSACleanup();
#endif
        }
    }

    void server::close_connect(socketor s)
    {
        closesocket(s.getRawSocket());
    }

    server::server(server&& s) noexcept
    {
        this->Socket = s.Socket;
        s.Socket = 0;
        this->Socket_info = s.Socket_info;
        s.Socket_info = SOCKADDR_IN{};

        this->hasListened = s.hasListened;
        s.hasListened = nullptr;
    }

    server::~server()
    {
        if (hasListened.use_count() == 1 && *hasListened)
        {
            closesocket(Socket);
#ifdef I_OS_WIN
            WSACleanup();
#endif
        }

    }

    int server::rawAccept(socketor& socketBuf)
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
}// namespace mysock
