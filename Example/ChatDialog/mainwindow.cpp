#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QMessageBox>
#include "ServerThread.h"



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    // 向监听线程发送关闭信号
    emit stopSignal();

    delete ui;
    delete server;
}

void MainWindow::tryToConnectToServer(const QString& host, const QString& name, int port)
{
    QRegExp isDomainName(R"-(^(([a-zA-Z0-9]|[a-zA-Z0-9][a-zA-Z0-9\-]*[a-zA-Z0-9])\.)*([A-Za-z0-9]|[A-Za-z0-9][A-Za-z0-9\-]*[A-Za-z0-9])$)-");
    if(isDomainName.exactMatch(host))
    {
        qDebug() << "Domain name: " << host;
        auto client = new mysock::Client(host.toLatin1().data(), port);
        if(client->connect2server() != mysock::SUCESS){
            emit connectFailed("Connection failed");
            delete client;
            return;
        }
        client->Send(name.toLocal8Bit().data());

        ui->label->setText(QString::fromLocal8Bit("连接地址： ") + host + QString::fromLocal8Bit(" 端口： ") + QString::number(port));
        auto *clientThread = new ClientThread(client, this);
        clientThread->SetUserName(name);

        connect(ui->sendButton, &QPushButton::clicked, this, [=](){
            QString msg = ui->textEdit->toPlainText();
            if(msg.isEmpty())
                return;
            clientThread->send(msg);
            ui->textBrowser->append(name + "(me):\n" + msg + "\n\n");
            ui->textEdit->clear();
        });
        connect(clientThread, &ClientThread::recvMsg, this, [=](const QString& msg){
            ui->textBrowser->append(msg);
            ui->textBrowser->append("\n\n");
        });
        connect(this, &MainWindow::stopSignal, clientThread, &ClientThread::stop);
        clientThread->start();
        emit connectSuccess();
        show();
    }
    else
    {
        emit connectFailed("Host name is not valid");
        return;
    }

}

void MainWindow::tryToCreateServer(const QString& name, int port)
{
    server = new mysock::server(port);
    if(server->Listen() != mysock::SUCESS){
        emit connectFailed("Server connect failed");
        delete server;
        return;
    }

    ui->label->setText(QString::fromLocal8Bit("连接地址： 127.0.0.1") + QString::fromLocal8Bit(" 端口： ") + QString::number(port));
    auto *serverThread = new ServerThread(server, this);
    serverThread->start();
    connect(this, &MainWindow::stopSignal, serverThread, &ServerThread::stop);

    auto client = new mysock::Client("127.0.0.1", port);
    if(client->connect2server() != mysock::SUCESS){
        emit connectFailed("Connection failed");
        delete client;
        return;
    }
    client->Send(name.toLocal8Bit().data());

    auto clientThread = new ClientThread(client);
    clientThread->SetUserName(name);
    connect(ui->sendButton, &QPushButton::clicked, this, [=](){
        QString msg = ui->textEdit->toPlainText();
        if(msg.isEmpty())
            return;
        clientThread->send(msg);
        ui->textBrowser->append(name +  "(me):\n" + msg + "\n\n");
        ui->textEdit->clear();
    });
    connect(clientThread, &ClientThread::recvMsg, this, [=](const QString& msg){
        ui->textBrowser->append(msg);
        ui->textBrowser->append("\n\n");
    });
    connect(this, &MainWindow::stopSignal, clientThread, &ClientThread::stop);
    clientThread->start();
    emit connectSuccess();
    show();
}


