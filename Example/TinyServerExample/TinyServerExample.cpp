//
// Created by HWZ on 2022/8/10.
//
#include <tinyServer.h>
#include <client.h>
#include <iostream>
#define connectTimes 100

int main()
{

    mysock::TinyServer server;
    if(server.listen() != mysock::LISTEN_SUCESS){
        std::cerr << "listen error!\n";
        return 1;
    }
    auto serverThread = std::thread([&](){
        auto res = server.startTask([](const mysock::Server::Client &cli, std::atomic_bool *pStopSign){
            static size_t size{0};
            cli.Send(std::to_string(size) + ": Sent by server");
            std::cout << cli.receive() << "\n";
            if(++size >= connectTimes)
                *pStopSign = true;
            return;
        });
        std::cout << "Server return code: " << res << "\n";
    });

    auto clientThread = std::thread([&](){
        for(size_t size{}; size < connectTimes; ++size){
            mysock::Client cli;
            if (cli.Connect2Server() != mysock::SOCKETOR_SUCESS){
                std::cerr << "connect fail!\n";
            }
            std::cout << cli.receive() << "\n";
            cli.Send( std::to_string(size) + ": sent by client");
        }
    });
    clientThread.join();
    serverThread.join();
}