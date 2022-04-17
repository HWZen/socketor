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
    class Server : protected socketor
    {
    public:
        explicit Server(uint16_t Port = DEFAULT_PORT);

        /**
         * @brief Set listen port
         * @param port listen port
         * @return true if success, otherwise server has been listened
         * */
        bool setPort(uint16_t Port);

        // get listen port
        uint16_t getPort() const;

        // 开启监听函数
        int Listen() noexcept(false);

        bool isListen() const;

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

        int Accept(socketor& client);

        void CloseServer();
        static void CloseConnect(socketor s);


        ~Server() override;

    private:

        std::shared_ptr<std::atomic_bool> hasListened;

        int rawAccept(socketor& socketBuf);


    public:
        static const uint16_t DEFAULT_PORT = 5150;

    };
} // namespace mysock

