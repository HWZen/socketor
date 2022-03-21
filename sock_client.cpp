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

        //��������ַ�б�������ɺ����
        int i = 0;
        for (i = 1, pptr = pHostEntry->h_addr_list; *pptr != nullptr; ++pptr)
        {
            memset(szIpBuff, 0, sizeof(szIpBuff));
            //inet_ntop�ķ���ֵΪNULL�����ʾʧ�ܣ����򷵻���Ӧ��IP��ַ����ʱszIpRetָ�����szIpBuff��
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

    // �����ͻ����׽���
    IN_ADDR tmp;
    tmp.s_addr = inet_addr(server_address.c_str());
    Socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    Socket_info = {PF_INET, htons(server_port), tmp};
    //AF_INETָ��ʹ��TCP/IPЭ���壻
    //SOCK_STREAM, IPPROTO_TCP����ָ��ʹ��TCPЭ��
    // ָ��Զ�̷������ĵ�ַ��Ϣ(�˿ںš�IP��ַ��)
    // memset(&server, 0, sizeof(SOCKADDR_IN));            //�Ƚ������ַ��server��Ϊȫ0
    // server.sin_family = PF_INET;                        //������ַ��ʽ��TCP/IP��ַ��ʽ
    // server.sin_port = htons(port);                          //ָ�����ӷ������Ķ˿ںţ�htons()���� converts values between the host and network byte order
    // server.sin_addr.s_addr = inet_addr(server_address.c_str());       //ָ�����ӷ�������IP��ַ
    //�ṹSOCKADDR_IN��sin_addr�ֶ����ڱ���IP��ַ��sin_addr�ֶ�Ҳ��һ���ṹ�壬sin_addr.s_addr�������ձ���IP��ַ
    //inet_addr()���ڽ� �����"127.0.0.1"�ַ���ת��ΪIP��ַ��ʽ

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


