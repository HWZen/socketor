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
            // ��ʼ������������accept()����Ϻ������ȳɹ����ܺ�Ż�������г���
#ifdef I_OS_WIN
            int iaddrSize = sizeof(SOCKADDR_IN);
            SOCKET temp = accept(Socket, (struct sockaddr *)&client, &iaddrSize);
            socketor Client(temp, client);
#endif

#ifdef I_OS_LINUX
            socklen_t length = sizeof(client);
            socketor Client(accept(Socket, (struct sockaddr *)&client, &length), client);
#endif
            // ���ܳɹ���
            // cout << "Accepted client:" << Client.address() << ":" << Client.port() << endl;
            // �½��̣߳������߳��н��к����Ľӷ�ͨ�Ų���

            RecThr = std::make_shared<std::thread>(call_back, Client);
            RecThr->detach();
        }
    } 
}// namespace mysock
