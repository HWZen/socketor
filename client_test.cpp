#include "sock_client.h"
#include <iostream>
#include <fast_io.h>

int main()
{
    try
    {
        mysock::Client client("hwzen.myds.me", 5150);

        bool check = client.connect2server();

        if(!check)
            throw(mysock::exception<std::string>("connect fail"));

        for(int i{}; i<1000000;++i)
        {
            client.rawSend((const void*)&i, sizeof(int));
//            char buf[2];
//            client.receive(buf,2);
//            println(buf[0]);
        }
        int i = -1;
        client.rawSend(&i, sizeof(int));
//        char buf[2];
//        client.receive(buf,2);
    }
    catch (mysock::exception<int> &e)
    {
        std::cerr << e.date() << std::endl << "error code: " << e.date() << std::endl;
    }
    catch (mysock::exception<std::string> &e)
    {
        std::cerr << e.date() << std::endl;
    }

    return 0;
}