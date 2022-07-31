//
// Created by HWZ on 2022/4/12.
//

#include "ServerThread.h"
#include <QDebug>

// QString to std::string
inline std::string QStringToStdString(const QString& str)
{
    QByteArray ba = str.toLocal8Bit();
    return { ba.data() };
}

void ServerThread::run()
{
    auto task = [=](const mysock::socketor& client)
    {
        qDebug() << "accept : " << client.address().c_str();
        auto stdName = client.receive();
        if(stdName == "lost connect")
        {
            qDebug() << "lost connect";
            return;
        }
        auto name = QString::fromLocal8Bit(stdName.c_str());
        auto tmp = new ClientThread(new mysock::socketor(client));
        tmp->SetUserName(name);
        tmp->start();
        m_clientThreads.push_back(tmp);
        connect(tmp, &ClientThread::recvMsg, this, [=](const QString& msg)
        {
            recvMsg(tmp, msg);
        });

        connect(tmp, &ClientThread::finished, this, [=]()
        {
            removeClient(tmp);
        });
    };
    while (true)
    {
        auto client = m_server->accept();
        if(client.hasConnected())
            task(client);
    }
}

void ServerThread::stop()
{
    for (auto& i: m_clientThreads)
        i->stop();
    terminate();
}

void ServerThread::removeClient(ClientThread* client)
{
    qDebug() << "client thread finished: " << client->GetSock().address().c_str();
    m_clientThreads.removeOne(client);
    client->deleteLater();
}

void ServerThread::recvMsg(ClientThread* clientThread, const QString& msg)
{
    qDebug() << "recv msg: " << msg;
    QString recvMsg = clientThread->GetUserName() + " (" + clientThread->GetSock().address().c_str() + "):\n" + msg;
    for (auto& i: m_clientThreads)
    {
        if (i != clientThread)
        {
            i->send(recvMsg);
        }
    }

}

