#pragma once
#include "socketor.h"
#include <vector>
#include <thread>
#include <unordered_map>
#include <memory>
#include <functional>
#include <atomic>
#include <optional>


namespace mysock
{
    
    class Server : protected socketor
    {
    public:
        explicit Server(uint16_t Port = DEFAULT_PORT);

        Server(Server &&) noexcept;

        /**
         * @brief Set listen port
         * @param port listen port
         * @return true if success, otherwise server has been listened
         * */
        constexpr bool setPort(uint16_t Port);

        /**
         * @brief Get listen port
         * @return uint16_t Listen port
         */
        constexpr uint16_t getPort() const{
            return socketor::port();
        }

        /**
         * @brief Start listen
         * 
         * @return int error code
         */
        int listen() noexcept(false);

        /**
         * @brief Check if the service is listening
         *
         * @retval true Is listening
         * @retval false Not listening
         */
        bool isListen() const;

        /**
         * @brief Close listening
         *
         * @return int error code
         * @retval flag::NO_LISTENED server no listened
         */
        int closeListen();

        class Client : public socketor
        {
            friend Server;
        private:
            uint16_t m_serverPort{Server::DEFAULT_PORT};

            bool m_hasConnected{true};
        public:
            using socketor::socketor;

            Client(Client &&) noexcept = default;

            Client(const Client &) = default;

            Client& operator=(const Client& other) = default;

            Client& operator=(Client&& other) noexcept = default;

            void setServerPort(uint16_t port);

            uint16_t getServerPort();

            int64_t Send(const void *dataBuf, size_t len) const override;

            int64_t Send(const std::string &str) const override;

            bool hasConnected(){
                return m_hasConnected;
            }

            int closeConnect();
        };

        /**
         * @brief accept a connection
         * @return client socketor, hasConnected() will return true if accept success
         */
        Server::Client accept();

        ~Server() override;

        Server(const Server&) = delete;

    private:
        bool m_hasListened{false};

        int rawAccept(Client &socketBuf);


    public:
        // const static member
        static inline uint16_t DEFAULT_PORT = SOCKETOR_DEFAULT_PORT;

    };
} // namespace mysock

