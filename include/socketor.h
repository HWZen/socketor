//
// Created by HWZ on 2021/1/19.
//

#ifndef SOCKETOR_H
#define SOCKETOR_H

#include <string>
#include <memory>
#include <atomic>
#include <stdexcept>
#include "osplatformutil.h"

#ifdef I_OS_WIN

#include <WinSock2.h>


#pragma comment(lib, "ws2_32.lib")

#endif // I_OS_WIN

#define MAX_SOCKET_SIZE 0x10000

#ifdef I_OS_LINUX

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/shm.h>

typedef int SOCKET;
typedef struct sockaddr_in SOCKADDR_IN;
#define closesocket(x) close(x)
#define SOCKET_ERROR -1

#define TRUE 1

#endif // I_OS_LINUX

namespace mysock {
    enum flag {
        SUCESS,
        LISTEN_SUCESS = SUCESS,
        BIND_FAIL,
        LISTEN_FAIL,
        CONNECT_FAIL,
        EMPTY_OBJ,
        NO_LISTENED,
        NO_CONNECTED,
        BYTE_FAIL,
        WSA_ERROR,
        GET_HOST_NAME_FAIL
    };

    class socketor {
    protected:
        SOCKADDR_IN Socket_info{};
        std::string Address{};
        uint16_t Port{};

        SOCKET Socket{};

    public:
        socketor() = default;

        socketor(const socketor &) = default;

        socketor(socketor &&) noexcept = default;

        socketor &operator=(const socketor &) = default;

        socketor &operator=(socketor &&) noexcept = default;

        /**
         * @brief construct with sock fd
         *
         * @param socket_fd     socket fd
         * @param socket_info   socket info
         */
        socketor(SOCKET socket_fd, SOCKADDR_IN socket_info);

        virtual ~socketor() = default;

        /**
         * @brief Send data
         *
         * @param dataBuf data buffer
         * @param len data length
         *
         * @return sent len
         * @retval -1 sent fail
         */
        virtual int64_t Send(const void *dataBuf, size_t len) const;

        /**
         * @brief Send string
         * 
         * @param str String
         *
         * @return sent len
         * @retval -1 sent fail
         */
        virtual int64_t Send(const std::string &str) const;

        /**
         * @brief Receive data, block
         * 
         * @param buf Receive buffer
         * @param len Receive length
         *
         * @return int64_t Receive length
         * @retval -1 received error
         */
        virtual int64_t receive(void *buf, size_t len) const;

        /**
         * @brief receive data as string
         * 
         * @return std::string
         * @retval "" receive error
         */
        virtual std::string receive() const;


        /**
         * @brief get connect address
         *
         * @return address str
         */
        std::string address() const {
            return Address;
        };

        /**
         * @brief get connect port
         *
         * @return connect port
         */
        constexpr uint16_t port() const {
            return Port;
        };

        /**
         * @brief set socket receive timeout
         *
         * @param timeout (sec)
         * @retval flag::SUCCESS    set ok
         * @retval SOCKET_ERROR     set fail
         *
         */
        [[nodiscard("should be verified")]]int setRecvTimeout(int timeout);

        /**
         * @brief set socket send timeout
         *
         * @param timeout (sec)
         * @retval flag::SUCCESS    set ok
         * @retval SOCKET_ERROR     set fail
         */
        [[nodiscard("should be verified")]]int setSendTimeout(int timeout);

        /**
         * @brief Get the Raw Socket fd
         * 
         * @return socket fd
         */
        SOCKET getRawSocket() {
            return Socket;
        }

    protected:
        // windows platform, startUp wsa
#ifdef I_OS_WIN

        class WsaStartUpError : public std::exception {
        public:
            using std::exception::exception;
        };

        class WsaInitClass {
        private:

            WsaInitClass() {
                WORD wVersionRequested = 0;

                wVersionRequested = MAKEWORD(2, 2);
                WSAData wsaData;
                auto err = WSAStartup(wVersionRequested, &wsaData);
                if (err != 0) {
                    throw WsaStartUpError("WSAStartup failed");
                }

                if (LOBYTE(wsaData.wVersion) != 2 ||
                    HIBYTE(wsaData.wVersion) != 2) {
                    WSACleanup();
                    throw WsaStartUpError("byte alignment failed");
                }
            }

        public:
            ~WsaInitClass() {
                WSACleanup();
            }

            static void WsaInit() {
                static WsaInitClass ins;
            }

            WsaInitClass(const WsaInitClass &) = delete;
        };

        static void wsaInit() {
            WsaInitClass::WsaInit();
        }
#endif // I_OS_WIN
    };




} // namespace mysock

#endif //SOCKETOR_H
