#pragma once
#include "socketor.h"
#include <string>

//���������ʹ�õĳ���
#define SERVER_ADDRESS "127.0.0.1" //��������IP��ַ
#define DEFAULT_PORT 6800          //�������Ķ˿ں�

#ifdef I_OS_LINUX
#define SOCK_DEC
#endif

#ifdef I_OS_WIN
#ifdef DEC
#define SOCK_DEC  __declspec(dllexport)
#else
#define SOCK_DEC  __declspec(dllimport)
#endif
#endif

namespace mysock
{
    class SOCK_DEC Client : public socketor
    {
    private:
#ifdef I_OS_WIN
        WSADATA wsaData{};
        size_t _msgSize;
#endif
        // ��������ʵ��ַ
        std::string Real_Addr;
    public:
        explicit Client(const char *server_address = SERVER_ADDRESS, int port = DEFAULT_PORT) noexcept(false);
        void Send(const std::string &str)
        {
            socketor::Send(str);
            Sleep(100);
        }

        // ������������
        bool connect2server();
    };
} // namespace mysock
