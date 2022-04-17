//
// Created by HWZ on 2022/4/15.
//
#include "gtest/gtest.h"
#include "client.h"

class sock_clientTest : public ::testing::Test {
protected:
    sock_clientTest() {
        // You can do set-up work for each test here.
    }

    ~sock_clientTest() override {
        // You can do clean-up work that doesn't throw exceptions here.
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
