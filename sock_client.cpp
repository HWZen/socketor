#include "sock_client.h"
#ifdef I_OS_WIN
#include <ws2tcpip.h>
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
    if ((hptr = gethostbyname(server_address)) == NULL)
    {
        printf("  gethostbyname error for host:%s\n ", server_address);
        exit(0);
    }
    switch (hptr->h_addrtype)
    {
    case AF_INET:
    case AF_INET6:
        pptr = hptr->h_addr_list;
        server_address = inet_ntop(hptr->h_addrtype, hptr->h_addr, str, sizeof(str));
        break;
    default:
        printf(" unknown address type\n ");
        exit(0);
        break;
    }

#endif

    // 创建客户端套节字
    IN_ADDR tmp;
    tmp.s_addr = inet_addr(server_address.c_str());
    Socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    Socket_info = {PF_INET, htons(server_port), tmp};
    //AF_INET指明使用TCP/IP协议族；
    //SOCK_STREAM, IPPROTO_TCP具体指明使用TCP协议
    // 指明远程服务器的地址信息(端口号、IP地址等)
    // memset(&server, 0, sizeof(SOCKADDR_IN));            //先将保存地址的server置为全0
    // server.sin_family = PF_INET;                        //声明地址格式是TCP/IP地址格式
    // server.sin_port = htons(port);                          //指明连接服务器的端口号，htons()用于 converts values between the host and network byte order
    // server.sin_addr.s_addr = inet_addr(server_address.c_str());       //指明连接服务器的IP地址
    //结构SOCKADDR_IN的sin_addr字段用于保存IP地址，sin_addr字段也是一个结构体，sin_addr.s_addr用于最终保存IP地址
    //inet_addr()用于将 形如的"127.0.0.1"字符串转换为IP地址格式

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


