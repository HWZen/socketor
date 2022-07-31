#include "server.h"
#include <iostream>
#include <string>

using std::cin, std::cout, std::endl, std::string;

int main(){
    mysock::Server server;
    server.listen();

    auto client = server.accept();
    cout << "accepted: " << client.address() << endl;
    string res = client.receive();
    cout << "received: " << res << endl;
    client.closeConnect();

}