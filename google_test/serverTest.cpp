//
// Created by HWZ on 2022/4/15.
//
#include "gtest/gtest.h"
#include "server.h"

class sock_serverTest : public ::testing::Test {
protected:
    mysock::Server *server;
    sock_serverTest() {
        server = new mysock::Server{};
    }

    ~sock_serverTest() override {
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
}

TEST(serverConstruct, parm) {
    mysock::Server server{1234};
    EXPECT_EQ(server.getPort(), 1234);
}