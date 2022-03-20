#include "sock_server.h"
#include <iostream>
#include <string>
#include "function.h"
#include <fast_io.h>

using namespace std;

void foo(mysock::socketor client);

int main()
{
    mysock::server Server(5150);
    try
    {
        Server.Listen();
        println("Listening");
        Server.Accept(foo);
        println("waiting for accept");
    }
    catch (mysock::exception<mysock::flag> &e)
    {
        switch (e.date())
        {
        case mysock::flag::BIND_FAIL:
            cout << EnumName<mysock::flag,mysock::flag::BIND_FAIL>() << endl;
            break;
        case mysock::flag::LISTEN_FAIL:
            cout << EnumName<mysock::flag,mysock::flag::LISTEN_FAIL>() << endl;
            break;
        case mysock::LISTEN_SUCESS:
            break;
        }
    }
    catch (std::exception &e)
    {
        cerr << e.what() << endl;
    }

}

void foo(mysock::socketor client)
{
    println("get accept: ", client.address());
    int cnt = 0;
    vector<int> error_buf;

    int *buf = new int{};
    do{
        if(int i = client.receive(buf,4);i == SOCKET_ERROR){
            println(fast_io::c_stderr(), "recv err: ", i);
            break;
        }
        if(*buf == -1)
            break;
        if(cnt != *buf)
        {
            error_buf.push_back(*buf);
            println(*buf);
            cnt = *buf;
        }
        ++cnt;


    }
    while(*buf != -1);

    println("lose size: ", error_buf.size());

}