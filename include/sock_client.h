#pragma once
#include "socketor.h"
#include <string>
#include <memory>

//���������ʹ�õĳ���
#define SERVER_ADDRESS "127.0.0.1" //��������IP��ַ
#define DEFAULT_PORT 6800          //�������Ķ˿ں�

#ifdef I_OS_LINUX
#define SOCK_DEC
#endif

namespace mysock
{
    class Client : public socketor
    {
    public:
        explicit Client(const char *server_address = SERVER_ADDRESS, int port = DEFAULT_PORT);

        // ������������
        [[nodiscard]]int connect2server();

        void Send(const std::string &str) override
        {
            socketor::Send(str);
        }


        void rawSend(const void *str, size_t len)
        {
            socketor::Send(str, len);
        }

        void close_connect();

    private:
#ifdef I_OS_WIN
        WSADATA wsaData{};
        size_t _msgSize{};
#endif
        // ��������ʵ��ַ
        std::string server_address;

        int server_port;

        std::shared_ptr<bool> hasConnected;
    };
} // namespace mysock
