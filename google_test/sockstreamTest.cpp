//
// Created by HWZ on 2022/4/16.
//
#define __ENABLE 1
#if __ENABLE

#include "gtest/gtest.h"
#include "server.h"
#include "client.h"
#include "sockstream.h"
#include <iostream>
#include <thread>
#include <cmath>

using namespace mysock;
using namespace std;

class SockstreamTest : public ::testing::Test
{
protected:
    static inline Server server{5150};
    static inline Client client{"127.0.0.1", 5150};
    static inline socketor sock{};
    osockstream sss{&client};
    isockstream ssr{&sock};

    SockstreamTest()
    {
        if(server.isListen())
            return;
        if(auto err = server.Listen(); err != SUCESS)
        {
            std::cerr << "server listen error: " << err << std::endl;
#ifdef I_OS_WIN
            std::cerr << "error code: " << WSAGetLastError() << std::endl;
#endif
            throw std::bad_alloc();
        }

        std::thread th([&](){
            if(auto err = server.Accept(sock); err != SUCESS)
            {
                std::cerr << "server accept error: " << err << std::endl;
                throw std::bad_alloc();
            }
            else
            {
                std::cout << "server accept success" << std::endl;
            }
        });

        if(auto err = client.Connect2Server(); err != SUCESS)
        {
            std::cerr << "client connect error: " << err << std::endl;
#ifdef I_OS_WIN
            std::cerr << "error code: " << WSAGetLastError() << std::endl;
            system(("net helpmsg " + std::to_string(err)).c_str());
#endif
            throw std::bad_alloc();
        }
        th.join();
    }

    ~SockstreamTest() override
    = default;

    // If the constructor and destructor are not enough for setting up
    // and cleaning up each test, you can define the following methods:

    void SetUp() override
    {
        // Code here will be called immediately after the constructor (right
        // before each test).
    }

    void TearDown() override
    {
        // Code here will be called immediately after each test (right
        // before the destructor).
    }

    // Class members declared here can be used by all tests in the test case.
};

TEST_F(SockstreamTest, String2String)
{
    sss << "hello world"s;
    string str;
    ssr >> str;
    EXPECT_EQ(str, "hello");
    ssr >> str;
    EXPECT_EQ(str, "world");
}

TEST_F(SockstreamTest, String2Int)
{
    sss << "12345"s;
    int i;
    ssr >> i;
    EXPECT_EQ(i, 12345);
}

TEST_F(SockstreamTest, Int2String)
{
    sss << 12345;
    string str;
    ssr >> str;
    EXPECT_EQ(str, "12345"s);
}

TEST_F(SockstreamTest, Int2Int)
{
    sss << 12345;
    int i;
    ssr >> i;
    EXPECT_EQ(i, 12345);
}

TEST_F(SockstreamTest, Double2String)
{
    sss.precision(4);
    sss.setf(ios::fixed);
    ssr.precision(4);
    ssr.setf(ios::fixed);
    sss << 12345.6789;
    string str;
    ssr >> str;
    EXPECT_EQ(str, "12345.6789"s);
}

TEST_F(SockstreamTest, String2Double)
{
    sss.precision(4);
    sss.setf(ios::fixed);
    ssr.precision(4);
    ssr.setf(ios::fixed);
    sss << "12345.6789"s;
    double d;
    ssr >> d;
    EXPECT_DOUBLE_EQ(d, 12345.6789);
}

TEST_F(SockstreamTest, Double2Double)
{
    sss.precision(4);
    sss.setf(ios::fixed);
    ssr.precision(4);
    ssr.setf(ios::fixed);
    sss << 12345.6789;
    double d;
    ssr >> d;
    EXPECT_DOUBLE_EQ(d, 12345.6789);
}

TEST_F(SockstreamTest, Int2Double){
    sss.precision(4);
    sss.setf(ios::fixed);
    ssr.precision(4);
    ssr.setf(ios::fixed);
    sss << 12345;
    double d;
    ssr >> d;
    EXPECT_DOUBLE_EQ(d, 12345);
}

TEST_F(SockstreamTest, Double2Int){
    sss.precision(4);
    sss.setf(ios::fixed);
    ssr.precision(4);
    ssr.setf(ios::fixed);
    sss << 12345.6789;
    int i;
    ssr >> i;
    EXPECT_EQ(i, 12345);
}

TEST_F(SockstreamTest, complex)
{
    sss << "123 Hello"s;
    int i;
    ssr >> i;
    EXPECT_EQ(i, 123);
    string str;
    ssr >> str;
    EXPECT_EQ(str, "Hello"s);
}

#endif // __ENABLE