//
// Created by 10414 on 2021/11/7.
//

#ifndef SOCKETOR_TURING_H
#define SOCKETOR_TURING_H

#include <iostream>
#include <Windows.h>
#include <wininet.h>
#define READ_BUFFER_SIZE        4096


enum HttpInterfaceError
{
    Hir_Success = 0,        //�ɹ�
    Hir_InitErr,            //��ʼ��ʧ��
    Hir_ConnectErr,            //����HTTP������ʧ��
    Hir_SendErr,            //��������ʧ��
    Hir_QueryErr,            //��ѯHTTP����ͷʧ��
    Hir_404,                //ҳ�治����
    Hir_IllegalUrl,            //��Ч��URL
    Hir_CreateFileErr,        //�����ļ�ʧ��
    Hir_DownloadErr,        //����ʧ��
    Hir_QueryIPErr,            //��ȡ������Ӧ�ĵ�ַʧ��
    Hir_SocketErr,            //�׽��ִ���
    Hir_UserCancel,            //�û�ȡ������
    Hir_BufferErr,            //�ļ�̫�󣬻���������
    Hir_HeaderErr,            //HTTP����ͷ����
    Hir_ParamErr,            //�������󣬿�ָ�룬���ַ�
    Hir_UnknowErr,
};
enum HttpRequest
{
    Hr_Get,
    Hr_Post
};
class CWininetHttp
{
public:
    CWininetHttp(void);
    ~CWininetHttp(void);

public:
    //  ͨ��HTTP����Get��Post��ʽ��ȡJSON��Ϣ [3/14/2017/shike]
    const std::string RequestJsonInfo( const std::string& strUrl,
                                       HttpRequest type = Hr_Get,
                                       std::string lpHeader = "",
                                       std::string lpPostData = "");
    // UTF-8תΪGBK2312 [3/14/2017/shike]


protected:
    // ��������Json���� [3/14/2017/shike]
    std::string ParseJsonInfo(const std::string &strJsonInfo);

    // �رվ�� [3/14/2017/shike]
    void Release();

    // �ͷž�� [3/14/2017/shike]
    void ReleaseHandle( HINTERNET& hInternet );

    // ����URL��ַ [3/14/2017/shike]
    void ParseURLWeb( std::string strUrl, std::string& strHostName, std::string& strPageName, WORD& sPort);



private:
    HINTERNET            m_hSession;
    HINTERNET            m_hConnect;
    HINTERNET            m_hRequest;
    HttpInterfaceError    m_error;
};
class turing : public CWininetHttp
{

public:
    std::string send(std::string str);

};


#endif //SOCKETOR_TURING_H
