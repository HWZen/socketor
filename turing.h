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
    Hir_Success = 0,        //成功
    Hir_InitErr,            //初始化失败
    Hir_ConnectErr,            //连接HTTP服务器失败
    Hir_SendErr,            //发送请求失败
    Hir_QueryErr,            //查询HTTP请求头失败
    Hir_404,                //页面不存在
    Hir_IllegalUrl,            //无效的URL
    Hir_CreateFileErr,        //创建文件失败
    Hir_DownloadErr,        //下载失败
    Hir_QueryIPErr,            //获取域名对应的地址失败
    Hir_SocketErr,            //套接字错误
    Hir_UserCancel,            //用户取消下载
    Hir_BufferErr,            //文件太大，缓冲区不足
    Hir_HeaderErr,            //HTTP请求头错误
    Hir_ParamErr,            //参数错误，空指针，空字符
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
    //  通过HTTP请求：Get或Post方式获取JSON信息 [3/14/2017/shike]
    const std::string RequestJsonInfo( const std::string& strUrl,
                                       HttpRequest type = Hr_Get,
                                       std::string lpHeader = "",
                                       std::string lpPostData = "");
    // UTF-8转为GBK2312 [3/14/2017/shike]


protected:
    // 解析卡口Json数据 [3/14/2017/shike]
    std::string ParseJsonInfo(const std::string &strJsonInfo);

    // 关闭句柄 [3/14/2017/shike]
    void Release();

    // 释放句柄 [3/14/2017/shike]
    void ReleaseHandle( HINTERNET& hInternet );

    // 解析URL地址 [3/14/2017/shike]
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
