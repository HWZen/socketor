#pragma once
#include "socketor.h"
#include <vector>
#include <thread>
#include <unordered_map>
#include <memory>
#include <functional>


namespace mysock
{

    void Accept_call_back(socketor s);
    class server : protected socketor
    {
    public:
        explicit server(int Port = DEFAULT_PORT);



        server(const server &s) = default;

        server(server &&s) noexcept ;


        // 开启监听函数
        int Listen() noexcept(false);

        // 受理连接并调用接收函数
        int Accept(void(* call_back)(socketor));

        template<typename Fn, typename ...Args>
        int Accept(Fn &&callBackFun, Args&& ...args){
            socketor client;
            int err = rawAccept(client);
            if (err != SUCESS)
                return err;
            std::invoke(callBackFun,args...,client);
            return SUCESS;
        }

        void close_server();
        static void close_connect(socketor s);


        ~server() override;

    private:
#ifdef I_OS_WIN
        WSADATA wsaData{};
#endif
        std::shared_ptr<bool> hasListened;

        int rawAccept(socketor& socketBuf);

    public:
        static const int DEFAULT_PORT = 5150;

    };
} // namespace mysock

