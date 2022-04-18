//
// Created by HWZ on 2021/1/19.
//

#ifndef SOCKETOR_H
#define SOCKETOR_H

#include <string>
#include <memory>
#include <atomic>
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


#ifdef __GNUC__

namespace std
{
    class msvc_exception
    {
    protected:
        const string describe;
    public:
        msvc_exception(string str) : describe(std::move(str))
        {
        }

        msvc_exception() : describe("no describe")
        {
        }

        virtual const char* what() const
        {
            return describe.c_str();
        }
    };

#define exception msvc_exception
}

#endif

namespace mysock
{
    enum flag
    {
        SUCESS,
        LISTEN_SUCESS = SUCESS,
        BIND_FAIL,
        LISTEN_FAIL,
        CONNECT_FAIL,
        EMPTY_OBJ,
        NO_LISTENTED,
        BYTE_FAIL,
        WSA_ERROR,
        GET_HOST_NAME_FAIL
    };

    class socketor
    {
    protected:
        SOCKADDR_IN Socket_info{};
        std::string Address{};
        uint16_t Port{};

        SOCKET Socket{};

        // connect flag, thread safe
        std::shared_ptr<std::atomic_bool> hasConnected;
    public:
        socketor();

        // copy constructor
        socketor(const socketor&);

        // move constructor
        socketor(socketor&&) noexcept;

        // copy assignment
        socketor& operator=(const socketor&);

        socketor(SOCKET target_socket, SOCKADDR_IN socket_info);

        virtual ~socketor();


        /**
         * @brief Send data
         * 
         * @param dataBuf data buffer
         * @param len data length
         */
        virtual void Send(const void* dataBuf, size_t len) const;

        /**
         * @brief Send string
         * 
         * @param str String
         */
        virtual void Send(const std::string& str) const;

        /**
         * @brief Receive data, block
         * 
         * @param buf Receive buffer
         * @param len Receive length
         * @return int64_t Receive length, -1 if error
         *
         */
        virtual int64_t receive(void* buf, size_t len) const;

        /**
         * @brief receive string, block
         * 
         * @return std::string, empty string if error
         * 
         */
        virtual std::string receive() const;


        // get socket address
        std::string address() const
        {
            return Address;
        };

        // get socket port
        uint16_t port() const
        {
            return Port;
        };

        // set receive timeout
        [[nodiscard("should be verified")]]auto setRecvTimeout(int timeout)
        {
#ifdef I_OS_LINUX
            struct timeval tv;
            tv.tv_sec = timeout / 1000;
            tv.tv_usec = (timeout % 1000) * 1000;
            return setsockopt(Socket, SOL_SOCKET, SO_RCVTIMEO, (char*)&tv, sizeof(struct timeval));
#endif // I_OS_LINUX

#ifdef I_OS_WIN
            return setsockopt(Socket, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(timeout));
#endif // I_OS_WIN

        }

        // set send timeout
        [[nodiscard("should be verified")]]auto setSendTimeout(int timeout)
        {
#ifdef I_OS_LINUX
            struct timeval tv;
            tv.tv_sec = timeout / 1000;
            tv.tv_usec = (timeout % 1000) * 1000;
            return setsockopt(Socket, SOL_SOCKET, SO_SNDTIMEO, (char*)&tv, sizeof(struct timeval));
#endif // I_OS_LINUX

#ifdef I_OS_WIN
            return setsockopt(Socket, SOL_SOCKET, SO_SNDTIMEO, (char*)&timeout, sizeof(timeout));
#endif // I_OS_WIN
        }

        /**
         * @brief Get the Raw Socket object
         * 
         * @return SOCKET
         */
        SOCKET getRawSocket()
        {
            return Socket;
        }

    protected:
        // windows platform, wsa count
#ifdef I_OS_WIN

        static inline std::atomic_uint init_count{0};

        static inline WSADATA wsaData;

        static void WSASTARTUP();

        static void WSACLEANUP();

#endif // I_OS_WIN

    };



} // namespace mysock

#endif //SOCKETOR_H
