//
// Created by HWZ on 2022/4/15.
//
#include "gtest/gtest.h"
#include "socketor.h"
#include "osplatformutil.h"

#ifdef I_OS_WIN
class SocketTest : public mysock::socketor {
    public:
    static auto getCount(){
        return init_count.load();
    }
};
#endif // I_OS_WIN


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

#ifdef I_OS_WIN
TEST(wsaCountTest, default)
{
    auto **sock = new mysock::socketor*[5];
    for (int i = 0; i < 5; ++i)
    {
        EXPECT_NO_THROW(sock[i] = new mysock::socketor);
        EXPECT_EQ(SocketTest::getCount(), i + 1);

    }
    for (int i = 0; i < 5; ++i)
    {
        EXPECT_NO_THROW(delete sock[i]);
        EXPECT_EQ(SocketTest::getCount(), 5 - i - 1);
    }
}

TEST(wsaCountTest, socketCopy){
    mysock::socketor *first;
    EXPECT_NO_THROW(first = new mysock::socketor);
    auto **sock = new mysock::socketor*[5];
    for (int i = 0; i < 5; ++i)
    {
        EXPECT_NO_THROW(sock[i] = new mysock::socketor{*first});
        EXPECT_EQ(SocketTest::getCount(), i + 2);

    }
    for (int i = 0; i < 5; ++i)
    {
        EXPECT_NO_THROW(delete sock[i]);
        EXPECT_EQ(SocketTest::getCount(), 5 - i);
    }
    delete first;
}
#endif // I_OS_WIN