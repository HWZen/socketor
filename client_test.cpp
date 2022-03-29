#include "sock_client.h"
#include <iostream>
#include <fast_io.h>


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
//            char buf[2];
//            client.receive(buf,2);
//            println(buf[0]);
        }
        int i = -1;
        client.rawSend(&i, sizeof(int));
        char buf[2];
        client.receive(buf,2);
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