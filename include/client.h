#pragma once
#include "socketor.h"
#include <string>
#include <memory>
#include <atomic>


namespace mysock
{
    class Client : public socketor
    {
    public:
        explicit Client(const char *server_address = DEFAULT_SERVER, uint16_t port = DEFAULT_PORT);

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


        /**
         * @brief Connect to server, if address is domain name, it will be resolved to @member server_address
         * 
         * @return int error code
         */
        [[nodiscard]]int Connect2Server();

        /**
         * @brief Send data to server
         * 
         * @param str Send data
         */
        void Send(const std::string &str) const override
        {
            socketor::Send(str);
        }

        /**
         * @brief Send data to server with traditional mode
         *
         * @param str
         * @param len
         */
        void rawSend(const void *str, size_t len)
        {
            socketor::Send(str, len);
        }

        // close socket, if no connect
        void CloseConnect();

        ~Client() override;

    private:
        // server real address
        std::string server_address;

        // server port
        uint16_t server_port;

        // connect flag, thread safe
        std::shared_ptr<std::atomic_bool> hasConnected;

    public:
        // const static member
        constexpr static inline char const * const DEFAULT_SERVER = "127.0.0.1";
        static inline const uint16_t DEFAULT_PORT = 5150;
    };
} // namespace mysock
