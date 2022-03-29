//
// Created by 10414 on 2021/11/9.
//
#include <fast_io.h>
#include "boost/asio.hpp"
using namespace boost::asio;
using socket_ptr = boost::shared_ptr<ip::tcp::socket> ;

void client_session(socket_ptr sock) {
    while(true){
        int data[512];
        size_t len = sock->read_some(buffer(data));
        if(len > 4)
            println(len / 4);
        if ( len > 0 && data[len / sizeof(int) - 1] == -1){
            break;
        }
    }
    write(*sock, buffer("ok",2));


}

int main()
{

    io_service server;
    ip::tcp::endpoint ep2( ip::tcp::v4(), 5150); // listen on 2001
    ip::tcp::acceptor acc(server, ep2);
    socket_ptr sock(new ip::tcp::socket(server));
    acc.accept(*sock);
    client_session(sock);

}
