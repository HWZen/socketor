//
// Created by HWZ on 2022/4/12.
//

#include "ClientThread.h"
#include <QDebug>

// QString to std::string
std::string QStringToStdString(const QString &str)
{
    QByteArray ba = str.toLocal8Bit();
    return {ba.data()};
}

void ClientThread::run()
{
    while(true){
        QString str = QString::fromLocal8Bit(m_sock->receive().c_str());
        if (str == "lose connect")
        {
            qDebug() << "lose connect: " << m_sock->address().c_str();
            break;
        }
        emit recvMsg(str);
    }
}

void ClientThread::send(const QString& msg)
{
    auto str = QStringToStdString(msg);
    m_sock->Send(str);
}


void ClientThread::stop()
{
    terminate();
}

const mysock::socketor& ClientThread::GetSock() const
{
    return *m_sock;
}

ClientThread::~ClientThread()
{
    delete m_sock;
}

const QString& ClientThread::GetUserName() const
{
    return userName;
}

void ClientThread::SetUserName(const QString& name)
{
    ClientThread::userName = name;
}
