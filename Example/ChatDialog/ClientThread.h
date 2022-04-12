//
// Created by HWZ on 2022/4/12.
//

#ifndef CHATDIALOG_CLIENTTHREAD_H
#define CHATDIALOG_CLIENTTHREAD_H

#include <QThread>
#include <socketor.h>

class ClientThread : public QThread
{
Q_OBJECT
public:
    explicit ClientThread(mysock::socketor* sock, QObject* parent = nullptr) :
            QThread(parent),
            m_sock(sock)
    {
    }

    void run() override;

    void send(const QString &msg);

    const mysock::socketor& GetSock() const;

    virtual ~ClientThread();

    void stop();

    const QString& GetUserName() const;

    void SetUserName(const QString& name);

signals:
    void recvMsg(const QString &msg);

private:
    mysock::socketor* m_sock;
    QString userName;

};


#endif //CHATDIALOG_CLIENTTHREAD_H
