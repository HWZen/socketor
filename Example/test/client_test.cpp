#include "sock_client.h"
#include <iostream>
#ifdef __cpp_concepts
#include <fast_io.h>
#else
template<typename ...Args>
decltype(auto) print(Args &&...args)
{
    return (std::cout << ... << args);
}

template<typename ...Args>
decltype(auto) println(Args &&...args)
{
    return (std::cout << ... << args) << "\n";
}

template<typename ...Args>
decltype(auto) perr(Args &&...args)
{
    return (std::cerr << ... << args) << "\n";
}

#endif //__cpp_concepts

int main()
{
    try
    {
        mysock::Client client("127.0.0.1", 5150);

        int check = client.connect2server();

        if(check != mysock::SUCESS)
            throw(mysock::exception<std::string>(std::string("connect fail: ") + std::to_string(check)));

        for(int i{}; i<1000000;++i)
        {
            client.rawSend((const void*)&i, sizeof(int));
        }
        int i = -1;
        client.rawSend(&i, sizeof(int));
        char buf[2];
        client.receive(buf,2);
    }
    catch (mysock::exception<int> &e)
    {
         perr(e.date(), "\n", "error code: ", e.describe());
    }
    catch (mysock::exception<std::string> &e)
    {
        perr(e.what());
    }

    return 0;
}