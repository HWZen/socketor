#pragma once
#include "socketor.h"
#include <vector>
#include <thread>
#include <unordered_map>
#include <memory>
#include <functional>
#include <atomic>


namespace mysock
{
    
    class Server : protected socketor
    {
    public:
        explicit Server(uint16_t Port = DEFAULT_PORT);

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

        class client : public socketor
        {
        private:
            uint16_t m_serverPort;

            bool m_hasConnected{true};
        public:
            using socketor::socketor;

            client& operator=(const client& other) = default;

            client& operator=(client&& other) noexcept = default;

            void setServerPort(uint16_t port);

            uint16_t getServerPort();

            int64_t Send(const void *dataBuf, size_t len) const override;

            int64_t Send(const std::string &str) const override;

            int closeConnect();
        };

        int accept();

        ~Server() override;

        Server(const Server&) = delete;

    private:
        bool hasListened{false};

        int rawAccept(client &socketBuf);


    public:
        // const static member
        static inline uint16_t DEFAULT_PORT = 5150;

    };
} // namespace mysock

