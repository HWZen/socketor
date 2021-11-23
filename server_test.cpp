#include "sock_server.h"
#include <iostream>
#include <string>
#include "turing.h"
#include "function.h"

using namespace std;

void foo(mysock::socketor client);

int main()
{
    mysock::server Server(5150);
    try
    {
        Server.Listen();
        Server.Accept(foo);
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
        }
    }
    catch (std::exception &e)
    {
        cerr << e.what() << endl;
    }

}

void foo(mysock::socketor client)
{
    string res;
    cout << "ю╢вт" << client.address() << ":  ";
    try
    {
        turing t;
        while ((res = client.receive()) != "lose connect")
        {
            res = mysock::UtfToGbk(res.c_str());
            cout << res << endl;
            std::string ans = t.send(res);
            cout << t.send(res) << endl;
        }
        cout << endl;
    }
    catch (std::exception &e)
    {
        cerr << e.what() << endl;
        throw (e);
    }


}