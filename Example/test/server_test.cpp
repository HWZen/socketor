#include "sock_server.h"
#include <string>
#include "function.h"
#if __has_include("fast_io.h") && defined(__cpp_concepts)
#include <fast_io.h>
#else

#include <iostream>
#ifdef __cpp_concepts
template <typename T>
concept can_print = requires(std::ostream os, T ty)
{
    os << ty;
};
#else
#define can_print typename
#endif// __cpp_concepts

template<can_print ...Args>
decltype(auto) print(Args &&...args)
{
    return (std::cout << ... << args);
}

template<can_print ...Args>
decltype(auto) println(Args &&...args)
{
    return (std::cout << ... << args) << "\n";
}

template<can_print ...Args>
decltype(auto) perr(Args &&...args)
{
    return (std::cerr << ... << args) << "\n";
}

#endif //__has_include("fast_io.h") && defined(__cpp_concepts)

using namespace std;

void foo(mysock::socketor client);

int main()
{
    mysock::server Server(5150);
    try
    {
        if(int err = Server.Listen(); err != mysock::SUCESS)
            throw mysock::exception<mysock::flag>(mysock::flag(err), "Listen fail");
        println("Listening");
        Server.Accept(foo);
        println("waiting for accept");
    }
    catch (mysock::exception<mysock::flag> &e)
    {
        switch (e.date())
        {
        case mysock::flag::BIND_FAIL:
            perr(EnumName<mysock::flag,mysock::flag::BIND_FAIL>());
            break;
        case mysock::flag::LISTEN_FAIL:
            perr(EnumName<mysock::flag,mysock::flag::LISTEN_FAIL>());
            break;
        case mysock::LISTEN_SUCESS:
            break;
        default:
            perr("unknow error: ", e.date());
            perr("error description: ", e.what());
            break;
        }
    }
    catch (std::exception &e)
    {
        perr(e.what());
    }

}

void foo(mysock::socketor client)
{
    println("get accept: ", client.address());
    int cnt = 0;
    vector<int> error_buf;

    int *buf = new int{};
    do{
        if(int i = client.receive(buf,4);i <= SOCKET_ERROR){
            perr("recv err: ", i);
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