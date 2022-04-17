#pragma once
#include "socketor.h"
#include <string>
#include <memory>



namespace mysock
{
    class Client : public socketor
    {
    public:
        explicit Client(const char *server_address = SERVER_ADDRESS, uint16_t port = DEFAULT_PORT);

        /**
         * @brief Set server port
         * @param port Server port
         * @return true if success, otherwise has connected to server
         **/
        bool setPort(uint16_t port);

        /**
         * @brief Set server address
         * @param address Server address
         * @return true if success, otherwise has connected to server
         **/
        bool setAddress(const std::string &address);

        // get server port
        uint16_t getPort() const;

        // get server address
        std::string getAddress() const;


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

        uint16_t server_port;

        std::shared_ptr<bool> hasConnected;

    public:
        //���������ʹ�õĳ�
        constexpr static char const * const SERVER_ADDRESS = "127.0.0.1"; //��������IP��ַ
        static const uint16_t DEFAULT_PORT = 5150;
    };
} // namespace mysock
