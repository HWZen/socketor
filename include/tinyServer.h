//
// Created by HWZ on 2022/8/9.
//

#ifndef SOCKETOR_TINYSERVER_H
#define SOCKETOR_TINYSERVER_H

#include "server.h"
#include <thread>


#include <cassert>
#include <mutex>

#ifdef I_OS_LINUX
#include <sys/ioctl.h>
#endif

namespace mysock{
    class TinyServer : protected Server
    {
    public:
        using Server::listen;
        using Server::isListen;
        using Server::closeListen;
        using Server::setPort;
        using Server::getPort;

        explicit TinyServer(uint16_t Port = DEFAULT_PORT);


        template<class Fn, typename ...Args>
        int startTask(Fn &&fn, Args &&...args);

        void stopTask(){
            stopSign = true;
        }

    private:
        std::atomic_bool stopSign{ false};
        std::mutex taskMut;
    };




    template<class Fn, typename... Args>
    int TinyServer::startTask(Fn&& fn, Args&& ... args)
    {
        // tasks are not allowed to run in parallel.
        std::lock_guard lg(taskMut);

        stopSign = false;
        while(!stopSign){
            if(isListen()){
                auto cli{accept()};
                if(cli.hasConnected()){
#ifdef I_OS_WIN // On Windows, a connection is accepted from a No-block listener socket, the connection socket is also No-block.
                // There we set the socket to block type.
                    u_long ul = 0;
                    ioctlsocket(cli.getRawSocket(), FIONBIO, &ul);
#endif
                    std::thread(fn, std::move(cli), &stopSign, std::forward<Args>(args)...).detach();
                }
                else{
                    std::this_thread::sleep_for(std::chrono::milliseconds(10));
                }
            }
            else
                return NO_LISTENED;
        }
        return SOCKETOR_SUCESS;
    }
}

#endif //SOCKETOR_TINYSERVER_H
