//
// Created by HWZ on 2022/8/10.
//
#include <gtest/gtest.h>
#include <tinyServer.h>
#include "client.h"

TEST(tinyServerinit, init)
{
    mysock::TinyServer server;
}

TEST(tinyServerinit, init2)
{
    mysock::TinyServer server(8080);
}

TEST(tinyServerFunc, noListening)
{
    mysock::TinyServer server;
    EXPECT_EQ(server.startTask([](mysock::Server::Client&&,void *){}), mysock::NO_LISTENED);
}

TEST(tinyServerFunc, noBlockTest)
{
    mysock::TinyServer server;
    EXPECT_EQ(server.listen(), mysock::LISTEN_SUCESS);
    std::thread([&server](){
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
        server.stopTask();
    }).detach();
    EXPECT_EQ(server.startTask([](mysock::Server::Client&&,void *){}), mysock::SOCKETOR_SUCESS);
}

TEST(tinyServerFunc, accept)
{
    mysock::TinyServer server;
    EXPECT_EQ(server.listen(), mysock::LISTEN_SUCESS);
    std::thread([](){
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
        mysock::Client().Connect2Server();
    }).detach();

    EXPECT_EQ(server.startTask([](mysock::Server::Client&&,std::atomic_bool *stopSign){
        *stopSign = true;
    }), mysock::SOCKETOR_SUCESS);
}

TEST(tinyServerFunc, acceptTwice)
{
    mysock::TinyServer server;
    EXPECT_EQ(server.listen(), mysock::LISTEN_SUCESS);
    std::thread([](){
        std::this_thread::sleep_for(std::chrono::milliseconds(5));

        mysock::Client().Connect2Server();
        mysock::Client().Connect2Server();

    }).detach();

    EXPECT_EQ(server.startTask([](mysock::Server::Client&&,std::atomic_bool *stopSign){
        *stopSign = true;
    }), mysock::SOCKETOR_SUCESS);

    EXPECT_EQ(server.startTask([](mysock::Server::Client&&,std::atomic_bool *stopSign){
        *stopSign = true;
    }), mysock::SOCKETOR_SUCESS);


}

