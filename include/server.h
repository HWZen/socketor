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
        bool setPort(uint16_t Port);

        // get listen port
        uint16_t getPort() const;

        /**
         * @brief Start listen
         * 
         * @return int error code
         */
        int Listen() noexcept(false);

        // check if server has been listened
        bool isListen() const;

        /**
         * @brief Accept a new connection
         * 
         * @param call_back Callback function
         * @return int Error code
         */
        int Accept(void(* call_back)(socketor));

        /**
         * @brief
         *
         * @tparam Fn function type, class member function is also supported
         * @tparam Args function arguments
         * @param callBackFun Callback function, last argument must be
         * mysock::socketor
         * @param args function arguments, if Fn is class member
         * function, the first argument is the class instance
         * @return int Error code
         */
        template<typename Fn, typename ...Args>
        int Accept(Fn &&callBackFun, Args&& ...args){
            socketor client;
            int err = rawAccept(client);
            if (err != SUCESS)
                return err;
            std::invoke(callBackFun,args...,client);
            return SUCESS;
        }

        /**
         * @brief Accept a new connection
         * 
         * @param client Client socket
         * @return int Error code
         */
        int Accept(socketor& client);

        /**
         * @brief Close listen, if no listen, do nothing
         * 
         */
        void CloseServer();

        /**
         * @brief Close a connection
         * 
         * @param s A socketor
         *
         */
        static void CloseConnect(socketor s);


        ~Server() override;

    private:

        // socketor::hasConnected 's reference
        std::shared_ptr<std::atomic_bool> &hasListened{socketor::hasConnected};

        int rawAccept(socketor& socketBuf);


    public:
        // const static member
        static inline uint16_t DEFAULT_PORT = 5150;

    };
} // namespace mysock

