#include "sock_server.h"
#include <memory>
#include <thread>

namespace mysock
{
    void Accept_call_back(socketor s) { s.close_connect(); }
    server::server(int Port)
    {
#ifdef I_OS_WIN
        WSAStartup(0x0202, &wsaData);
#endif
        Socket_info.sin_family = AF_INET;
        Socket_info.sin_port = htons(Port);
        Socket_info.sin_addr.s_addr = htonl(INADDR_ANY);
        Socket =socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    }

    int server::Listen()
    {
        if (bind(Socket, (struct sockaddr *)&Socket_info, sizeof(SOCKADDR_IN)) == -1)
            throw exception<flag>(BIND_FAIL, "Listen fail");
        if (listen(Socket, 20) == -1)
            throw exception<flag>(LISTEN_FAIL, "Listen fail");
        return LISTEN_SUCESS;
    }

    void server::Accept(void (*call_back)(socketor))
    {
        SOCKADDR_IN client;

        std::shared_ptr<std::thread> RecThr;


        while (TRUE)
        {
            // 开始接受握手请求，accept()是阻断函数，等成功接受后才会继续进行程序
#ifdef I_OS_WIN
            int iaddrSize = sizeof(SOCKADDR_IN);
            SOCKET temp = accept(Socket, (struct sockaddr *)&client, &iaddrSize);
            socketor Client(temp, client);
#endif

#ifdef I_OS_LINUX
            socklen_t length = sizeof(client);
            socketor Client(accept(Socket, (struct sockaddr *)&client, &length), client);
#endif
            // 接受成功后：
            // cout << "Accepted client:" << Client.address() << ":" << Client.port() << endl;
            // 新建线程，在新线程中进行后续的接发通信操作

            RecThr = std::make_shared<std::thread>(call_back, Client);
            RecThr->detach();
        }
    } 
}// namespace mysock
