//
// Created by HWZ on 2022/4/15.
//
#include <gtest/gtest.h>
#include "server.h"
#include "client.h"
#include "osplatformutil.h"

class ServerTest : public ::testing::Test {
protected:
    mysock::Server *server;
    ServerTest() {
        server = new mysock::Server{};
    }

    ~ServerTest() override {
        delete server;
    }

    // If the constructor and destructor are not enough for setting up
    // and cleaning up each test, you can define the following methods:

    void SetUp() override {
        // Code here will be called immediately after the constructor (right
        // before each test).
    }

    void TearDown() override {
        // Code here will be called immediately after each test (right
        // before the destructor).
    }

    // Objects declared here can be used by all tests in the test case for Foo.
};

// Construct test
TEST(serverConstruct, Default) {
    mysock::Server server{};
    EXPECT_EQ(server.getPort(), mysock::Server::DEFAULT_PORT);
    EXPECT_EQ(server.isListen(), false);
}

TEST(serverConstruct, parm) {
    mysock::Server server{1234};
    EXPECT_EQ(server.getPort(), 1234);
    EXPECT_EQ(server.isListen(), false);
}

TEST_F(ServerTest, serverListen) {
    bool listenStatus = server->listen() == mysock::SOCKETOR_SUCESS;
    EXPECT_EQ(listenStatus, true);
#ifdef I_OS_WIN
    if(!listenStatus) {
        std::cerr << WSAGetLastError() << std::endl;
    }
#endif // I_OS_WIN
    EXPECT_EQ(server->isListen(), listenStatus);
}

TEST_F(ServerTest, serverAccept) {
    EXPECT_EQ(server->listen(), mysock::SOCKETOR_SUCESS);
    mysock::Client cl;
    std::thread(&mysock::Client::Connect2Server, &cl).detach();
    auto cli = server->accept();
    EXPECT_EQ(cli.hasConnected(), true);
    EXPECT_EQ(cli.address(), "127.0.0.1");
    cli.closeConnect();
}

