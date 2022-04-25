#include "server.h"
#include <iostream>
#include <string>

using std::cin, std::cout, std::endl, std::string;

int main(){
    mysock::Server server;
    server.Listen();
    server.Accept([](mysock::socketor sock){
        cout << "accepted: " << sock.address() << endl;
        string res = sock.receive();
        cout << "received: " << res << endl;
        mysock::Server::CloseConnect(sock);
    });

}