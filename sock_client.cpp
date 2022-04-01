#include "sock_client.h"
#ifdef I_OS_WIN
#include <ws2tcpip.h>
#endif

#ifdef I_OS_LINUX
using IN_ADDR = in_addr;
#endif

mysock::Client::Client(const char *_server_address, int port)
{
    hasConnected = std::make_shared<bool>(false);
    server_port = port;
    server_address = _server_address;

}

[[nodiscard]]int mysock::Client::connect2server()
{
#ifdef I_OS_WIN

    // Initialize Windows socket library
    WSAStartup(0x0202, &wsaData);
    WORD wVersionRequested = 0;
    int err = 0;

    wVersionRequested = MAKEWORD(2, 2);

    err = WSAStartup(wVersionRequested, &wsaData);

    if (err != 0)
        return WSA_ERROR;


    if (LOBYTE(wsaData.wVersion) != 2 ||
        HIBYTE(wsaData.wVersion) != 2)
    {
        WSACleanup();
        return BYTE_FAIL;
    }
#ifdef _MSC_VER
    char **pptr = nullptr;
    HOSTENT *pHostEntry = gethostbyname(server_address.c_str());
    char szIpBuff[32] = {0};
    if (nullptr != pHostEntry && server_address[0] != '\0')
    {

        //将主机地址列表输出，可含多个
        int i = 0;
        for (i = 1, pptr = pHostEntry->h_addr_list; *pptr != nullptr; ++pptr)
        {
            memset(szIpBuff, 0, sizeof(szIpBuff));
            //inet_ntop的返回值为NULL，则表示失败，否则返回相应的IP地址（此时szIpRet指向的是szIpBuff）
            const char *szIpRet = inet_ntop(pHostEntry->h_addrtype, *pptr, szIpBuff, sizeof(szIpBuff));
            if(szIpRet != nullptr)
                server_address = szIpBuff;
        }

    }
    else{
        err = WSAGetLastError();

        WSACleanup();
        return GET_HOST_NAME_FAIL;
    }
#endif //_MSC_VER

#endif //I_OS_WIN

#ifdef I_OS_LINUX

    char **pptr;
    struct hostent *hptr;
    char str[32];
    if ((hptr = gethostbyname(server_address.c_str())) == NULL)
        return GET_HOST_NAME_FAIL;
    switch (hptr->h_addrtype)
    {
    case AF_INET:
    case AF_INET6:
        pptr = hptr->h_addr_list;
        server_address = inet_ntop(hptr->h_addrtype, hptr->h_addr, str, sizeof(str));
        break;
    default:
        return GET_HOST_NAME_FAIL;
    }

#endif //I_OS_LINUX

    IN_ADDR tmp;
    tmp.s_addr = inet_addr(server_address.c_str());
    Socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    Socket_info = {PF_INET, htons(server_port), tmp};
    if(connect(Socket, (struct sockaddr *)&Socket_info, sizeof(SOCKADDR_IN))<0)
        return CONNECT_FAIL;
    return SUCESS;
}

void mysock::Client::close_connect()
{
    closesocket(Socket);
#ifdef I_OS_WIN
    WSACleanup();
#endif
}


