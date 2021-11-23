#include "sock_client.h"
#include <iostream>

int main()
{
    try
    {
        mysock::Client client("192.168.5.149", 5150);

        bool check = client.connect2server();

        if(!check)
            throw(mysock::exception<std::string>("connect fail"));

        client.Send("ÎÒÔÚÄþ²¨Ó´");

        client.close_connect();
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