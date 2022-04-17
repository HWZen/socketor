#pragma once
#include "socketor.h"
#include <string>
#include <memory>



namespace mysock
{
    class Client : public socketor
    {
    public:
        explicit Client(const char *server_address = SERVER_ADDRESS, int port = DEFAULT_PORT);

        // ������������
        [[nodiscard]]int Connect2Server();

        void Send(const std::string &str) const override
        {
            socketor::Send(str);
        }


        void rawSend(const void *str, size_t len)
        {
            socketor::Send(str, len);
        }

        void CloseConnect();

        ~Client() override;

    private:
#ifdef I_OS_WIN
        WSADATA wsaData{};
#endif
        // ��������ʵ��ַ
        std::string server_address;

        int server_port;

        std::shared_ptr<bool> hasConnected;

    public:
        //���������ʹ�õĳ�
        constexpr static char const * const SERVER_ADDRESS = "127.0.0.1"; //��������IP��ַ
        static const int DEFAULT_PORT = 5150;
    };
} // namespace mysock
