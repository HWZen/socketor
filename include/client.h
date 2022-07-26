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
         *
         * @param port Server port
         *
         * @return true if success, otherwise has connected to server
         **/
        bool setPort(uint16_t port);

        /**
         * @brief Set server address
         *
         * @param address Server address
         *
         * @return true if success, otherwise has connected to server
         **/
        bool setAddress(const std::string &address);

        /**
         *
         * @return client port
         */
        uint16_t getPort() const;

        /**
         * @brief get server address
         */
        std::string getAddress() const;


        /**
         * @brief Connect to server, if address is domain name, it will be resolved to @member server_address
         * 
         * @return int error code
         */
        int Connect2Server();

        /**
         * @brief Send data to server
         *
         * @param str Send data
         *
         * @return sent len
         * @retval 0 no connected
         * @retval -1 sent fail
         */
        int64_t Send(const std::string &str) const override
        {
            if(hasConnected)
                return socketor::Send(str);
            return 0;
        }

        /**
         * @brief Send data to server with classical type
         *
         * @param str
         * @param len
         *
         * @return sent len
         * @retval 0 no connected
         * @retval -1 sent fail
         */
        int64_t Send(const void *str, size_t len) const override
        {
            if(hasConnected)
                return socketor::Send(str, len);
            return 0;
        }

        /**
         * @brief close socket, if no connect
         */
        void CloseConnect();

        ~Client() override;

        Client(const Client&) = delete;

    private:
        // server real address
        std::string server_address;

        // server port
        uint16_t server_port;

        // server
        bool hasConnected{false};

    public:
        // const static member
        constexpr static inline char const * const DEFAULT_SERVER = "127.0.0.1";
        static inline const uint16_t DEFAULT_PORT = 5150;
    };
} // namespace mysock
