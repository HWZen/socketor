#include "client.h"
#include <iostream>
#include <string>
using std::cin, std::cout, std::endl, std::string;

int main(){
    mysock::Client c;
    if(auto err = c.Connect2Server(); err != mysock::SUCESS)
    {
        cout << "connect fail.\n";
        return 1;
    }

    string s;
    cin >> s;

    c.Send(s);
    c.CloseConnect();

    return 0;
}