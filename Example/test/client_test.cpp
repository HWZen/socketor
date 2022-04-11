#include "sock_client.h"
#include <string>

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
#endif //__cpp_concepts

template<can_print ...Args>
decltype(auto) print(Args&& ...args)
{
    return (std::cout << ... << args);
}

template<can_print ...Args>
decltype(auto) println(Args&& ...args)
{
    return (std::cout << ... << args) << "\n";
}

template<can_print ...Args>
decltype(auto) perr(Args&& ...args)
{
    return (std::cerr << ... << args) << "\n";
}

#endif //__has_include("fast_io.h") && defined(__cpp_concepts)

int main(int argc, char **argv) try
{
    std::string hostName = argc > 1 ? argv[1] : "127.0.0.1";

    mysock::Client client(hostName.c_str(), 5150);

    int check = client.connect2server();

    if (check != mysock::SUCESS)
        throw (mysock::exception<std::string>(std::string("connect fail: ") + std::to_string(check)));

    for (int i{}; i < 1000000; ++i)
    {
        client.rawSend((const void*)&i, sizeof(int));
    }
    int i = -1;
    client.rawSend(&i, sizeof(int));
    char buf[2];
    client.receive(buf, 2);


    return 0;
}
catch (mysock::exception<int>& e)
{
    perr(e.date(), "\n", "error code: ", e.describe());
}
catch (mysock::exception<std::string>& e)
{
    perr(e.what());
}