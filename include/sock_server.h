#pragma once
#include "socketor.h"
#include <vector>
#include <thread>
#include <unordered_map>
#include <memory>

#define DEFAULT_PORT 5150

#ifdef I_OS_LINUX
#define SOCK_DEC
#endif

#ifdef I_OS_WIN
#ifdef DEC
#define SOCK_DEC  __declspec(dllexport)
#else
#define SOCK_DEC  __declspec(dllimport)
#endif // DEC
#endif // I_OS_WIN

namespace mysock
{

    void Accept_call_back(socketor s);
    class SOCK_DEC server : public socketor
    {
    public:
        explicit server(int Port = DEFAULT_PORT);

        server(const server &s) = default;

        server(server &&s) noexcept ;


        // ������������
        int Listen() noexcept(false);

        // �������Ӳ����ý��պ���
        int Accept(void(* call_back)(socketor));

        void Send(const std::string &str) override
        {
            socketor::Send(str);
        }

        void rawSend(const void *str, size_t len)
        {
            socketor::Send(str, len);
        }

        void close_connect();
        static void close_connect(socketor s);

        ~server();

    private:
#ifdef I_OS_WIN

#endif
        std::shared_ptr<bool> hasListened;



    };
} // namespace mysock

