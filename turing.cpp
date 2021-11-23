//
// Created by 10414 on 2021/11/7.
//

#include "turing.h"
//#include "Common.h"
#include <fstream>
#pragma comment(lib, "Wininet.lib")
#include <tchar.h>
#include "document.h"
#include "stringbuffer.h"
#include "writer.h"
#include "filereadstream.h"
#include "function.h"
using namespace std;

CWininetHttp::CWininetHttp():m_hSession(nullptr),m_hConnect(NULL),m_hRequest(NULL)
{
}

CWininetHttp::~CWininetHttp()
{
    Release();
}

//  通过HTTP请求：Get或Post方式获取JSON信息 [3/14/2017/shike]
const std::string CWininetHttp::RequestJsonInfo(const std::string& lpUrl,
                                                HttpRequest type/* = Hr_Get*/,
                                                std::string strHeader/*=""*/,
                                                std::string strPostData/*=""*/)
{
    std::string strRet = "";
    try
    {
        if ( lpUrl.empty())
        {
            throw Hir_ParamErr;
        }
        Release();
        m_hSession = InternetOpen(_T("Http-connect"), INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, NULL);    //局部

        if ( NULL == m_hSession )
        {
            throw Hir_InitErr;
        }

        INTERNET_PORT port    = INTERNET_DEFAULT_HTTP_PORT;
        std::string strHostName = "";
        std::string strPageName = "";

        ParseURLWeb(lpUrl, strHostName, strPageName, port);
//        printf("lpUrl:%s,\nstrHostName:%s,\nstrPageName:%s,\nport:%d\n",lpUrl.c_str(),strHostName.c_str(),strPageName.c_str(),(int)port);

        m_hConnect = InternetConnectA(m_hSession, strHostName.c_str(), port, NULL, NULL, INTERNET_SERVICE_HTTP, NULL, NULL);

        if ( NULL == m_hConnect )
        {
            throw Hir_ConnectErr;
        }

        std::string strRequestType;
        if ( Hr_Get == type )
        {
            strRequestType = "GET";
        }
        else
        {
            strRequestType = "POST";
        }

        m_hRequest = HttpOpenRequestA(m_hConnect,strRequestType.c_str(), strPageName.c_str(),"HTTP/1.1", NULL, NULL, INTERNET_FLAG_RELOAD, NULL);
        if ( NULL == m_hRequest )
        {
            throw Hir_InitErr;
        }

        DWORD dwHeaderSize = (strHeader.empty()) ? 0 : strlen(strHeader.c_str());
        BOOL bRet = FALSE;
        if ( Hr_Get == type )
        {
            bRet = HttpSendRequestA(m_hRequest,strHeader.c_str(),dwHeaderSize,NULL, 0);
        }
        else
        {
            DWORD dwSize = (strPostData.empty()) ? 0 : strlen(strPostData.c_str());
            bRet = HttpSendRequestA(m_hRequest,strHeader.c_str(),dwHeaderSize,(LPVOID)strPostData.c_str(), dwSize);
        }
        if ( !bRet )
        {
            throw Hir_SendErr;
        }

        char szBuffer[READ_BUFFER_SIZE + 1] = {0};
        DWORD dwReadSize = READ_BUFFER_SIZE;
        if ( !HttpQueryInfoA(m_hRequest, HTTP_QUERY_RAW_HEADERS, szBuffer, &dwReadSize, NULL) )
        {
            throw Hir_QueryErr;
        }
        if ( NULL != strstr(szBuffer, "404") )
        {
            throw Hir_404;
        }

        while( true )
        {
            bRet = InternetReadFile(m_hRequest, szBuffer, READ_BUFFER_SIZE, &dwReadSize);
            if ( !bRet || (0 == dwReadSize) )
            {
                break;
            }
            szBuffer[dwReadSize]='\0';
            strRet.append(szBuffer);
        }
    }
    catch(HttpInterfaceError error)
    {
        m_error = error;
    }
    return std::move(strRet);
}

// 解析URL地址 [3/14/2017/shike]
void CWininetHttp::ParseURLWeb( std::string strUrl, std::string& strHostName, std::string& strPageName, WORD& sPort)
{
    sPort = 80;
    string strTemp(strUrl);
    std::size_t nPos = strTemp.find("http://");
    if (nPos != std::string::npos)
    {
        strTemp = strTemp.substr(nPos + 7, strTemp.size() - nPos - 7);
    }

    nPos = strTemp.find('/');
    if ( nPos == std::string::npos )    //没有找到
    {
        strHostName = strTemp;
    }
    else
    {
        strHostName = strTemp.substr(0, nPos);
    }

    std::size_t nPos1 = strHostName.find(':');
    if ( nPos1 != std::string::npos )
    {
        std::string strPort = strTemp.substr(nPos1 + 1, strHostName.size() - nPos1 - 1);
        strHostName = strHostName.substr(0, nPos1);
        sPort = (WORD)atoi(strPort.c_str());
    }
    if ( nPos == std::string::npos )
    {
        return ;
    }
    strPageName = strTemp.substr(nPos, strTemp.size() - nPos);
}

// 关闭句柄 [3/14/2017/shike]
void CWininetHttp::Release()
{
    ReleaseHandle(m_hRequest);
    ReleaseHandle(m_hConnect);
    ReleaseHandle(m_hSession);
}

// 释放句柄 [3/14/2017/shike]
void CWininetHttp::ReleaseHandle( HINTERNET& hInternet )
{
    if (hInternet)
    {
        InternetCloseHandle(hInternet);
        hInternet = NULL;
    }
}

// UTF-8转为GBK2312 [3/14/2017/shike]


 std::string CWininetHttp::ParseJsonInfo(const string &strJsonInfo)
{
    rapidjson::Document d;
    d.Parse(strJsonInfo.c_str());
    auto res = d.FindMember("results");
    if(res != d.MemberEnd())
    {
        for(auto &t : res->value.GetArray())
        {
            if(!strcmp(t.GetObjectA()["resultType"].GetString(),"text"))
            {
                string str = t.GetObjectA()["values"].GetObjectA()["text"].GetString();
                str = mysock::UtfToGbk(str.c_str());
                return str;
            }

        }
    }
    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    d.Accept(writer);
    string tmp = buffer.GetString();
    return tmp;


}

std::string turing::send(string str)
{
    FILE* fp = fopen("../request.json", "rb");
    char readBuffer[65536];
    rapidjson::FileReadStream is(fp, readBuffer, sizeof(readBuffer));
    rapidjson::Document d;
    d.ParseStream(is);
    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    d.Accept(writer);
//    cout << buffer.GetString() << endl;
    auto &V = d["perception"].GetObjectA()["inputText"].GetObjectA()["text"];
    string s = buffer.GetString();
    string buf= mysock::string_To_UTF8(str);
    s.insert(48,buf);
    V.SetString(buf.c_str(), buf.size());

    auto res = RequestJsonInfo("http://openapi.turingapi.com/openapi/api/v2",Hr_Post,"Content-Type:application/json;charset=UTF-8", s);

    return ParseJsonInfo(res);

}
