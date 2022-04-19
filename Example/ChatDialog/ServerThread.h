//
// Created by HWZ on 2022/4/12.
//

#ifndef CHATDIALOG_SERVERTHREAD_H
#define CHATDIALOG_SERVERTHREAD_H

#include <QThread>
#include <server.h>
#include "ClientThread.h"

class ServerThread : public QThread
{
Q_OBJECT

public:

    explicit ServerThread(mysock::Server* server, QObject* parent = nullptr) : QThread(parent), m_server(server)
    {
    }

    ~ServerThread() override = default;

    [[noreturn]] void run() override;

    void stop();

private:
    mysock::Server* m_server;

    QList<ClientThread*> m_clientThreads;

    void recvMsg(ClientThread* clientThread, const QString& msg);

    void removeClient(ClientThread* client);

};


#endif //CHATDIALOG_SERVERTHREAD_H
