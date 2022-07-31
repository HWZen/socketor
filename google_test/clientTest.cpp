//
// Created by HWZ on 2022/4/15.
//
#include "gtest/gtest.h"
#include "client.h"
#include "server.h"

class ClientTest : public ::testing::Test {
protected:
    mysock::Client *client;
    mysock::Server *server;
    ClientTest() {
        server = new mysock::Server{5150};
        if(int err = server->listen(); err != mysock::LISTEN_SUCESS)
        {
#ifdef I_OS_WIN
            auto wsaErr = WSAGetLastError();
            std::cerr << "Listen error: " << wsaErr << std::endl
                << "Error message: " << err << std::endl;
#endif // I_OS_WIN
            throw std::runtime_error("server listen error");
        }
    }

    ~ClientTest() override {
        delete server;
    }

    // If the constructor and destructor are not enough for setting up
    // and cleaning up each test, you can define the following methods:

    void SetUp() override {
        client = new mysock::Client{"127.0.0.1",5150};
    }

    void TearDown() override {
        delete client;
    }

    // Objects declared here can be used by all tests in the test case for Foo.
};

TEST(clientConstrcutor, default)
{
    mysock::Client client;
    EXPECT_EQ(client.getPort(), mysock::Client::DEFAULT_PORT);
    EXPECT_EQ(client.getAddress(), mysock::Client::DEFAULT_SERVER);
}

TEST(clientConstrcutor, param)
{
    mysock::Client client{"192.168.0.1",1234};
    EXPECT_EQ(client.getPort(), 1234);
    EXPECT_EQ(client.getAddress(), "192.168.0.1");
}

TEST_F(ClientTest, DNS)
{
    EXPECT_TRUE(client->setPort(443));
    client->setAddress("baidu.com");
    EXPECT_EQ(client->getAddress(), "baidu.com");
    auto err = client->Connect2Server();
    EXPECT_EQ(err, mysock::SUCESS);
    EXPECT_NE(client->getAddress(), "baidu.com");
    client->CloseConnect();
}

TEST_F(ClientTest, Connect)
{
    auto err = client->Connect2Server();
    EXPECT_EQ(err, mysock::SUCESS);
}

TEST_F(ClientTest, SetParm)
{
    client->setPort(1234);
    EXPECT_EQ(client->getPort(), 1234);
    client->setAddress("192.168.0.1");
    EXPECT_EQ(client->getAddress(), "192.168.0.1");
}

TEST_F(ClientTest, SetParmSafe)
{
    auto err = client->Connect2Server();
    EXPECT_EQ(err, mysock::SUCESS);
    err = client->setPort(1234);
    EXPECT_EQ(err, false);
    EXPECT_NE(client->getPort(), 1234);

    err = client->setAddress("192.168.0.1");
    EXPECT_EQ(err, false);
    EXPECT_NE(client->getAddress(), "192.168.0.1");
}

