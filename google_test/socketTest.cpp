//
// Created by HWZ on 2022/4/15.
//
#include "gtest/gtest.h"
#include "socketor.h"


TEST(socketorTest, AddressName)
{
    mysock::socketor sock;
    EXPECT_EQ(sock.address(), "");
}

TEST(socketorTest, AddressPort)
{
    mysock::socketor sock;
    EXPECT_EQ(sock.port(), 0);
}